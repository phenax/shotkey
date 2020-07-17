#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct Command {
  char* command;
  unsigned int mode;
  int persist;
} Command;

typedef struct Key {
  unsigned int mod;
  KeySym key;
  Command command;
} Key;

typedef struct ModeProperties {
  char* label;
} ModeProperties;

#define NormalMode -1

#define cmd(c)      { c,     NormalMode,  False }
#define mode(m, p)  { NULL,  m,           p }

#include "config.h"

#define LENGTH(X) (sizeof X / sizeof X[0])
#define CLEANMASK(mask) (mask & ~LockMask & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

int current_mode = NormalMode;
int is_mode_persistent = 0;

extern char** environ;

void bind_key(Display *dpy, Window win, unsigned int mod, KeySym key) {
  int keycode = XKeysymToKeycode(dpy, key);
  XGrabKey(dpy, keycode, mod, win, False, GrabModeAsync, GrabModeAsync);
}
void unbind_key(Display *dpy, Window win, unsigned int mod, KeySym key) {
  int keycode = XKeysymToKeycode(dpy, key);
  XUngrabKey(dpy, keycode, mod, win);
}

void bind_mouse_button(Display *dpy, Window win, unsigned int mod, KeySym button) {
  XGrabButton(dpy, button, mod, win, False, ButtonPress, GrabModeSync, GrabModeAsync, None, None);
}

int error_handler(Display *disp, XErrorEvent *xe) {
  switch(xe->error_code) {
    case BadAccess:
      printf("shotkey: [BadAccess] Cant grab key binding. Already grabbed\n");
      return 0;
  }

  printf("shotkey: Something went wrong\n");
  return 1;
}

void spawn(char** command) {
  if (fork() == 0) {
    setsid();
    execve(command[0], command, environ);
    fprintf(stderr, "shotkey: execve %s", command[0]);
    perror(" failed");
    exit(0);
  }
}

char* get_mode_label() {
  if (current_mode == NormalMode)
    return "";
  if (LENGTH(mode_properties) <= current_mode) 
    return "";

  ModeProperties props = mode_properties[current_mode];
  return props.label;
}

void handle_mode_change() {
  char str[255];

  sprintf(str, "%d", current_mode);
  setenv("SHOTKEY_MODE_ID", str, 1);

  sprintf(str, "%s", get_mode_label());
  setenv("SHOTKEY_MODE_LABEL", str, 1);

  char* cmd[] = {shell, "-c", on_mode_change, NULL};
  spawn(cmd);
}

void set_mode(int mode, unsigned int persist) {
    current_mode = mode;
    is_mode_persistent = persist;
    handle_mode_change();
}

void run(Display* dpy, Window win, Command command) {
  if (command.command) {
    char* cmd[] = {shell, "-c", command.command, NULL};
    spawn(cmd);
  } else if (command.mode != NormalMode) {
    // Bind keyboard for mode
    XGrabKeyboard(dpy, win, False, GrabModeAsync, GrabModeAsync, CurrentTime);

    // Bind an escape key to quit mode
    bind_key(dpy, win, 0, XK_Escape);

    // Set mode
    set_mode(command.mode, command.persist);
  }
}

void keypress(Display *dpy, Window win, XKeyEvent *ev) {
  unsigned int i, stay_in_mode = False;
  Key mode_key;
  KeySym keysym = XKeycodeToKeysym(dpy, (KeyCode) ev->keycode, 0);

  if (current_mode == NormalMode) {
    // Bind all the normal mode keys
    for (i = 0; i < LENGTH(keys); i++) {
      if (keysym == keys[i].key && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)) {
        run(dpy, win, keys[i].command);
      }
    }
  } else {
    stay_in_mode = False;

    if (modes[current_mode] && current_mode < LENGTH(modes)) {
      // Check if key is in mode and execute
      for (i = 0; i < LENGTH(modes[current_mode]); i++) {
        mode_key = modes[current_mode][i];

        if (keysym == mode_key.key && CLEANMASK(mode_key.mod) == CLEANMASK(ev->state)) {
          // Action taken so keep the mode alive
          stay_in_mode = True;
          run(dpy, win, mode_key.command);
        }
      }

      if (is_mode_persistent ? !stay_in_mode : True) {
        // Unbind mode related keys
        XUngrabKeyboard(dpy, CurrentTime);

        // Unbind escape key
        unbind_key(dpy, win, 0, XK_Escape);
      }
    }

    if (is_mode_persistent ? !stay_in_mode : True) {
      set_mode(NormalMode, False);
    }
  }
}

void buttonpress(Display* dpy, Window win, XButtonEvent* ev) {
  printf("Muse click\n");

  // Replay event
  XAllowEvents(dpy, ReplayPointer, CurrentTime);
}

int main() {
  XSetErrorHandler(error_handler);

  int running = 1, i = 0;

  Display *dpy = XOpenDisplay(0);
  Window root = DefaultRootWindow(dpy);

  // Grab keys
  for (i = 0; i < LENGTH(keys); i++) {
    bind_key(dpy, root, keys[i].mod, keys[i].key);
  }

  bind_mouse_button(dpy, root, 0, Button1);

  long int event_mask = KeyPressMask | ButtonPressMask;
  XSelectInput(dpy, root, event_mask);

  handle_mode_change();

  /* main event loop */
  XEvent ev;
  XSync(dpy, False);
  while (running) {
    XMaskEvent(dpy, event_mask, &ev);

    switch (ev.type) {
      case KeyPress:
        keypress(dpy, root, &ev.xkey);
        break;
      case ButtonPress:
        buttonpress(dpy, root, &ev.xbutton);
        break;
    }
  }

  XCloseDisplay(dpy);
}

