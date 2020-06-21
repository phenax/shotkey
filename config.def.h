// Shell
char shell[] = "/bin/sh";

#define Super Mod4Mask

enum {
  Music,
  Bright,

  // Declare modes above this
  MODE_SIZE,
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
Key modes[MODE_SIZE][10] = {
  [Music] = {
    { 0, XK_m,   cmd("notify-send inside_music") },
  },
  [Bright] = {
    { 0, XK_m,   cmd("notify-send inside_bright") },
    { 0, XK_n,   cmd("notify-send inside_bright_1") },
  },
};

// Define normal mode key bindings here
Key keys[] = {
  { Super|ShiftMask, XK_y,   cmd("notify-send hello") },
  { Super|ShiftMask, XK_z,   mode(Music, False) },
  { Super|ShiftMask, XK_x,   mode(Bright, True) },
};

ModeProperties mode_properties[MODE_SIZE] = {
  [Music] = { "Music player" },
  [Bright] = { "Brightness" },
};

// Call this script on mode change
char* on_mode_change = "notify-send \"kadj [$SHOTKEY_MODE_ID] $SHOTKEY_MODE_LABEL\"";

