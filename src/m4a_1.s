    .include "asm_macros.inc"
    .include "asm_m4a.inc"
    .include "asm_agbio.inc"

    .text

    .syntax unified
    .thumb

    function MulByQ32
MulByQ32:
    adr r2, 1f
    bx r2
1:
    .arm
    umull r2, r3, r0, r1
    add r0, r3, 0
    bx lr
    .thumb

    endfunction MulByQ32

    function SoundMain
SoundMain:
    @ id check
    ldr r0, .L081179B0 @ =SOUND_INFO_PTR
    ldr r0, [r0]
    ldr r2, .L081179B4 @ =ID_NUMBER
    ldr r3, [r0, O_SoundInfo_ident]
    cmp r2, r3
    beq 1f
    @ end if id check failed
    bx lr
1:
    adds r3, 1
    str r3, [r0, O_SoundInfo_ident]
    push {r4, r5, r6, r7, lr}
    mov r1, r8
    mov r2, sb
    mov r3, sl
    mov r4, fp
    push {r0, r1, r2, r3, r4}
    sub sp, 0x18
    ldrb r1, [r0, O_SoundInfo_max_lines]
    cmp r1, 0 @ if max_lines is 0, there is no maximum
    beq .L08117976
    ldr r2, .L081179BC @ =REG_VCOUNT
    ldrb r2, [r2]
    cmp r2, 0xA0
    bhs .L08117974
    adds r2, 0xE4
.L08117974:
    adds r1, r1, r2
.L08117976:
    str r1, [sp, 0x14]
    ldr r3, [r0, O_SoundInfo_mplay_head_func]
    cmp r3, 0
    beq .L08117986
    ldr r0, [r0, O_SoundInfo_mplay_head]
    bl .Lcall_via_r3
    ldr r0, [sp, 0x18] @ restore r0 = *SOUND_INFO_PTR
.L08117986:
    ldr r3, [r0, O_SoundInfo_cgb_sound_func]
    bl .Lcall_via_r3
    ldr r0, [sp, 0x18] @ restore r0 = *SOUND_INFO_PTR
    ldr r3, [r0, O_SoundInfo_pcm_samples_per_vblank]
    mov r8, r3
    ldr r5, .L081179C0 @ =O_SoundInfo_pcm_buffer
    adds r5, r5, r0
    ldrb r4, [r0, O_SoundInfo_pcm_dma_counter]
    subs r7, r4, 1
    bls .L081179A6
    ldrb r1, [r0, O_SoundInfo_pcm_dma_period]
    subs r1, r1, r7
    mov r2, r8
    muls r2, r1, r2
    adds r5, r5, r2
.L081179A6:
    str r5, [sp, 8]
    ldr r6, .L081179C4 @ =PCM_DMA_BUF_SIZE
    ldr r3, .L081179B8 @ =SoundMainRam + 1
    bx r3

    .align 2, 0
.L081179B0: .4byte SOUND_INFO_PTR
.L081179B4: .4byte ID_NUMBER
.L081179B8: .4byte SoundMainRam + 1
.L081179BC: .4byte REG_VCOUNT
.L081179C0: .4byte O_SoundInfo_pcm_buffer
.L081179C4: .4byte PCM_DMA_BUF_SIZE

    endfunction SoundMain

    function SoundMainRam_load
SoundMainRam_load:
    ldrb r3, [r0, O_SoundInfo_reverb]
    cmp r3, 0
    beq .L08117A28
    adr r1, .L081179D4
    bx r1

    .align 2, 0
.L081179D4:
    .arm
    cmp r4, 2
    addeq r7, r0, O_SoundInfo_pcm_buffer
    addne r7, r5, r8
    mov r4, r8
.L081179E4:
    ldsb r0, [r5, r6]
    ldsb r1, [r5]
    add r0, r1
    ldsb r1, [r7, r6]
    add r0, r1
    ldsb r1, [r7], 1
    add r0, r1
    mul r1, r0, r3
    mov r0, r1, asr 9
    tst r0, 0x80
    addne r0, 1
    strb r0, [r5, r6]
    strb r0, [r5], 1
    subs r4, r4, 1
    bgt .L081179E4
    adr r0, .L08117A56 + 1 @ plus 1 because THUMB
    bx r0
    .thumb
.L08117A28:
    movs r0, 0
    mov r1, r8
    adds r6, r5
    lsrs r1, 3
    bcc .L08117A36
    stm r5!, {r0}
    stm r6!, {r0}
.L08117A36:
    lsrs r1, 1
    bcc .L08117A42
    stm r5!, {r0}
    stm r6!, {r0}
    stm r5!, {r0}
    stm r6!, {r0}
.L08117A42:
    stm r5!, {r0}
    stm r6!, {r0}
    stm r5!, {r0}
    stm r6!, {r0}
    stm r5!, {r0}
    stm r6!, {r0}
    stm r5!, {r0}
    stm r6!, {r0}
    subs r1, 1
    bgt .L08117A42
.L08117A56:
    ldr r4, [sp, 0x18]
    ldr r0, [r4, O_SoundInfo_div_freq]
    mov r12, r0
    ldrb r0, [r4, O_SoundInfo_max_chans]
    adds r4, O_SoundInfo_chans
.L08117A60:
    str r0, [sp, 0x04]
    ldr r3, [r4, O_SoundChannel_wav]
    ldr r0, [sp, 0x14]
    cmp r0, 0
    beq 2f
    ldr r1, .L08117A7C @ =REG_VCOUNT
    ldrb r1, [r1]
    cmp r1, 0xA0
    bhs 1f
    adds r1, 0xE4
1:
    cmp r1, r0
    blo 2f
    b .L08117D52

    .align 2, 0
.L08117A7C: .4byte REG_VCOUNT

