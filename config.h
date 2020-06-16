// Shell
static char shell[] = "sh";

#define MOD Mod1Mask

enum {
  Music,

  // Declare modes above this
  MODE_SIZE, // NOTE: Do not remove this
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
static Key modes[MODE_SIZE][10] = {
  [Music] = {
    { MOD|ShiftMask, XK_m,   cmd("notify-send inside") },
  },
};

// Define normal mode key bindings here
static Key keys[] = {
  { MOD|ShiftMask, XK_y,   cmd("notify-send hello") },
  { MOD|ShiftMask, XK_z,   mode(Music) },
};

