#include <X11/XF86keysym.h>

// Shell
char shell[] = "/bin/sh";

#define Super Mod4Mask

enum {
  Music,
  Power,

  // Declare modes above this
  MODE_SIZE,
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
Key modes[MODE_SIZE][10] = {
  [Music] = {
    { 0, XK_h,       cmd("playerctl previous") },
    { 0, XK_l,       cmd("playerctl next") },
    { 0, XK_space,   cmd("playerctl play-pause") },
  },
  [Power] = {
    { 0, XK_s,       cmd("notify-send shutting_down") },
    { 0, XK_r,       cmd("notify-send restarting") },
  },
};

// Define normal mode key bindings here
Key keys[] = {
  { Mod1Mask,  XK_y,             cmd("notify-send hello") },
  { Mod1Mask,  XK_z,             mode(Music, True) },
  { Mod1Mask,  XF86XK_PowerOff,  mode(Power, False) },
};

ModeProperties mode_properties[MODE_SIZE] = {
  [Music] = { "Music player" },
  [Power] = { "Power menu" },
};

// Call this script on mode change
char* on_mode_change = "echo \"kadj [$SHOTKEY_MODE_ID] $SHOTKEY_MODE_LABEL\"";

