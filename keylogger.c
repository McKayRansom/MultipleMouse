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

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;


    window = DefaultRootWindow(display);
    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabPointer(display,
                 window,
                 1,
                 PointerMotionMask | ButtonPressMask | ButtonReleaseMask ,
                 GrabModeAsync,
                 GrabModeAsync,
                 None,
                 None,
                 CurrentTime);

    while(1) {
        XNextEvent(display, &xevent);

        switch (xevent.type) {
            case MotionNotify:
                printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                break;
            case ButtonPress:
                printf("Button pressed  : %s\n", key_name[xevent.xbutton.button - 1]);
                // XWarpPointer(display, window, None, 0, 0, 0, 0, 100, 100);
                // XSetInputFocus(display, window, RevertToNone, CurrentTime);
                break;
            case ButtonRelease:
                printf("Button released : %s\n", key_name[xevent.xbutton.button - 1]);
                break;
        }
    }

    return 0;
}
