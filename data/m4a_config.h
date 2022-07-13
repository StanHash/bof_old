#include "m4a.h"

// defined in lds
extern char _M4aNumMusicPlayers[];
extern char _M4aMaxLines[];

#define NUM_MUSIC_PLAYERS ((u16) _M4aNumMusicPlayers)
#define MAX_LINES ((u32) _M4aMaxLines)

enum
{
    SOUND_MODE = (8 << SOUND_MODE_MAXCHN_SHIFT) | (15 << SOUND_MODE_MASVOL_SHIFT)
        | SOUND_MODE_FREQ_15768 | SOUND_MODE_DA_BIT_8
};
