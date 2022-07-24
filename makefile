.SUFFIXES:

# ==================
# = PROJECT CONFIG =
# ==================

BUILD_NAME := bof

SRC_DIR = src
ASM_DIR = asm
BUILD_DIR = build

# ====================
# = TOOL DEFINITIONS =
# ====================

TOOLCHAIN ?= $(DEVKITARM)
AGBCC_HOME ?= tools/agbcc

ifneq (,$(TOOLCHAIN))
  export PATH := $(TOOLCHAIN)/bin:$(PATH)
endif

PREFIX := arm-none-eabi-

export OBJCOPY := $(PREFIX)objcopy
export AS := $(PREFIX)as
export CPP := $(PREFIX)cpp
export LD := $(PREFIX)ld
export STRIP := $(PREFIX)strip

CC1 := $(AGBCC_HOME)/bin/agbcc
OLD_CC1 := $(AGBCC_HOME)/bin/old_agbcc

# ================
# = BUILD CONFIG =
# ================

CPPFLAGS := -I $(AGBCC_HOME)/include -iquote include -iquote . -nostdinc -undef
CFLAGS   := -g -mthumb-interwork -Wall -Wno-unused -fhex-asm
ASFLAGS  := -mcpu=arm7tdmi -I include

LDS := $(BUILD_NAME).lds

ROM := $(BUILD_NAME).gba

ELF := $(ROM:%.gba=%.elf)
MAP := $(ROM:%.gba=%.map)

C_SRCS := $(wildcard $(SRC_DIR)/*.c)
C_OBJS := $(C_SRCS:%.c=$(BUILD_DIR)/%.o)

ASM_SRCS := $(wildcard $(SRC_DIR)/*.s $(ASM_DIR)/*.s)
ASM_OBJS := $(ASM_SRCS:%.s=$(BUILD_DIR)/%.o)

ALL_OBJS := $(C_OBJS) $(ASM_OBJS)
ALL_DEPS := $(ALL_OBJS:%.o=%.d)

SUBDIRS := $(sort $(dir $(ALL_OBJS)))
$(shell mkdir -p $(SUBDIRS))

# ===========
# = RECIPES =
# ===========

compare: $(ROM) $(BUILD_NAME).sha1
	sha1sum -c $(BUILD_NAME).sha1

.PHONY: compare

clean:
	@echo "RM $(ROM) $(ELF) $(MAP) $(BUILD_DIR)/"
	@rm -f $(ROM) $(ELF) $(MAP) 
	@rm -fr $(BUILD_DIR)/

.PHONY: clean

%.gba: %.elf
	$(OBJCOPY) --strip-debug -O binary $< $@

$(ELF): $(ALL_OBJS) $(LDS)
	@echo "LD $(LDS) $(ALL_OBJS:$(BUILD_DIR)/%=%)"
	@cd $(BUILD_DIR) && $(LD) -T ../$(LDS) -Map ../$(MAP) -L../tools/agbcc/lib -lc -lgcc $(ALL_OBJS:$(BUILD_DIR)/%=%) -o ../$@
	@$(STRIP) -N .gcc2_compiled. $(ELF)

# C dependency file
$(BUILD_DIR)/%.d: %.c
	@$(CPP) $(CPPFLAGS) $< -o $@ -MM -MG -MT $@ -MT $(BUILD_DIR)/$*.o

# C object
$(BUILD_DIR)/%.o: %.c
	@echo "CC $<"
	@$(CPP) $(CPPFLAGS) $< | $(CC1) $(CFLAGS) -o $(BUILD_DIR)/$*.s
	@echo ".text\n\t.align\t2, 0\n" >> $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@

# ASM dependency file (dummy, generated with the object)
$(BUILD_DIR)/%.d: $(BUILD_DIR)/%.o
	@touch $@

# ASM object
$(BUILD_DIR)/%.o: %.s
	@echo "AS $<"
	@$(AS) $(ASFLAGS) $< -o $@ --MD $(BUILD_DIR)/$*.d

ifneq (clean,$(MAKECMDGOALS))
  -include $(ALL_DEPS)
  .PRECIOUS: $(BUILD_DIR)/%.d
endif

# ======================
# = CFLAGS overrides =
# ======================

%m4a.o: CC1 := $(OLD_CC1)
%m4a.o: CFLAGS += -O2
%agbsram.o: CFLAGS += -O
