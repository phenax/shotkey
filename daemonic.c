#include<stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <unistd.h>

typedef struct Command {
  char* command;
  unsigned int mode;
} Command;

typedef struct Key {
  unsigned int mod;
  KeySym key;
  Command command;
} Key;

#define cmd(c)  (Command) { c,    -1 }
#define mode(m) (Command) { NULL, m }

#include "config.h"

#define LENGTH(X) (sizeof X / sizeof X[0])
#define CLEANMASK(mask) (mask & ~LockMask & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

int current_mode = -1;

void bind_key(Display *dpy, Window win, unsigned int mod, KeySym key) {
  int keycode = XKeysymToKeycode(dpy, key);
  XGrabKey(dpy, keycode, mod, win, False, GrabModeAsync, GrabModeAsync);
}
void unbind_key(Display *dpy, Window win, unsigned int mod, KeySym key) {
  int keycode = XKeysymToKeycode(dpy, key);
  XUngrabKey(dpy, keycode, mod, win);
}

int error_handler(Display *disp, XErrorEvent *xe) {
  if (xe->error_code == BadAccess) {
    printf("daemonic: [BadAccess] Cant grab key binding. Already grabbed\n");
    return 0;
  }

  printf("daemonic: Something went wrong\n");
  return 1;
}

void spawn(char ** command) {
	if (fork() == 0) {
		setsid();
		execvp(command[0], command);
		fprintf(stderr, "dwm: execvp %s", command[0]);
		perror(" failed");
		exit(0);
	}
}

void run(Display* dpy, Window win, Command command) {
  Key mode_key;
  unsigned int i;

  if (command.command) {
    printf("Running cmd %s\n", command.command);
    char* cmd[] = {shell, "-c", command.command, NULL};
    spawn(cmd);
  } else if(command.mode != -1) {
    printf("Enabling mode %d\n", command.mode);

    current_mode = command.mode;

    if (modes[current_mode] && current_mode < MODE_SIZE) {
      for (i = 0; i < LENGTH(modes[current_mode]); i++) {
        mode_key = modes[current_mode][i];
        bind_key(dpy, win, keys[i].mod, keys[i].key);
      }
    }
    // TODO: Bind keys associated with mode
    // TODO: Bind Escape
  }
}

void keypress(Display *dpy, Window win, XKeyEvent *ev) {
  unsigned int i;
  Key mode_key;
  KeySym keysym = XKeycodeToKeysym(dpy, (KeyCode) ev->keycode, 0);

  printf("kpress\n");

  if (current_mode == -1) {
    for (i = 0; i < LENGTH(keys); i++) {
      if (keysym == keys[i].key && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)) {
        run(dpy, win, keys[i].command);
      }
    }
  } else {
    // TODO: Check and execute the mode
    if (modes[current_mode] && current_mode < MODE_SIZE) {
      for (i = 0; i < LENGTH(modes[current_mode]); i++) {
        mode_key = modes[current_mode][i];

        if (keysym == mode_key.key && CLEANMASK(mode_key.mod) == CLEANMASK(ev->state)) {
          run(dpy, win, mode_key.command);
        }
      }
    }

    // TODO: Unbind mode keys

    current_mode = -1;
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

	/* main event loop */
  XEvent ev;
	XSync(dpy, False);
	while (running && !XNextEvent(dpy, &ev)) {
	  switch (ev.type) {
      case KeyPress: {
        keypress(dpy, root, &ev.xkey);
      }
    }
  }

	XCloseDisplay(dpy);
}

