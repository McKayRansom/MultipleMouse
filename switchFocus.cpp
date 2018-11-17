//credit to Craig S. Anderson on Stack Overflow
//https://stackoverflow.com/questions/14553435/how-to-listen-for-mouse-events-in-linux
//compile with:
//gcc mouse.c -o mouse -lX11

//See XLib manual: https://tronche.com/gui/x/xlib/

//must have xlib dev tools installed
//see:https://ubuntuforums.org/showthread.php?t=791474
//probably something like:
//sudo apt-get install libx11-dev

#include <stdio.h>
#include <X11/Xlib.h>

char *key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",  // :D
    "fivth"    // :|
};

int main(int argc, char **argv)
{
    Display *display;
    XEvent xevent;
    Window window;
    Window initialWindow;
    Window targetWindow;

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;


    window = DefaultRootWindow(display);

    Window root_return;
    Window parent_return;
    Window* children_return;
    unsigned int nchildren_return;
    XWindowAttributes attributes;
    XQueryTree(display, window, &root_return, &parent_return, &children_return, &nchildren_return);
    XGetWindowAttributes(display, window, &attributes);
    printf("current window is: %d x %d at %d, %d\n", attributes.width, attributes.height, attributes.x, attributes.y);
    XGetWindowAttributes(display, root_return, &attributes);
    printf("root window is: %d x %d at %d, %d\n", attributes.width, attributes.height, attributes.x, attributes.y);
    for (unsigned int i = 0; i < nchildren_return; i++) {
      XGetWindowAttributes(display, children_return[i], &attributes);
      printf("child window %d is: %d x %d at %d, %d\n", i, attributes.width, attributes.height, attributes.x, attributes.y);
    }

    XFree(children_return);

    int revertTo;

    XGrabButton(display,
                 AnyButton,
                 AnyModifier,
                 window,
                 0,
                 ButtonPressMask,
                 GrabModeSync,
                 GrabModeAsync,
                 None,
                 None);

    XSelectInput(display, window, ButtonPressMask | FocusChangeMask);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGetInputFocus(display, &initialWindow, &revertTo);
    int state = 0;
    const int state_waitForWindowSelect = 0;
    const int state_active = 1;
    while(1) {
        // XPeekEvent(display, &xevent);
        // if (xevent.type == ButtonPress) {
          // p
        // }
        XNextEvent(display, &xevent);

        switch (xevent.type) {
            case MotionNotify:
                printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                break;
            case FocusIn:
              printf("focus change in\n");
              break;
            case FocusOut:
              printf("focus change out\n");
              break;
            case ButtonRelease:
                // printf("Button released : %s\n", key_name[xevent.xbutton.button - 1]);
                // break;
            case ButtonPress:
                // printf("Button pressed  : %s\n", key_name[xevent.xbutton.button - 1]);
                // XWarpPointer(display, window, None, 0, 0, 0, 0, -100, 0);
                if (state == state_waitForWindowSelect) {
                  XGetInputFocus(display, &targetWindow, &revertTo);
                  if (targetWindow != initialWindow) {
                    printf("window select!\n");
                    state = state_active;
                  } else {
                    printf("waiting for window selection\n");
                  }
                } else { //state equals state_active
                  printf("window redirected\n");
                  XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
                }
                XAllowEvents(display, ReplayPointer, xevent.xbutton.time);
                XFlush(display);
                // XAllowEvents(display, SyncPointer, CurrentTime);
                // XSendEvent(display, PointerWindow, 0, ButtonPressMask, &xevent);
                // continue;
                break;

        }
    }

    return 0;
}