2:
    ldrb r6, [r4, O_SoundChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r6
    bne 1f
    b .L08117D48
1:
    movs r0, SOUND_CHANNEL_SF_START
    tst r0, r6
    beq .L08117ABA
    movs r0, SOUND_CHANNEL_SF_STOP
    tst r0, r6
    bne .L08117ACA
    movs r6, SOUND_CHANNEL_SF_ENV_ATTACK
    strb r6, [r4, O_SoundChannel_flags]
    adds r0, r3, 0
    adds r0, O_WaveData_data
    str r0, [r4, O_SoundChannel_current_ptr]
    ldr r0, [r3, O_WaveData_size]
    str r0, [r4, O_SoundChannel_count]
    movs r5, 0
    strb r5, [r4, O_SoundChannel_envelope_volume]
    str r5, [r4, O_SoundChannel_fw]
    ldrb r2, [r3, O_WaveData_flags]
    movs r0, WAVE_DATA_FLAG_LOOP
    tst r0, r2
    beq .L08117B12
    movs r0, SOUND_CHANNEL_SF_LOOP
    orrs r6, r0
    strb r6, [r4, O_SoundChannel_flags]
    b .L08117B12
.L08117ABA:
    ldrb r5, [r4, O_SoundChannel_envelope_volume]
    movs r0, SOUND_CHANNEL_SF_IEC
    tst r0, r6
    beq .L08117AD0
    ldrb r0, [r4, O_SoundChannel_pseudo_echo_length]
    subs r0, 1
    strb r0, [r4, O_SoundChannel_pseudo_echo_length]
    bhi .L08117B20
.L08117ACA:
    movs r0, 0
    strb r0, [r4, O_SoundChannel_flags]
    b .L08117D48
.L08117AD0:
    movs r0, SOUND_CHANNEL_SF_STOP
    tst r0, r6
    beq .L08117AF0
    ldrb r0, [r4, O_SoundChannel_release]
    muls r5, r0
    lsrs r5, 8
    ldrb r0, [r4, O_SoundChannel_pseudo_echo_volume]
    cmp r5, r0
    bhi .L08117B20
.L08117AE2:
    ldrb r5, [r4, O_SoundChannel_pseudo_echo_volume]
    cmp r5, 0
    beq .L08117ACA
    movs r0, SOUND_CHANNEL_SF_IEC
    orrs r6, r0
    strb r6, [r4, O_SoundChannel_flags]
    b .L08117B20
.L08117AF0:
    movs r2, SOUND_CHANNEL_SF_ENV
    ands r2, r6
    cmp r2, SOUND_CHANNEL_SF_ENV_DECAY
    bne .L08117B0E
    ldrb r0, [r4, O_SoundChannel_decay]
    muls r5, r0
    lsrs r5, 8
    ldrb r0, [r4, O_SoundChannel_sustain]
    cmp r5, r0
    bhi .L08117B20
    adds r5, r0, 0
    beq .L08117AE2
    subs r6, 1
    strb r6, [r4, O_SoundChannel_flags]
    b .L08117B20
.L08117B0E:
    cmp r2, SOUND_CHANNEL_SF_ENV_ATTACK
    bne .L08117B20
.L08117B12:
    ldrb r0, [r4, O_SoundChannel_attack]
    adds r5, r0
    cmp r5, 0xFF
    bcc .L08117B20
    movs r5, 0xFF
    subs r6, 1
    strb r6, [r4, O_SoundChannel_flags]
.L08117B20:
    strb r5, [r4, O_SoundChannel_envelope_volume]
    ldr r0, [sp, 0x18]
    ldrb r0, [r0, O_SoundChannel_release]
    adds r0, 1
    muls r0, r5
    lsrs r5, r0, 4
    ldrb r0, [r4, O_SoundChannel_volume_right]
    muls r0, r5
    lsrs r0, 8
    strb r0, [r4, O_SoundChannel_envelope_volume_right]
    ldrb r0, [r4, O_SoundChannel_volume_left]
    muls r0, r5
    lsrs r0, 8
    strb r0, [r4, O_SoundChannel_envelope_volume_left]
    movs r0, SOUND_CHANNEL_SF_LOOP
    ands r0, r6
    str r0, [sp, 0x10]
    beq .L08117B54
    adds r0, r3, 0
    adds r0, O_WaveData_data
    ldr r1, [r3, O_WaveData_loop_start]
    adds r0, r1
    str r0, [sp, 0x0C]
    ldr r0, [r3, O_WaveData_size]
    subs r0, r1
    str r0, [sp, 0x10]
.L08117B54:
    ldr r5, [sp, 0x8]
    ldr r2, [r4, O_SoundChannel_count]
    ldr r3, [r4, O_SoundChannel_current_ptr]
    adr r0, .L08117B60
    bx r0

    .align 2, 0
.L08117B60:
    .arm
    str r8, [sp]
    ldrb r10, [r4, O_SoundChannel_envelope_volume_right]
    ldrb r11, [r4, O_SoundChannel_envelope_volume_left]
    mov r10, r10, lsl 16
    mov r11, r11, lsl 16
    ldrb r0, [r4, O_SoundChannel_type]
    tst r0, 0x8
    beq .L08117CA0
.L08117B80:
    cmp r2, 4
    ble .L08117BF0
    subs r2, r2, r8
    movgt lr, 0
    bgt .L08117BAC
    mov lr, r8
    add r2, r8
    sub r8, r2, 4
    sub lr, r8
    ands r2, 0x3
    moveq r2, 4
.L08117BAC:
    ldr r6, [r5]
    ldr r7, [r5, PCM_DMA_BUF_SIZE]
.L08117BB4:
    ldsb r0, [r3], 1
    mul r1, r10, r0
    bic r1, 0xFF0000
    add r6, r1, r6, ror 8
    mul r1, r11, r0
    bic r1, 0xFF0000
    add r7, r1, r7, ror 8
    adds r5, r5, 0x40000000
    bcc .L08117BB4
    str r7, [r5, PCM_DMA_BUF_SIZE]
    str r6, [r5], 4
    subs r8, 4
    bgt .L08117BAC
    adds r8, r8, lr
    beq .L08117D34
.L08117BF0:
    ldr r6, [r5]
    ldr r7, [r5, PCM_DMA_BUF_SIZE]
.L08117BF8:
    ldsb r0, [r3], 1
    mul r1, r10, r0
    bic r1, 0xFF0000
    add r6, r1, r6, ror 8
    mul r1, r11, r0
    bic r1, 0xFF0000
    add r7, r1, r7, ror 8
    subs r2, 1
    beq .L08117C68
.L08117C1C:
    adds r5, r5, 0x40000000
    bcc .L08117BF8
    str r7, [r5, PCM_DMA_BUF_SIZE]
    str r6, [r5], 4
    subs r8, 4
    bgt .L08117B80
    b .L08117D34
.L08117C38:
    ldr r0, [sp, 0x18]
    cmp r0, 0
    beq .L08117C5C
    ldr r3, [sp, 0x14]
    rsb r9, r2, 0
.L08117C4C:
    adds r2, r0, r2
    bgt .L08117D04
    sub r9, r0
    b .L08117C4C
.L08117C5C:
    pop {r4, r12}
    mov r2, 0
    b .L08117C78
.L08117C68:
    ldr r2, [sp, 0x10]
    cmp r2, 0
    ldrne r3, [sp, 0x0C]
    bne .L08117C1C
.L08117C78:
    strb r2, [r4, O_SoundChannel_flags]
    mov r0, r5, lsr 30
    bic r5, r5, 0xC0000000
    rsb r0, r0, 0x3
    mov r0, r0, lsl 3
    mov r6, r6, ror r0
    mov r7, r7, ror r0
    str r7, [r5, PCM_DMA_BUF_SIZE]
    str r6, [r5], 4
    b .L08117D3C
.L08117CA0:
    push {r4, r12}
    ldr lr, [r4, O_SoundChannel_fw]
    ldr r1, [r4, O_SoundChannel_frequency]
    mul r4, r12, r1
    ldsb r0, [r3]
    ldsb r1, [r3, 1]!
    sub r1, r0
.L08117CBC:
    ldr r6, [r5]
    ldr r7, [r5, PCM_DMA_BUF_SIZE]
.L08117CC4:
    mul r9, lr, r1
    add r9, r0, r9, asr 23
    mul r12, r10, r9
    bic r12, 0xFF0000
    add r6, r12, r6, ror 8
    mul r12, r11, r9
    bic r12, 0xFF0000
    add r7, r12, r7, ror 8
    add lr, r4
    movs r9, lr, lsr 23
    beq .L08117D10
    bic lr, lr, 0x3F800000
    subs r2, r9
    ble .L08117C38
    subs r9, 1
    addeq r0, r1
.L08117D04:
    ldsbne r0, [r3, r9]!
    ldsb r1, [r3, 1]!
    sub r1, r0
.L08117D10:
    adds r5, 0x40000000
    bcc .L08117CC4
    str r7, [r5, PCM_DMA_BUF_SIZE]
    str r6, [r5], 4
    subs r8, 4
    bgt .L08117CBC
    sub r3, 1
    pop {r4, r12}
    str lr, [r4, O_SoundChannel_fw]
.L08117D34:
    str r2, [r4, O_SoundChannel_count]
    str r3, [r4, O_SoundChannel_current_ptr]
.L08117D3C:
    ldr r8, [sp]
    adr r0, .L08117D48 + 1
    bx r0
    .thumb
.L08117D48:
    ldr r0, [sp, 0x04]
    subs r0, 1
    ble .L08117D52
    adds r4, SoundChannel_size
    b .L08117A60
.L08117D52:
    ldr r0, [sp, 0x18]
    ldr r3, .L08117D68 @ =ID_NUMBER
    str r3, [r0]
    add sp, 0x1C
    pop {r0-r7}
    mov r8, r0
    mov r9, r1
    mov r10, r2
    mov r11, r3
    pop {r3}
.Lcall_via_r3:
    bx r3

    .align 2, 0
.L08117D68: .4byte ID_NUMBER

    endfunction SoundMainRam_load

    function RealClear64byte
RealClear64byte:
    mov r12, r4
    movs r1, 0
    movs r2, 0
    movs r3, 0
    movs r4, 0
    stm r0!, {r1-r4}
    stm r0!, {r1-r4}
    stm r0!, {r1-r4}
    stm r0!, {r1-r4}
    mov r4, r12
    bx lr

    endfunction RealClear64byte

    function RealClearChain
RealClearChain:
    ldr r3, [r0, O_SoundChannel_track]
    cmp r3, 0
    beq 4f
    ldr r1, [r0, O_SoundChannel_next_ptr]
    ldr r2, [r0, O_SoundChannel_prev_ptr]
    cmp r2, 0
    beq 1f
    str r1, [r2, O_SoundChannel_next_ptr]
    b 2f
1:
    str r1, [r3, O_MusicPlayerTrack_chan]
2:
    cmp r1, 0
    beq 3f
    str r2, [r1, O_SoundChannel_prev_ptr]
3:
    movs r1, 0
    str r1, [r0, O_SoundChannel_track]
4:
    bx lr

    endfunction RealClearChain

    function ply_fine
ply_fine:
    push {r4, r5, lr}
    adds r5, r1, 0
    ldr r4, [r5, O_MusicPlayerTrack_chan]
    cmp r4, 0
    beq .Lply_fine_done
.Lply_fine_loop:
    ldrb r1, [r4, O_SoundChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r1
    beq .Lply_fine_ok
    movs r0, SOUND_CHANNEL_SF_STOP
    orrs r1, r0
    strb r1, [r4, O_SoundChannel_flags]
.Lply_fine_ok:
    adds r0, r4, 0
    bl RealClearChain
    ldr r4, [r4, O_SoundChannel_next_ptr]
    cmp r4, 0
    bne .Lply_fine_loop
.Lply_fine_done:
    movs r0, 0
    strb r0, [r5, O_MusicPlayerTrack_flags]
    pop {r4, r5}
    pop {r0}
    bx r0

    endfunction ply_fine

    function MPlayJumpTableCopy
MPlayJumpTableCopy: @ 0x08117DD4
    mov ip, lr
    movs r1, 0x24
    ldr r2, .L08117E04 @ =MPlayJumpTableTemplate
.L08117DDA:
    ldr r3, [r2]
    bl chk_adr_r2
    stm r0!, {r3}
    adds r2, 4
    subs r1, 1
    bgt .L08117DDA
    bx ip

    .align 2, 0

ldrb_r3_r2:
    ldrb r3, [r2]
    @ fallthrough
chk_adr_r2:
    push {r0}
    lsrs r0, r2, 0x19
    bne .Lchk_adr_r2_done
    ldr r0, .L08117E04 @ =MPlayJumpTableTemplate
    cmp r2, r0
    blo .Lchk_adr_r2_reject
    lsrs r0, r2, 0xE
    beq .Lchk_adr_r2_done
.Lchk_adr_r2_reject:
    movs r3, 0
.Lchk_adr_r2_done:
    pop {r0}
    bx lr

    .align 2, 0
.L08117E04: .4byte MPlayJumpTableTemplate

ld_r3_tp_adr_i:
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    @ fallthrough
st_r2_inc_pt_cmd_ptr:
    adds r3, r2, 1
    str r3, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    b chk_adr_r2

    endfunction MPlayJumpTableCopy

    function ply_goto
ply_goto:
    push {lr}
    @ fallthrough
ply_goto_inner:
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r0, [r2, 3]
    lsls r0, 8
    ldrb r3, [r2, 2]
    orrs r0, r3
    lsls r0, 8
    ldrb r3, [r2, 1]
    orrs r0, r3
    lsls r0, 8
    bl ldrb_r3_r2
    orrs r0, r3
    str r0, [r1, O_MusicPlayerTrack_cmd_ptr]
    pop {r0}
    bx r0

    endfunction ply_goto

    function ply_patt
ply_patt:
    ldrb r2, [r1, O_MusicPlayerTrack_patternLevel]
    cmp r2, 3
    bhs .Lply_patt_done
    lsls r2, 2
    adds r3, r1, r2
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    adds r2, 4
    str r2, [r3, O_MusicPlayerTrack_pattern_stack]
    ldrb r2, [r1, O_MusicPlayerTrack_patternLevel]
    adds r2, 1
    strb r2, [r1, O_MusicPlayerTrack_patternLevel]
    b ply_goto
.Lply_patt_done:
    b ply_fine

    endfunction ply_patt

    function ply_pend
ply_pend:
    ldrb r2, [r1, O_MusicPlayerTrack_patternLevel]
    cmp r2, 0
    beq .Lply_pend_done
    subs r2, 1
    strb r2, [r1, O_MusicPlayerTrack_patternLevel]
    lsls r2, 2
    adds r3, r1, r2
    ldr r2, [r3, O_MusicPlayerTrack_pattern_stack]
    str r2, [r1, O_MusicPlayerTrack_cmd_ptr]
.Lply_pend_done:
    bx lr

    endfunction ply_pend

    function ply_rept
ply_rept:
    push {lr}
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    cmp r3, 0
    bne .Lply_rept_1
    adds r2, 1
    str r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    b ply_goto_inner
.Lply_rept_1:
    ldrb r3, [r1, O_MusicPlayerTrack_repN]
    adds r3, 1
    strb r3, [r1, O_MusicPlayerTrack_repN]
    mov r12, r3
    bl ld_r3_tp_adr_i
    cmp r12, r3
    bhs .Lply_rept_2
    b ply_goto_inner
.Lply_rept_2:
    movs r3, 0
    strb r3, [r1, O_MusicPlayerTrack_repN]
    adds r2, 5
    str r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    pop {r0}
    bx r0

    endfunction ply_rept

    function ply_prio
ply_prio:
    mov r12, lr
    bl ld_r3_tp_adr_i
    strb r3, [r1, O_MusicPlayerTrack_priority]
    bx r12

    endfunction ply_prio

    function ply_tempo
ply_tempo:
    mov r12, lr
    bl ld_r3_tp_adr_i
    lsls r3, 1
    strh r3, [r0, O_MusicPlayer_tempo_d]
    ldrh r2, [r0, O_MusicPlayer_tempo_u]
    muls r3, r2
    lsrs r3, 8
    strh r3, [r0, O_MusicPlayer_tempo_i]
    bx r12

    endfunction ply_tempo

    function ply_keysh
ply_keysh:
    mov r12, lr
    bl ld_r3_tp_adr_i
    strb r3, [r1, O_MusicPlayerTrack_key_shift]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, 0x0C
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_keysh

    function ply_voice
ply_voice:
    mov r12, lr
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    adds r2, 1
    str r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    lsls r2, r3, 1
    adds r2, r3
    lsls r2, 2
    ldr r3, [r0, O_MusicPlayer_tone]
    adds r2, r3
    ldr r3, [r2, O_ToneData_type]
    bl chk_adr_r2
    str r3, [r1, O_MusicPlayerTrack_ToneData_type]
    ldr r3, [r2, O_ToneData_wav]
    bl chk_adr_r2
    str r3, [r1, O_MusicPlayerTrack_ToneData_wav]
    ldr r3, [r2, O_ToneData_attack]
    bl chk_adr_r2
    str r3, [r1, O_MusicPlayerTrack_ToneData_attack]
    bx r12

    endfunction ply_voice

    function ply_vol
ply_vol:
    mov r12, lr
    bl ld_r3_tp_adr_i
    strb r3, [r1, O_MusicPlayerTrack_vol]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_VOLCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_vol

    function ply_pan
ply_pan:
    mov r12, lr
    bl ld_r3_tp_adr_i
    subs r3, C_V
    strb r3, [r1, O_MusicPlayerTrack_pan]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_VOLCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_pan

    function ply_bend
ply_bend:
    mov r12, lr
    bl ld_r3_tp_adr_i
    subs r3, C_V
    strb r3, [r1, O_MusicPlayerTrack_bend]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_PITCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_bend

    function ply_bendr
ply_bendr:
    mov r12, lr
    bl ld_r3_tp_adr_i
    strb r3, [r1, O_MusicPlayerTrack_bend_range]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_PITCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_bendr

    function ply_lfodl
ply_lfodl:
    mov r12, lr
    bl ld_r3_tp_adr_i
    strb r3, [r1, O_MusicPlayerTrack_lfo_delay]
    bx r12

    endfunction ply_lfodl

    function ply_modt
ply_modt:
    mov r12, lr
    bl ld_r3_tp_adr_i
    ldrb r0, [r1, O_MusicPlayerTrack_mod_t]
    cmp r0, r3
    beq .Lply_modt_done
    strb r3, [r1, O_MusicPlayerTrack_mod_t]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_VOLCHG | MPT_FLG_PITCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
.Lply_modt_done:
    bx r12
    endfunction ply_modt

    function ply_tune
ply_tune:
    mov r12, lr
    bl ld_r3_tp_adr_i
    subs r3, C_V
    strb r3, [r1, O_MusicPlayerTrack_tune]
    ldrb r3, [r1, O_MusicPlayerTrack_flags]
    movs r2, MPT_FLG_PITCHG
    orrs r3, r2
    strb r3, [r1, O_MusicPlayerTrack_flags]
    bx r12

    endfunction ply_tune

    function ply_port
ply_port:
    mov r12, lr
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    adds r2, 1
    ldr r0, .L08117F94 @ =REG_SOUND_BASE
    adds r0, r3
    bl st_r2_inc_pt_cmd_ptr
    strb r3, [r0]
    bx r12

    .align 2, 0
.L08117F94: .4byte REG_SOUND_BASE

    endfunction ply_port

    function m4aSoundVSync
m4aSoundVSync: @ 0x08117F98
    ldr r0, .L08118244 @ =SOUND_INFO_PTR
    ldr r0, [r0]
    ldr r2, .L08118248 @ =ID_NUMBER
    ldr r3, [r0, O_SoundInfo_ident]
    subs r3, r3, r2
    cmp r3, 1
    bhi .L08117FD8
    ldrb r1, [r0, O_SoundInfo_pcm_dma_counter]
    subs r1, 1
    strb r1, [r0, O_SoundInfo_pcm_dma_counter]
    bgt .L08117FD8
    ldrb r1, [r0, O_SoundInfo_pcm_dma_period]
    strb r1, [r0, O_SoundInfo_pcm_dma_counter]
    ldr r2, .L08117FDC @ =REG_DMA1SRC
    ldr r1, [r2, REG_DMA1CNT - REG_DMA1SRC]
    lsls r1, r1, 7
    blo .L08117FBE
    ldr r1, .L08117FE0 @ =0x84400004 @ (DMA_ENABLE | DMA_START_NOW | DMA_32BIT | DMA_SRC_INCREMENT | DMA_DST_FIXED) | 4
    str r1, [r2, REG_DMA1CNT - REG_DMA1SRC]
.L08117FBE:
    ldr r1, [r2, REG_DMA2CNT - REG_DMA1SRC]
    lsls r1, r1, 7
    blo .L08117FC8
    ldr r1, .L08117FE0 @ =0x84400004 @ (DMA_ENABLE | DMA_START_NOW | DMA_32BIT | DMA_SRC_INCREMENT | DMA_DST_FIXED) | 4
    str r1, [r2, REG_DMA2CNT - REG_DMA1SRC]
.L08117FC8:
    movs r1, 4 @ DMA_32BIT >> 24
    lsls r1, r1, 8
    strh r1, [r2, REG_DMA1CNT_H - REG_DMA1SRC]
    strh r1, [r2, REG_DMA2CNT_H - REG_DMA1SRC]
    movs r1, 0xB6 @ (DMA_ENABLE | DMA_START_SPECIAL | DMA_32BIT | DMA_REPEAT) >> 24
    lsls r1, r1, 8
    strh r1, [r2, REG_DMA1CNT_H - REG_DMA1SRC]
    strh r1, [r2, REG_DMA2CNT_H - REG_DMA1SRC]
.L08117FD8:
    bx lr

    .align 2, 0
.L08117FDC: .4byte REG_DMA1SRC
.L08117FE0: .4byte 0x84400004 @ (DMA_ENABLE | DMA_START_NOW | DMA_32BIT | DMA_SRC_INCREMENT | DMA_DST_FIXED) | 4

    endfunction m4aSoundVSync

    function MPlayMain
MPlayMain:
    ldr r2, .L08118248 @ =ID_NUMBER
    ldr r3, [r0, O_MusicPlayer_ident]
    cmp r2, r3
    beq .L08117FEE
    bx lr
.L08117FEE:
    adds r3, 1
    str r3, [r0, O_MusicPlayer_ident]
    push {r0, lr}
    ldr r3, [r0, O_MusicPlayer_next_func]
    cmp r3, 0
    beq .L08118000
    ldr r0, [r0, O_MusicPlayer_next]
    bl .Lbx_r3
.L08118000:
    pop {r0}
    push {r4-r7}
    mov r4, r8
    mov r5, r9
    mov r6, r10
    mov r7, r11
    push {r4-r7}
    adds r7, r0, 0
    ldr r0, [r7, O_MusicPlayer_status]
    cmp r0, 0
    bge .L08118018
    b .L0811822C
.L08118018:
    ldr r0, .L08118244 @ =SOUND_INFO_PTR
    ldr r0, [r0]
    mov r8, r0
    adds r0, r7, 0
    bl FadeOutBody
    ldr r0, [r7, O_MusicPlayer_status]
    cmp r0, 0
    bge .L0811802C
    b .L0811822C
.L0811802C:
    ldrh r0, [r7, O_MusicPlayer_tempo_c]
    ldrh r1, [r7, O_MusicPlayer_tempo_i]
    adds r0, r1
    b .L0811817C
.L08118034:
    ldrb r6, [r7, O_MusicPlayer_track_count]
    ldr r5, [r7, O_MusicPlayer_tracks]
    movs r3, 1
    movs r4, 0
.L0811803C:
    ldrb r0, [r5, O_MusicPlayerTrack_flags]
    movs r1, MPT_FLG_EXIST
    tst r1, r0
    bne .L08118046
    b .L08118158
.L08118046:
    mov r10, r3
    orrs r4, r3
    mov r11, r4
    ldr r4, [r5, O_MusicPlayerTrack_chan]
    cmp r4, 0
    beq .L0811807A
.L08118052:
    ldrb r1, [r4, O_SoundChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r1
    beq .L0811806E
    ldrb r0, [r4, O_SoundChannel_gate_time]
    cmp r0, 0
    beq .L08118074
    subs r0, 1
    strb r0, [r4, O_SoundChannel_gate_time]
    bne .L08118074
    movs r0, SOUND_CHANNEL_SF_STOP
    orrs r1, r0
    strb r1, [r4, O_SoundChannel_flags]
    b .L08118074
.L0811806E:
    adds r0, r4, 0
    bl ClearChain
.L08118074:
    ldr r4, [r4, O_SoundChannel_next_ptr]
    cmp r4, 0
    bne .L08118052
.L0811807A:
    ldrb r3, [r5, O_MusicPlayerTrack_flags]
    movs r0, MPT_FLG_START
    tst r0, r3
    beq .L081180F8
    adds r0, r5, 0
    bl Clear64byte
    movs r0, MPT_FLG_EXIST
    strb r0, [r5, O_MusicPlayerTrack_flags]
    movs r0, 0x2
    strb r0, [r5, O_MusicPlayerTrack_bend_range]
    movs r0, 0x40
    strb r0, [r5, O_MusicPlayerTrack_vol_x]
    movs r0, 0x16
    strb r0, [r5, O_MusicPlayerTrack_lfo_speed]
    movs r0, 1
    adds r1, r5, 6
    strb r0, [r1, O_MusicPlayerTrack_ToneData_type - 6]
    b .L081180F8
.L081180A0:
    ldr r2, [r5, O_MusicPlayerTrack_cmd_ptr]
    ldrb r1, [r2]
    cmp r1, 0x80
    bhs .L081180AC
    ldrb r1, [r5, O_MusicPlayerTrack_running_status]
    b .L081180B6
.L081180AC:
    adds r2, 1
    str r2, [r5, O_MusicPlayerTrack_cmd_ptr]
    cmp r1, 0xBD
    bcc .L081180B6
    strb r1, [r5, O_MusicPlayerTrack_running_status]
.L081180B6:
    cmp r1, 0xCF
    bcc .L081180CC
    mov r0, r8
    ldr r3, [r0, O_SoundInfo_plynote_func]
    adds r0, r1, 0
    subs r0, 0xCF
    adds r1, r7, 0
    adds r2, r5, 0
    bl .Lbx_r3
    b .L081180F8
.L081180CC:
    cmp r1, 0xB0
    bls .L081180EE
    adds r0, r1, 0
    subs r0, 0xB1
    strb r0, [r7, O_MusicPlayer_cmd]
    mov r3, r8
    ldr r3, [r3, O_SoundInfo_mplay_jump_table]
    lsls r0, 2
    ldr r3, [r3, r0]
    adds r0, r7, 0
    adds r1, r5, 0
    bl .Lbx_r3
    ldrb r0, [r5, O_MusicPlayerTrack_flags]
    cmp r0, 0
    beq .L08118154
    b .L081180F8
.L081180EE:
    ldr r0, .L08118240 @ =ClockTable
    subs r1, 0x80
    adds r1, r0
    ldrb r0, [r1]
    strb r0, [r5, O_MusicPlayerTrack_wait]
.L081180F8:
    ldrb r0, [r5, O_MusicPlayerTrack_wait]
    cmp r0, 0
    beq .L081180A0
    subs r0, 1
    strb r0, [r5, O_MusicPlayerTrack_wait]
    ldrb r1, [r5, O_MusicPlayerTrack_lfo_speed]
    cmp r1, 0
    beq .L08118154
    ldrb r0, [r5, O_MusicPlayerTrack_mod]
    cmp r0, 0
    beq .L08118154
    ldrb r0, [r5, O_MusicPlayerTrack_lfo_delay_c]
    cmp r0, 0
    beq .L0811811A
    subs r0, 1
    strb r0, [r5, O_MusicPlayerTrack_lfo_delay_c]
    b .L08118154
.L0811811A:
    ldrb r0, [r5, O_MusicPlayerTrack_lfo_speed_c]
    adds r0, r1
    strb r0, [r5, O_MusicPlayerTrack_lfo_speed_c]
    adds r1, r0, 0
    subs r0, 0x40
    lsls r0, 24
    bpl .L0811812E
    lsls r2, r1, 24
    asrs r2, 24
    b .L08118132
.L0811812E:
    movs r0, 0x80
    subs r2, r0, r1
.L08118132:
    ldrb r0, [r5, O_MusicPlayerTrack_mod]
    muls r0, r2
    asrs r2, r0, 6
    ldrb r0, [r5, O_MusicPlayerTrack_mod_m]
    eors r0, r2
    lsls r0, 24
    beq .L08118154
    strb r2, [r5, O_MusicPlayerTrack_mod_m]
    ldrb r0, [r5]
    ldrb r1, [r5, O_MusicPlayerTrack_mod_t]
    cmp r1, 0
    bne .L0811814E
    movs r1, MPT_FLG_PITCHG
    b .L08118150
.L0811814E:
    movs r1, MPT_FLG_VOLCHG
.L08118150:
    orrs r0, r1
    strb r0, [r5, O_MusicPlayerTrack_flags]
.L08118154:
    mov r3, r10
    mov r4, r11
.L08118158:
    subs r6, 1
    ble .L08118164
    movs r0, MusicPlayerTrack_size
    adds r5, r0
    lsls r3, 1
    b .L0811803C
.L08118164:
    ldr r0, [r7, O_MusicPlayer_clock]
    adds r0, 1
    str r0, [r7, O_MusicPlayer_clock]
    cmp r4, 0
    bne .L08118176
    movs r0, 0x80
    lsls r0, 24
    str r0, [r7, O_MusicPlayer_status]
    b .L0811822C
.L08118176:
    str r4, [r7, O_MusicPlayer_status]
    ldrh r0, [r7, O_MusicPlayer_tempo_c]
    subs r0, 150
.L0811817C:
    strh r0, [r7, O_MusicPlayer_tempo_c]
    cmp r0, 150
    bcc .L08118184
    b .L08118034
.L08118184:
    ldrb r2, [r7, O_MusicPlayer_track_count]
    ldr r5, [r7, O_MusicPlayer_tracks]
.L08118188:
    ldrb r0, [r5, O_MusicPlayerTrack_flags]
    movs r1, 0x80
    tst r1, r0
    beq .L08118222
    movs r1, MPT_FLG_VOLCHG | MPT_FLG_PITCHG
    tst r1, r0
    beq .L08118222
    mov r9, r2
    adds r0, r7, 0
    adds r1, r5, 0
    bl TrkVolPitSet
    ldr r4, [r5, O_MusicPlayerTrack_chan]
    cmp r4, 0
    beq .L08118218
.L081181A6:
    ldrb r1, [r4, O_SoundChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r1
    bne .L081181B6
    adds r0, r4, 0
    bl ClearChain
    b .L08118212
.L081181B6:
    ldrb r0, [r4, O_SoundChannel_type]
    movs r6, TONEDATA_TYPE_CGB
    ands r6, r0
    ldrb r3, [r5, O_MusicPlayerTrack_flags]
    movs r0, MPT_FLG_VOLCHG
    tst r0, r3
    beq .L081181D4
    bl ChnVolSetAsm
    cmp r6, 0
    beq .L081181D4
    ldrb r0, [r4, O_CgbChannel_modify]
    movs r1, 1
    orrs r0, r1
    strb r0, [r4, O_CgbChannel_modify]
.L081181D4:
    ldrb r3, [r5, O_MusicPlayerTrack_flags]
    movs r0, MPT_FLG_PITCHG
    tst r0, r3
    beq .L08118212
    ldrb r1, [r4, O_SoundChannel_key]
    movs r0, O_MusicPlayerTrack_key_m
    ldsb r0, [r5, r0]
    adds r2, r1, r0
    bpl .L081181E8
    movs r2, 0
.L081181E8:
    cmp r6, 0
    beq .L08118206
    mov r0, r8
    ldr r3, [r0, O_SoundInfo_midi_to_cgb_func]
    adds r1, r2, 0
    ldrb r2, [r5, O_MusicPlayerTrack_pit_m]
    adds r0, r6, 0
    bl .Lbx_r3
    str r0, [r4, O_CgbChannel_frequency]
    ldrb r0, [r4, O_CgbChannel_modify]
    movs r1, CGB_CHANNEL_MO_PIT
    orrs r0, r1
    strb r0, [r4, O_CgbChannel_modify]
    b .L08118212
.L08118206:
    adds r1, r2, 0
    ldrb r2, [r5, O_MusicPlayerTrack_pit_m]
    ldr r0, [r4, O_SoundChannel_wav]
    bl MidiKeyToFreq
    str r0, [r4, O_SoundChannel_frequency]
.L08118212:
    ldr r4, [r4, O_SoundChannel_next_ptr]
    cmp r4, 0
    bne .L081181A6
.L08118218:
    ldrb r0, [r5, O_MusicPlayerTrack_flags]
    movs r1, 0xF0
    ands r0, r1
    strb r0, [r5, O_MusicPlayerTrack_flags]
    mov r2, r9
.L08118222:
    subs r2, 1
    ble .L0811822C
    movs r0, MusicPlayerTrack_size
    adds r5, r0
    bgt .L08118188
.L0811822C:
    ldr r0, .L08118248 @ =ID_NUMBER
    str r0, [r7, O_MusicPlayer_ident]
    pop {r0-r7}
    mov r8, r0
    mov r9, r1
    mov r10, r2
    mov r11, r3
    pop {r3}
.Lbx_r3:
    bx r3

    .align 2, 0
.L08118240: .4byte ClockTable
.L08118244: .4byte SOUND_INFO_PTR
.L08118248: .4byte ID_NUMBER

    endfunction MPlayMain

    function TrackStop
TrackStop: @ 0x0811824C
    push {r4, r5, r6, lr}
    adds r5, r1, 0
    ldrb r1, [r5]
    movs r0, 0x80
    tst r0, r1
    beq .L08118284
    ldr r4, [r5, 0x20]
    cmp r4, 0
    beq .L08118282
    movs r6, 0
.L08118260:
    ldrb r0, [r4]
    cmp r0, 0
    beq .L0811827A
    ldrb r0, [r4, 1]
    movs r3, 7
    ands r0, r3
    beq .L08118278
    ldr r3, .L0811828C @ =SOUND_INFO_PTR
    ldr r3, [r3]
    ldr r3, [r3, 0x2C]
    bl .Lbx_r3
.L08118278:
    strb r6, [r4]
.L0811827A:
    str r6, [r4, 0x2C]
    ldr r4, [r4, 0x34]
    cmp r4, 0
    bne .L08118260
.L08118282:
    str r4, [r5, 0x20]
.L08118284:
    pop {r4, r5, r6}
    pop {r0}
    bx r0

    .align 2, 0
.L0811828C: .4byte SOUND_INFO_PTR

    endfunction TrackStop

    function ChnVolSetAsm
ChnVolSetAsm:
    ldrb r1, [r4, O_SoundChannel_velocity]
    movs r0, O_SoundChannel_rhythm_pan
    ldrsb r2, [r4, r0]
    movs r3, 0x80
    adds r3, r2
    muls r3, r1
    ldrb r0, [r5, O_MusicPlayerTrack_vol_mr]
    muls r0, r3
    asrs r0, 14
    cmp r0, 0xFF
    bls .L081182A8
    movs r0, 0xFF
.L081182A8:
    strb r0, [r4, O_SoundChannel_volume_right]
    movs r3, 0x7F
    subs r3, r2
    muls r3, r1
    ldrb r0, [r5, O_MusicPlayerTrack_vol_ml]
    muls r0, r3
    asrs r0, 14
    cmp r0, 0xFF
    bls .L081182BC
    movs r0, 0xFF
.L081182BC:
    strb r0, [r4, O_SoundChannel_volume_left]
    bx lr

    endfunction ChnVolSetAsm

    @ note: documented addresses are wrong from here on (they we copied over from pokeemerald)
    @ couldn't be bothered to change them

    function ply_note
ply_note:
    push {r4-r7,lr}
    mov r4, r8
    mov r5, r9
    mov r6, r10
    mov r7, r11
    push {r4-r7}
    sub sp, 0x18
    str r1, [sp]
    adds r5, r2, 0
    ldr r1, =SOUND_INFO_PTR
    ldr r1, [r1]
    str r1, [sp, 0x4]
    ldr r1, =ClockTable
    adds r0, r1
    ldrb r0, [r0]
    strb r0, [r5, O_MusicPlayerTrack_gate_time]
    ldr r3, [r5, O_MusicPlayerTrack_cmd_ptr]
    ldrb r0, [r3]
    cmp r0, 0x80
    bhs .L081DDB46
    strb r0, [r5, O_MusicPlayerTrack_key]
    adds r3, 1
    ldrb r0, [r3]
    cmp r0, 0x80
    bhs .L081DDB44
    strb r0, [r5, O_MusicPlayerTrack_velocity]
    adds r3, 1
    ldrb r0, [r3]
    cmp r0, 0x80
    bhs .L081DDB44
    ldrb r1, [r5, O_MusicPlayerTrack_gate_time]
    adds r1, r0
    strb r1, [r5, O_MusicPlayerTrack_gate_time]
    adds r3, 1
.L081DDB44:
    str r3, [r5, O_MusicPlayerTrack_cmd_ptr]
.L081DDB46:
    movs r0, 0
    str r0, [sp, 0x14]
    adds r4, r5, 0
    adds r4, O_MusicPlayerTrack_ToneData_type
    ldrb r2, [r4]
    movs r0, TONEDATA_TYPE_RHY | TONEDATA_TYPE_SPL
    tst r0, r2
    beq .L081DDB98
    ldrb r3, [r5, O_MusicPlayerTrack_key]
    movs r0, TONEDATA_TYPE_SPL
    tst r0, r2
    beq .L081DDB66
    ldr r1, [r5, O_MusicPlayerTrack_ToneData_keySplitTable]
    adds r1, r3
    ldrb r0, [r1]
    b .L081DDB68
.L081DDB66:
    adds r0, r3, 0
.L081DDB68:
    lsls r1, r0, 1
    adds r1, r0
    lsls r1, 2
    ldr r0, [r5, O_MusicPlayerTrack_ToneData_wav]
    adds r1, r0
    mov r9, r1
    mov r6, r9
    ldrb r1, [r6]
    movs r0, TONEDATA_TYPE_SPL | TONEDATA_TYPE_RHY
    tst r0, r1
    beq .L081DDB80
    b .L081DDCEA
.L081DDB80:
    movs r0, TONEDATA_TYPE_RHY
    tst r0, r2
    beq .L081DDB9C
    ldrb r1, [r6, O_ToneData_pan_sweep]
    movs r0, 0x80
    tst r0, r1
    beq .L081DDB94
    subs r1, TONEDATA_P_S_PAN
    lsls r1, 1
    str r1, [sp, 0x14]
.L081DDB94:
    ldrb r3, [r6, O_SoundChannel_type]
    b .L081DDB9C
.L081DDB98:
    mov r9, r4
    ldrb r3, [r5, O_MusicPlayerTrack_key]
.L081DDB9C:
    str r3, [sp, 0x8]
    ldr r6, [sp]
    ldrb r1, [r6, O_MusicPlayer_priority]
    ldrb r0, [r5, O_MusicPlayerTrack_priority]
    adds r0, r1
    cmp r0, 0xFF
    bls .L081DDBAC
    movs r0, 0xFF
.L081DDBAC:
    str r0, [sp, 0x10]
    mov r6, r9
    ldrb r0, [r6, O_ToneData_type]
    movs r6, TONEDATA_TYPE_CGB
    ands r6, r0
    str r6, [sp, 0xC]
    beq .L081DDBEC
    ldr r0, [sp, 0x4]
    ldr r4, [r0, O_SoundInfo_cgb_chans]
    cmp r4, 0
    bne .L081DDBC4
    b .L081DDCEA
.L081DDBC4:
    subs r6, 1
    lsls r0, r6, 6
    adds r4, r0
    ldrb r1, [r4, O_CgbChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r1
    beq .L081DDC40
    movs r0, SOUND_CHANNEL_SF_STOP
    tst r0, r1
    bne .L081DDC40
    ldrb r1, [r4, O_CgbChannel_priority]
    ldr r0, [sp, 0x10]
    cmp r1, r0
    bcc .L081DDC40
    beq .L081DDBE4
    b .L081DDCEA
.L081DDBE4:
    ldr r0, [r4, O_CgbChannel_track]
    cmp r0, r5
    bcs .L081DDC40
    b .L081DDCEA
.L081DDBEC:
    ldr r6, [sp, 0x10]
    adds r7, r5, 0
    movs r2, 0
    mov r8, r2
    ldr r4, [sp, 0x4]
    ldrb r3, [r4, O_SoundInfo_max_chans]
    adds r4, O_SoundInfo_chans
.L081DDBFA:
    ldrb r1, [r4, O_SoundChannel_flags]
    movs r0, SOUND_CHANNEL_SF_ON
    tst r0, r1
    beq .L081DDC40
    movs r0, SOUND_CHANNEL_SF_STOP
    tst r0, r1
    beq .L081DDC14
    cmp r2, 0
    bne .L081DDC18
    adds r2, 1
    ldrb r6, [r4, O_SoundChannel_priority]
    ldr r7, [r4, O_SoundChannel_track]
    b .L081DDC32
.L081DDC14:
    cmp r2, 0
    bne .L081DDC34
.L081DDC18:
    ldrb r0, [r4, O_SoundChannel_priority]
    cmp r0, r6
    bcs .L081DDC24
    adds r6, r0, 0
    ldr r7, [r4, O_SoundChannel_track]
    b .L081DDC32
.L081DDC24:
    bhi .L081DDC34
    ldr r0, [r4, O_SoundChannel_track]
    cmp r0, r7
    bls .L081DDC30
    adds r7, r0, 0
    b .L081DDC32
.L081DDC30:
    bcc .L081DDC34
.L081DDC32:
    mov r8, r4
.L081DDC34:
    adds r4, SoundChannel_size
    subs r3, 1
    bgt .L081DDBFA
    mov r4, r8
    cmp r4, 0
    beq .L081DDCEA
.L081DDC40:
    adds r0, r4, 0
    bl ClearChain
    movs r1, 0
    str r1, [r4, O_SoundChannel_prev_ptr]
    ldr r3, [r5, O_MusicPlayerTrack_chan]
    str r3, [r4, O_SoundChannel_next_ptr]
    cmp r3, 0
    beq .L081DDC54
    str r4, [r3, O_SoundChannel_prev_ptr]
.L081DDC54:
    str r4, [r5, O_MusicPlayerTrack_chan]
    str r5, [r4, O_SoundChannel_track]
    ldrb r0, [r5, O_MusicPlayerTrack_lfo_delay]
    strb r0, [r5, O_MusicPlayerTrack_lfo_delay_c]
    cmp r0, r1
    beq .L081DDC66
    adds r1, r5, 0
    bl clear_mod_m
.L081DDC66:
    ldr r0, [sp]
    adds r1, r5, 0
    bl TrkVolPitSet
    ldr r0, [r5, O_MusicPlayerTrack_gate_time]
    str r0, [r4, O_SoundChannel_gate_time]
    ldr r0, [sp, 0x10]
    strb r0, [r4, O_SoundChannel_priority]
    ldr r0, [sp, 0x8]
    strb r0, [r4, O_SoundChannel_key]
    ldr r0, [sp, 0x14]
    strb r0, [r4, O_SoundChannel_rhythm_pan]
    mov r6, r9
    ldrb r0, [r6, O_ToneData_type]
    strb r0, [r4, O_SoundChannel_type]
    ldr r7, [r6, O_ToneData_wav]
    str r7, [r4, O_SoundChannel_wav]
    ldr r0, [r6, O_ToneData_attack]
    str r0, [r4, O_SoundChannel_attack]
    ldrh r0, [r5, O_MusicPlayerTrack_pseudo_echo_volume]
    strh r0, [r4, O_SoundChannel_pseudo_echo_volume]
    bl ChnVolSetAsm
    ldrb r1, [r4, O_SoundChannel_key]
    movs r0, O_MusicPlayerTrack_key_m
    ldsb r0, [r5, r0]
    adds r3, r1, r0
    bpl .L081DDCA0
    movs r3, 0
.L081DDCA0:
    ldr r6, [sp, 0xC]
    cmp r6, 0
    beq .L081DDCCE
    mov r6, r9
    ldrb r0, [r6, O_ToneData_length]
    strb r0, [r4, O_CgbChannel_length]
    ldrb r1, [r6, O_ToneData_pan_sweep]
    movs r0, 0x80
    tst r0, r1
    bne .L081DDCBA
    movs r0, 0x70
    tst r0, r1
    bne .L081DDCBC
.L081DDCBA:
    movs r1, 0x8
.L081DDCBC:
    strb r1, [r4, O_CgbChannel_sweep]
    ldrb r2, [r5, O_MusicPlayerTrack_pit_m]
    adds r1, r3, 0
    ldr r0, [sp, 0xC]
    ldr r3, [sp, 0x4]
    ldr r3, [r3, O_SoundInfo_midi_to_cgb_func]
    bl .Lbx_r3
    b .L081DDCDC
.L081DDCCE:
    ldrb r2, [r5, O_MusicPlayerTrack_pit_m]
    adds r1, r3, 0
    adds r0, r7, 0
    bl MidiKeyToFreq
.L081DDCDC:
    str r0, [r4, O_SoundChannel_frequency]
    movs r0, SOUND_CHANNEL_SF_START
    strb r0, [r4, O_SoundChannel_flags]
    ldrb r1, [r5, O_MusicPlayerTrack_flags]
    movs r0, 0xF0
    ands r0, r1
    strb r0, [r5, O_MusicPlayerTrack_flags]
.L081DDCEA:
    add sp, 0x18
    pop {r0-r7}
    mov r8, r0
    mov r9, r1
    mov r10, r2
    mov r11, r3
    pop {r0}
    bx r0

    .pool

    endfunction ply_note

    function ply_endtie
ply_endtie:
    push {r4, r5}
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    cmp r3, 0x80
    bhs .Lply_endtie_load_key
    strb r3, [r1, O_MusicPlayerTrack_key]
    adds r2, #1
    str r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    b 1f
.Lply_endtie_load_key:
    ldrb r3, [r1, O_MusicPlayerTrack_key]
1:
    ldr r1, [r1, O_MusicPlayerTrack_chan]
    cmp r1, 0
    beq .Lply_endtie_done
    movs r4, SOUND_CHANNEL_SF_START | SOUND_CHANNEL_SF_ENV
    movs r5, SOUND_CHANNEL_SF_STOP
.Lply_endtie_loop:
    ldrb r2, [r1, O_SoundChannel_flags]
    tst r2, r4
    beq .Lply_endtie_continue
    tst r2, r5
    bne .Lply_endtie_continue
    ldrb r0, [r1, O_SoundChannel_midi_key]
    cmp r0, r3
    bne .Lply_endtie_continue
    movs r0, SOUND_CHANNEL_SF_STOP
    orrs r2, r0
    strb r2, [r1, O_SoundChannel_flags]
    b .Lply_endtie_done
.Lply_endtie_continue:
    ldr r1, [r1, O_SoundChannel_next_ptr]
    cmp r1, 0
    bne .Lply_endtie_loop
.Lply_endtie_done:
    pop {r4, r5}
    bx lr

    endfunction ply_endtie

    .align 2, 0
clear_mod_m:
    movs r2, 0
    strb r2, [r1, O_MusicPlayerTrack_mod_m]
    strb r2, [r1, O_MusicPlayerTrack_lfo_speed_c]
    ldrb r2, [r1, O_MusicPlayerTrack_mod_t]
    cmp r2, 0
    bne .Lclear_mod_m_volchg
    movs r2, MPT_FLG_PITCHG
    b 1f
.Lclear_mod_m_volchg:
    movs r2, MPT_FLG_VOLCHG
1:
    ldrb r3, [r1]
    orrs r3, r2
    strb r3, [r1]
    bx lr

    .align 2, 0
ld_r3_tp_adr_i_unchecked:
    ldr r2, [r1, O_MusicPlayerTrack_cmd_ptr]
    adds r3, r2, 1
    str r3, [r1, O_MusicPlayerTrack_cmd_ptr]
    ldrb r3, [r2]
    bx lr

    function ply_lfos
ply_lfos:
    mov r12, lr
    bl ld_r3_tp_adr_i_unchecked
    strb r3, [r1, O_MusicPlayerTrack_lfo_speed]
    cmp r3, 0
    bne .Lply_lfos_done
    bl clear_mod_m
.Lply_lfos_done:
    bx r12

    endfunction ply_lfos

    function ply_mod
ply_mod:
    mov r12, lr
    bl ld_r3_tp_adr_i_unchecked
    strb r3, [r1, O_MusicPlayerTrack_mod]
    cmp r3, 0
    bne .Lply_mod_done
    bl clear_mod_m
.Lply_mod_done:
    bx r12

    endfunction ply_mod

    .align 2, 0
