//credit to Craig S. Anderson on Stack Overflow
//https://stackoverflow.com/questions/14553435/how-to-listen-for-mouse-events-in-linux
//compile with:
//gcc mouse.c -o mouse -lX11

//See XLib manual: https://tronche.com/gui/x/xlib/

//must have xlib dev tools installed
//see: https://ubuntuforums.org/showthread.php?t=791474
//probably something like:
//sudo apt-get install libx11-dev

#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XI.h>
#include <X11/Xutil.h>


int main(int argc, char **argv)
{
    Display *display;
    XDeviceMotionEvent event;
    Window window;

    if( (display = XOpenDisplay(NULL)) == NULL )
        return -1;


    window = DefaultRootWindow(display);

    //see xinput command line utility code
    //https://cgit.freedesktop.org/xorg/app/xinput/tree/src/list.c?id=5a8f3121c7f64dbb47fad99c027389e7d78453e4

    XDeviceInfo *info;
    int num_devices;

    info = XListInputDevices(display, &num_devices);
    for (int i = 0; i < num_devices; i++) {
      printf("\"%s\"\tid=%ld\t[", info->name, info->id);

      switch (info->use) {
      case IsXPointer:
        printf("XPointer");
        break;
      case IsXKeyboard:
        printf("XKeyboard");
        break;
      case IsXExtensionDevice:
        printf("XExtensionDevice");
        break;
      case IsXExtensionKeyboard:
        printf("XExtensionKeyboard");
        break;
      case IsXExtensionPointer:
        printf("XExtensionPointer");
        break;
      default:
        printf("Unknown class");
        break;
      }
      printf("]\n");
      // if (info->type != None) {
        // char *type = XGetAtomName(display, info->type);

        // printf("Type is %s\n", type);
      // } else {
      //   printf("\n");
      // }
      info++;
    }
    int wirelessID = 15;
    int touchID = 13;
    XDevice* wirelessMouse= XOpenDevice(display, wirelessID);
    XDevice* touchPad= XOpenDevice(display, touchID);

    XEventClass event_class;
    int event_type;
    DeviceMotionNotify(wirelessMouse, event_type, event_class);

    XEventClass touch_event_class;
    int touch_event_type;
    DeviceMotionNotify(touchPad, touch_event_type, touch_event_class);
    // printf("\n")

    XEventClass mice[] = {event_class, touch_event_class};

    XSelectExtensionEvent(display, window, mice, 2);

    XAllowEvents(display, AsyncBoth, CurrentTime);

    XGrabPointer(display,
                 window,
                 1,
                 PointerMotionMask,
                 GrabModeAsync,
                 GrabModeAsync,
                 None,
                 None,
                 CurrentTime);

    printf("listening for events\n");
    int pointerX = 0;
    int pointerY = 0;
    int mouse1X = 0;
    int mouse1Y = 0;
    int mouse2X = 0;
    int mouse2Y = 0;
    while(1) {
        XNextEvent(display, &event);

        if (event.deviceid == wirelessID) {
          int x = event.x_root;
          int y = event.y_root;
          int dx = x - pointerX;
          int dy = y - pointerY;
          mouse1X += dx;
          mouse1Y += dy;
          pointerX = x;
          pointerY = y;
          printf("Wireless Mouse move      : [%d, %d, %d, %d]\n", dx, dy, mouse1X, mouse1Y );
        } else if (event.deviceid == touchID){
          int x = event.x_root;
          int y = event.y_root;
          int dx = x - pointerX;
          int dy = y - pointerY;
          mouse2X += dx;
          mouse2Y += dy;
          pointerX = x;
          pointerY = y;
          printf("Touch Mouse move      : [%d, %d, %d, %d]\n", dx, dy, mouse2X, mouse2Y );
        }
        /*else if (event.type == MotionNotify) {
          int x = event.xmotion.x_root;
          int y = event.xmotion.y_root;
          //pointerX = x;
          //pointerY = y;
          printf("Main Mouse move      : [%d, %d]\n", x, y);
        }*/
            // printf("received vent");
            // case event_type:
                // break;
            // case ButtonPress:
                // printf("Button pressed  : %s\n", key_name[xevent.xbutton.button - 1]);
                // XWarpPointer(display, window, None, 0, 0, 0, 0, 100, 100);
                // XSetInputFocus(display, window, RevertToNone, CurrentTime);
                // break;
            // case ButtonRelease:
                // printf("Button released : %s\n", key_name[xevent.xbutton.button - 1]);
                // break;
        // }
     }

    return 0;
}
