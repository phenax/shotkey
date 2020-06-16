#include<stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct Key {
  unsigned int mod;
  KeySym key;
  char* command;
} Key;

#include "config.h"

#define LENGTH(X) (sizeof X / sizeof X[0])
#define CLEANMASK(mask) (mask & ~LockMask & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))

void bind_key(Display *dpy, Window win, unsigned int mod, KeySym key) {
  int keycode = XKeysymToKeycode(dpy, key);
  XGrabKey(dpy, keycode, mod, win, False, GrabModeAsync, GrabModeAsync);
}

int error_handler(Display *disp, XErrorEvent *xe) {
  if (xe->error_code == BadAccess) {
    printf("daemonic: [BadAccess] Cant grab key binding. Already grabbed\n");
    return 0;
  }

  printf("daemonic: Something went wrong");
  return 1;
}

void keypress(Display *dpy, XKeyEvent *ev) {
  unsigned int i;
  KeySym keysym = XKeycodeToKeysym(dpy, (KeyCode) ev->keycode, 0);

  for (i = 0; i < LENGTH(keys); i++) {
    if (keysym == keys[i].key && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state) && keys[i].command) {
      printf("Executin: %s\n", keys[i].command);
    }
  }
}

int main() {
  XSetErrorHandler(error_handler);

  int running = 10;
  int i;

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
    if (ev.type == KeyPress) {
      keypress(dpy, &ev.xkey);
      running--;
    }
  }

	XCloseDisplay(dpy);
}

