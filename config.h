#include <X11/XF86keysym.h>

// Shell
char shell[] = "/bin/sh";

#define Super Mod4Mask

#define SCRIPT(str) cmd("~/scripts/" #str)
#define DWMLAYOUT(str) cmd("dwmc layout " #str)
#define NOOP cmd("")

enum {
  MusicPlayer,
  Screenshot,
  Mouse,
  Layout,

  // Declare modes above this
  MODE_SIZE,
};

// Define mode key bindings here
// NOTE: "10" here is the maximum number of key bindings for each mode
Key modes[MODE_SIZE][20] = {
  [Layout] = { // {{{
    { 0, XK_t,         DWMLAYOUT(vtile) },
    { 0, XK_h,         DWMLAYOUT(htile) },
    { 0, XK_g,         DWMLAYOUT(grid) },
    { 0, XK_d,         DWMLAYOUT(deck) },
  }, // }}}
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
  [Mouse] = { // {{{
    { 0, XK_h,           SCRIPT(mouse.sh move left 30) },
    { 0, XK_j,           SCRIPT(mouse.sh move down 30) },
    { 0, XK_k,           SCRIPT(mouse.sh move up 30) },
    { 0, XK_l,           SCRIPT(mouse.sh move right 30) },

    { 0, XK_Shift_L,   NOOP },
    { 0, XK_Shift_R,   NOOP },
    { ShiftMask, XK_h,        SCRIPT(mouse.sh move left 200) },
    { ShiftMask, XK_j,        SCRIPT(mouse.sh move down 200) },
    { ShiftMask, XK_k,        SCRIPT(mouse.sh move up 200) },
    { ShiftMask, XK_l,        SCRIPT(mouse.sh move right 200) },

    { 0, XK_space,    SCRIPT(mouse.sh click) },
    { 0, XK_c,        SCRIPT(mouse.sh rclick) },
  },// }}}
};

// Define normal mode key bindings here
Key keys[] = {
  // Mod                        Key                          // Command

  // Application {{{
  { Super,                      XK_Return,                   cmd("sensible-terminal -d $(xcwd)") },
  { Super|ControlMask,          XK_Return,                   cmd("sensible-terminal") },
  { Super|ShiftMask,            XK_Return,                   cmd("~/.bin/tst -d $(xcwd)") },

  { Super,                      XK_c,                        cmd("sensible-browser") },
  { Super|ShiftMask,            XK_c,                        cmd("sensible-private-browser") },

  { Super|ShiftMask,            XK_f,                        cmd("sensible-terminal -d $(xcwd) -e lf") },
  // }}}

  // Menus {{{
  { Super,                      XK_d,                        cmd("dmenu_run") },
  { Super,                      XK_semicolon,                cmd("dmenu_run -it ':'") },

  { Super|ShiftMask,            XK_p,                        cmd("passmenu --type -p 'pass :: '") },
  { Super|ShiftMask,            XK_t,                        SCRIPT(change-tagname.sh) },
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
  { Super,                      XK_Print,                    mode(Screenshot, False) },
  { Super|ControlMask,          XK_m,                        mode(Mouse, True) },
  { Super,                      XK_l,                        mode(Layout, False) },
  // }}}

  // Media controls {{{
  { Super,                      XK_m,                        mode(MusicPlayer, True) },
  { 0,                          XF86XK_AudioRaiseVolume,     SCRIPT(sound.sh volume up) },
  { 0,                          XF86XK_AudioLowerVolume,     SCRIPT(sound.sh volume down) },
  { 0,                          XF86XK_AudioMute,            SCRIPT(sound.sh mute) },
  { 0,                          XF86XK_AudioMicMute,         SCRIPT(sound.sh mute-mic) },
  // }}}
};

ModeProperties mode_properties[MODE_SIZE] = {
  [MusicPlayer] = { "Music player" },
  [Screenshot] = { "Screeshot" },
  [Mouse] = { "Mouse control" },
  [Layout] = { "Layout" },
};

// Call this script on mode change
char* on_mode_change = "~/scripts/shotkey.sh on-mode-change";

