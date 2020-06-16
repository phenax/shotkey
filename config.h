// Shell
static char shell[] = "sh";

#define MOD Mod1Mask

enum {
  Music,
MODE_SIZE };

static Key modes[MODE_SIZE][5] = {
  [Music] = {
    { MOD|ShiftMask, XK_m,   cmd("notify-send inside") },
  },
};

static Key keys[] = {
  { MOD|ShiftMask, XK_y,   cmd("notify-send hello") },
  { MOD|ShiftMask, XK_z,   mode(Music) },
};

