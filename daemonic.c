#include<stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

typedef struct Key {
  unsigned int mod;
  KeySym key;
  char* command;
} Key;

#include "config.h"

#define LENGTH(X)               (sizeof X / sizeof X[0])

void
bind_key(Display *dpy, Window win, unsigned int mod, KeySym key)
{
  int keycode = XKeysymToKeycode(dpy, key);

  XGrabKey(dpy, keycode, mod, win, False, GrabModeAsync, GrabModeAsync);
}

int
error_handler(Display *disp, XErrorEvent *xe)
{
  if (xe->error_code == BadAccess) {
    printf("daemonic: [BadAccess] Cant grab key binding. Already grabbed\n");
    return 0;
  }

  printf("daemonic: Something went wrong");
  return 1;
}

int
main()
{
  int running = 10;
  XSetErrorHandler(error_handler);

  Display *dpy = XOpenDisplay(0);
  Window root = DefaultRootWindow(dpy);

  // Grab keys
  int i;
  for (i = 0; i < LENGTH(keys); i++) {
    bind_key(dpy, root, keys[i].mod, keys[i].key);
  }

  XSelectInput(dpy, root, KeyPressMask);

	/* main event loop */
  XEvent ev;
	XSync(dpy, False);
	while (running && !XNextEvent(dpy, &ev)) {
    if (ev.type == KeyPress) {
      printf("Key: %d\n", ev.xkey.keycode);

      running--;
    }
  }

	XCloseDisplay(dpy);
}

