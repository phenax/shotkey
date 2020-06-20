#include <X11/XF86keysym.h>

// Shell
static char shell[] = "/bin/sh";

#define Super Mod4Mask

#define SCRIPT(str) cmd("~/scripts/" #str)

enum {
  MusicPlayer,

  // Declare modes above this
  MODE_SIZE,
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
static Key modes[MODE_SIZE][10] = {
  [MusicPlayer] = { // {{{
    { 0, XK_l,        SCRIPT(spotify.sh next) },
    { 0, XK_h,        SCRIPT(spotify.sh prev) },
    { 0, XK_space,    SCRIPT(spotify.sh play_pause) },
  },// }}}
};

// Define normal mode key bindings here
static Key keys[] = {
  // Mod                        Key                          // Command

  // Application {{{
  { Super,                      XK_Return,                   cmd("sensible-terminal -d $(xcwd)") },
  { Super|ShiftMask,            XK_Return,                   cmd("~/.bin/tst -d $(xcwd)") },

  { Super,                      XK_c,                        cmd("sensible-browser") },
  { Super|ShiftMask,            XK_c,                        cmd("sensible-private-browser") },

  { Super|ShiftMask,            XK_f,                        cmd("sensible-terminal -d $(xcwd) -e lf") },
  // }}}

  // Menus {{{
  { Super,                      XK_d,                        cmd("dmenu_run") },
  { Super,                      XK_semicolon,                cmd("dmenu_run -it ':'") },

  { Super|ShiftMask,            XK_p,                        cmd("passmenu --type -p 'pass :: '") },
  { Super|ShiftMask,            XK_o,                        SCRIPT(open-project.sh) },
  { Super|ShiftMask,            XK_i,                        SCRIPT(bookmarks-menu/bookmarker.sh menu) },
  { Super|ShiftMask,            XK_g,                        SCRIPT(bookmarks-menu/gh-projects.sh menu) },
  { Super|ShiftMask,            XK_e,                        SCRIPT(fontawesome-menu/fa-menu.sh menu) },
  { Super,                      XK_y,                        cmd("clipmenu") },
  // }}}

  // System {{{
  { 0,                          XF86XK_PowerOff,             SCRIPT(powercontrol.sh menu) },
  { ControlMask|ShiftMask,      XK_l,                        SCRIPT(powercontrol.sh lock) },
  { 0,                          XF86XK_MonBrightnessUp,      SCRIPT(brightness.sh inc 10) },
  { 0,                          XF86XK_MonBrightnessDown,    SCRIPT(brightness.sh dec 10) },
  { 0,                          XK_Print,                    SCRIPT(screenshot.sh full) },
  { ControlMask,                XK_Print,                    SCRIPT(screenshot.sh part) },
  // }}}

  // Media controls {{{
  { Super,                      XK_m,                        mode(MusicPlayer, True) },
  { 0,                          XF86XK_AudioRaiseVolume,     SCRIPT(sound.sh volume up) },
  { 0,                          XF86XK_AudioLowerVolume,     SCRIPT(sound.sh volume down) },
  { 0,                          XF86XK_AudioMute,            SCRIPT(sound.sh mute) },
  { 0,                          XF86XK_AudioMicMute,         SCRIPT(sound.sh mute-mic) },
  // }}}
};

static ModeProperties mode_properties[MODE_SIZE] = {
  [MusicPlayer] = { "Music player" },
};

// Call this script on mode change
static char* on_mode_change = "~/scripts/shotkey.sh on-mode-change";

