//HackUSU 2018
//made by McKay Ransom, Glendyn King, and Alan

//See XLib manual: https://tronche.com/gui/x/xlib/

//must have xlib dev tools installed
//see:https://ubuntuforums.org/showthread.php?t=791474

#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput.h>
#include <X11/extensions/XI.h>
#include <X11/Xutil.h>

#include "mouse.h"

#define maxDevices 10
struct InputDevices {
  XDevice* mouseDevices[maxDevices];
  int num_mouseDevices;
  XDevice* keyboardDevices[maxDevices];
  int num_keyboardDevices;
};

//prints out all input devices
//see xinput command line utility code
//https://cgit.freedesktop.org/xorg/app/xinput/tree/src/list.c?id=5a8f3121c7f64dbb47fad99c027389e7d78453e4
struct InputDevices findInputDevices(Display *display) {
  XDeviceInfo *info;
  int num_devices;
  struct InputDevices devices;

  //get a list of all connected devices from os I guess
  info = XListInputDevices(display, &num_devices);
  //for each device
  for (int i = 0; i < num_devices; i++, info++) {
    //if it has a type (basically a safety check)
    if (info->type != None) {
      //get it's type (either KEYBOARD, TOUCHPAD, TOUCHSCREEN, or MOUSE)
      char *type = XGetAtomName(display, info->type);
      //if it is a mouse or touchpad
      if (strcmp(type, "MOUSE") || strcmp(type, "TOUCHPAD")) {
        //make sure we still have room for it
        if (devices.num_mouseDevices < maxDevices) {
          //get device and add device to array
          devices.mouseDevices[devices.num_mouseDevices++] =
                              XOpenDevice(display, info->id);
        }
      }
      //if it is a keyboard
      else if (strcmp(type, "KEYBOARD")) {
        if (devices.num_keyboardDevices < maxDevices) {
          //get device and add device to array
          devices.keyboardDevices[devices.num_keyboardDevices++] =
                              XOpenDevice(display, info->id);
        }
      }
    }
  }
}

//move the mouse to a new location (in absolute coords)
void moveMouse(Display* display, Window window, int x, int y) {
  XWarpPointer(display, //display
                window, //source window
                window, //destination window
                0,      //source x
                0,      //source y
                0,      //source width
                0,      //source height
                x,      //destination x
                y);     //destination y
}
//overload of the above
void moveMouseStruct(Display* display, Window window, struct mouse mouse) {
  moveMouse(display, window, mouse.x, mouse.y);
}

int main(int argc, char ** argv) {
  Display *display;
  Window window;

  //X11 init
  if( (display = XOpenDisplay(NULL)) == NULL )
      return -1;
  window = DefaultRootWindow(display);

  //devices init
  struct InputDevices allDevices;
  //find all possible devices
  allDevices = findInputDevices(display);
  //ask user to select devices they actually want
  // TODO: actually implement user device selection!
  // devices = userFilterInputDevices(devices);
  struct InputDevices devices;
  //wireless
  devices.mouseDevices[0] = XOpenDevice(display, 15);
  //touchPad
  devices.mouseDevices[1] = XOpenDevice(display, 13);
  devices.num_mouseDevices = 2;

  devices.keyboardDevices[0] = XOpenDevice(display, 12);
  devices.num_keyboardDevices = 1;

  struct mouse mouseStructs[10];
  //motion + press + release
  XEventClass mouseEvents[30];
  int mouseEventTypes[30];
  //setup event listening
  for (int i = 0; i <devices.num_mouseDevices; i++) {
    mouseStructs[i].x = 0; //TODO: change to center of screen!
    mouseStructs[i].y = 0;
    //Motion
    DeviceMotionNotify(devices.mouseDevices[i],
          mouseEventTypes[i * 3],
          mouseEvents[i * 3]);
    //press
    DeviceButtonPress(devices.mouseDevices[i],
          mouseEventTypes[i * 3 + 1],
          mouseEvents[i * 3 + 1]);

    DeviceButtonRelease(devices.mouseDevices[i],
          mouseEventTypes[i * 3 + 2],
          mouseEvents[i * 3 + 2]);

    XGrabDevice(display,
          devices.mouseDevices[i], //display
          window,   //grab window
          1,        //owner events
          3,        //event_count
          mouseEvents + (i * 3), //event_list
          GrabModeAsync,   //this_device_mode
          GrabModeAsync,  //other_device_mode
          CurrentTime);   //time
  }
  // XSelectExtensionEvent(display, window, mouseEvents + 1, devices.num_mouseDevices - 1);

  // XGrabButton(display,
  //              AnyButton,
  //              AnyModifier,
  //              window,
  //              0,
  //              ButtonPressMask,
  //              GrabModeSync,
  //              GrabModeAsync,
  //              None,
  //              None);
   // XSelectInput(display, window, ButtonPressMask | PointerMotionMask);

   // XEventClass keyboardEvents[10];
   // TODO: setup keyboard events!!!

   printf("--------- Init Finished ----------\n");

   printf("Listening for events...\n");


   XEvent event;
   struct mouse mainMouse;
   mainMouse.x = 0;
   mainMouse.y = 0;
   int currentMouse = 0;
   int disregardNextMove = 0;
   moveMouse(display, window, 0, 0);
   while(1) {
     XNextEvent(display, &event);
     //all mouse event types are the same so we just check 1

     //Device motion event
     if (event.type == mouseEventTypes[0]) {
     //   //use this for manual moves
     //   if (disregardX || disregardY) {
     //     if (disregardX == ((XDeviceMotionEvent*)&event)->x_root
     //                && disregardY == ((XDeviceMotionEvent*)&event)->y_root) {
     //       disregardX = 0;
     //       disregardY = 0;
     //       printf("disregarding move: %d, %d\n", ((XDeviceMotionEvent*)&event)->x_root, ((XDeviceMotionEvent*)&event)-> y_root);
     //     }
     //     // continue;
     //   }
        if (disregardNextMove) {
          disregardNextMove--;
          continue;
        }
       //check every mouse device
       for (int i = 0; i < devices.num_mouseDevices; i++) {
         if (((XDeviceMotionEvent*)&event)->deviceid == devices.mouseDevices[i]->device_id) {
           //update mouse position
           if (currentMouse != i) {
             printf("Mouse Switch!\n");
             currentMouse = i;
             //force the mouse to move
             moveMouseStruct(display, window, mouseStructs[i]);
             //throw out event caused by moving the mouse
             disregardNextMove = 2;
             //todo: copy struct
             mainMouse = mouseStructs[i];
           } else {
             mouse_update(&mouseStructs[i], &mainMouse, *(XDeviceMotionEvent*)&event);
           }
         }
       }
     //Button Press event
     } else if (event.type == mouseEventTypes[1]) {
       printf("button pressed!\n");

       //pass event through to other programs!
       XAllowEvents(display, ReplayPointer, event.xbutton.time);
       XFlush(display);
     //Button Release Event
     } else if (event.type == mouseEventTypes[2]) {
       printf("button released!\n");

       //pass event through to other programs!
       XAllowEvents(display, ReplayPointer, event.xbutton.time);
       XFlush(display);

     } else {
       printf("??? event\n");
     }
   }


}
