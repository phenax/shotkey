// Shell
static char shell[] = "/bin/sh";

#define MOD Mod1Mask

enum {
  Music,
  Bright,

  // Declare modes above this
  MODE_SIZE, // NOTE: Do not remove this
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
static Key modes[MODE_SIZE][10] = {
  [Music] = {
    { 0, XK_m,   cmd("notify-send inside_music") },
  },
  [Bright] = {
    { 0, XK_m,   cmd("notify-send inside_bright") },
    { 0, XK_n,   cmd("notify-send inside_bright_1") },
  },
};

// Define normal mode key bindings here
static Key keys[] = {
  { MOD|ShiftMask, XK_y,   cmd("notify-send hello") },
  { MOD|ShiftMask, XK_z,   mode(Music, False) },
  { MOD|ShiftMask, XK_x,   mode(Bright, True) },
};

static ModeProperties mode_properties[MODE_SIZE] = {
  [Music] = { "Music player" },
  [Bright] = { "Brightness" },
};

// Call this script on mode change
static char* on_mode_change = "notify-send \"kadj [$MODE_ID] $MODE_LABEL\"";

