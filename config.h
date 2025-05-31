#include <X11/XF86keysym.h>

// Shell
char shell[] = "/bin/sh";

#define Super Mod4Mask
#define AltMask Mod1Mask

#define SCRIPT(str) cmd("~/scripts/" #str)
#define NOOP cmd("")

enum {
  MusicPlayer,
  Screenshot,

  // Declare modes above this
  MODE_SIZE,
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
Key modes[MODE_SIZE][20] = {
  [MusicPlayer] = { // {{{
    { 0, XK_l,        SCRIPT(music/player.sh next) },
    { 0, XK_h,        SCRIPT(music/player.sh prev) },
    { 0, XK_space,    SCRIPT(music/player.sh play_pause) },
  },// }}}
  [Screenshot] = { // {{{
    { 0, XK_f,        SCRIPT(screenshot.sh full) },
    { 0, XK_p,        SCRIPT(screenshot.sh part) },
    { 0, XK_w,        SCRIPT(screenshot.sh window) },
  },// }}}
};

// Define normal mode key bindings here
Key keys[] = {
  // Mod                        Key                          // Command

  // Application
  { Super,                      XK_Return,                   cmd("sensible-terminal -d $(xcwd)") },

  { Super,                      XK_c,                        cmd("zsh -c sensible-browser") },
  { Super|ShiftMask,            XK_c,                        cmd("zsh -c sensible-private-browser") },
  { Super,                      XK_b,                        cmd("brave") },
  { Super|ControlMask,          XK_b,                        cmd("firefox") },
  { Super|ControlMask,          XK_n,                        cmd("zsh -c null-browser") },

  { Super|ShiftMask,            XK_f,                        cmd("sensible-terminal -d $(xcwd) -e lf") },

  // Menus
  { Super,                      XK_d,                        cmd("dmenu_run") },
  { Super,                      XK_semicolon,                cmd("dmenu_run -it ':'") },
  { Super|ShiftMask,            XK_d,                        cmd("j4-dmenu-desktop") },

  { Super|ShiftMask,            XK_p,                        cmd("passmenu --type -p 'pass :: '") },
  { Super|ShiftMask,            XK_o,                        SCRIPT(open-project.sh) },
  { Super|ShiftMask,            XK_e,                        SCRIPT(fontawesome-menu/fa-menu.sh menu) },

  // System
  { 0,                            XF86XK_PowerOff,             SCRIPT(powercontrol.sh menu) },
  { ControlMask,                  XK_Delete,                   SCRIPT(powercontrol.sh menu) },
  { Super|ControlMask|ShiftMask,  XK_l,                        SCRIPT(powercontrol.sh lock) },
  { 0,                            XF86XK_MonBrightnessUp,      SCRIPT(brightness.sh inc 10) },
  { 0,                            XF86XK_MonBrightnessDown,    SCRIPT(brightness.sh dec 10) },
  { Super,                        XK_Print,                    mode(Screenshot, False) },
  { Super|AltMask,                XK_m,                        cmd("chelleport") },
  { Super|AltMask,                XK_s,                        cmd("chelleport -m search") },
  { Super|AltMask,                XK_t,                        cmd("dsoys") },

  // Media controls
  // { Super|AltMask,              XK_m,                        mode(MusicPlayer, True) },
  { 0,                          XF86XK_AudioRaiseVolume,     SCRIPT(sound.sh volume up) },
  { 0,                          XF86XK_AudioLowerVolume,     SCRIPT(sound.sh volume down) },
  { 0,                          XF86XK_AudioMute,            SCRIPT(sound.sh mute) },
  { 0,                          XF86XK_AudioMicMute,         SCRIPT(sound.sh mute-mic) },
  { 0,                          XF86XK_AudioNext,            SCRIPT(music/player.sh next) },
  { 0,                          XF86XK_AudioPrev,            SCRIPT(music/player.sh prev) },
  { 0,                          XF86XK_AudioPlay,            SCRIPT(music/player.sh play_pause) },

  // Notifications
  { ControlMask,              XK_space,                    cmd("dunstctl close") },
  { ControlMask|ShiftMask,    XK_space,                    cmd("dunstctl close-all") },
  { ControlMask|ShiftMask,    XK_period,                   cmd("dunstctl context") },
  { ControlMask,              XK_grave,                    cmd("dunstctl history-pop") },
};

ModeProperties mode_properties[MODE_SIZE] = {
  [MusicPlayer] = { "Music player" },
  [Screenshot] = { "Screeshot" },
};

// Call this script on mode change
char* on_mode_change = "~/scripts/shotkey.sh on-mode-change";

