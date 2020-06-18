#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

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

#define cmd(c)      (Command) { c,     NormalMode,  False }
#define mode(m, p)  (Command) { NULL,  m,           p }

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

int error_handler(Display *disp, XErrorEvent *xe) {
  switch(xe->error_code) {
    case BadAccess:
      printf("hotkeythingy: [BadAccess] Cant grab key binding. Already grabbed\n");
      return 0;
  }

  printf("hotkeythingy: Something went wrong\n");
  return 1;
}

void spawn(char** command) {
	if (fork() == 0) {
		setsid();
		execve(command[0], command, environ);
		fprintf(stderr, "hotkeythingy: execve %s", command[0]);
		perror(" failed");
		exit(0);
	}
}

char* get_mode_label() {
  if (current_mode == NormalMode) return "";
  if (LENGTH(mode_properties) <= current_mode) return "";
  ModeProperties props = mode_properties[current_mode];
  return props.label;
}

void handle_mode_change() {
  char str[255];

  sprintf(str, "%d", current_mode);
  setenv("MODE_ID", str, 1);

  sprintf(str, "%s", get_mode_label());
  setenv("MODE_LABEL", str, 1);

  char* cmd[] = {shell, "-c", on_mode_change, NULL};
  spawn(cmd);
}

void set_mode(int mode, unsigned int persist) {
    current_mode = mode;
    is_mode_persistent = persist;
    handle_mode_change();
}

void run(Display* dpy, Window win, Command command) {
  Key mode_key;
  unsigned int i;

  if (command.command) {
    char* cmd[] = {shell, "-c", command.command, NULL};
    spawn(cmd);
  } else if(command.mode != NormalMode) {
    // Bind keyboard for mode
    XGrabKeyboard(dpy, win, False, GrabModeAsync, GrabModeAsync, CurrentTime);

    // Bind an escape key to quit mode
    bind_key(dpy, win, 0, XK_Escape);

    // Set mode
    set_mode(command.mode, command.persist);
  }
}

void keypress(Display *dpy, Window win, XKeyEvent *ev) {
  unsigned int i;
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
    // Will quit if the key pressed is not defined in the mode
    is_mode_persistent = False;

    if (modes[current_mode] && current_mode < LENGTH(modes)) {
      // Check if key is in mode and execute
      for (i = 0; i < LENGTH(modes[current_mode]); i++) {
        mode_key = modes[current_mode][i];

        if (keysym == mode_key.key && CLEANMASK(mode_key.mod) == CLEANMASK(ev->state)) {
          // Action taken so keep the mode alive
          is_mode_persistent = True;
          run(dpy, win, mode_key.command);
        }
      }

      if (!is_mode_persistent) {
        // Unbind mode related keys
        XUngrabKeyboard(dpy, CurrentTime);

        // Unbind escape key
        unbind_key(dpy, win, 0, XK_Escape);
      }
    }

    if (!is_mode_persistent) {
      set_mode(NormalMode, False);
    }
  }
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

  XSelectInput(dpy, root, KeyPressMask);

  handle_mode_change();

	/* main event loop */
  XEvent ev;
	XSync(dpy, False);
	while (running) {
    XMaskEvent(dpy, KeyPressMask, &ev);

	  switch (ev.type) {
      case KeyPress: {
        keypress(dpy, root, &ev.xkey);
        break;
      }
    }
  }

	XCloseDisplay(dpy);
}

