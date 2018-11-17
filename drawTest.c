/*
   Simple Xlib application drawing a box in a window.
   To Compile: gcc -O2 -Wall -o test test.c -L /usr/X11R6/lib -lX11 -lm
 */

 #include<X11/Xlib.h>
 #include<stdio.h>
 #include<stdlib.h> // prevents error for exit on line 18 when compiling with gcc
 int main() {
   Display *d;
   int s;
   Window w;
   XEvent e;

                        /* open connection with the server */
   d=XOpenDisplay(NULL);
   if(d==NULL) {
     printf("Cannot open display\n");
     exit(1);
   }
   s=DefaultScreen(d);

                        /* create window */
   w=XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, 100, 100, 1,
                         BlackPixel(d, s), None);
   // w = DefaultRootWindow(d);

   //XSetBackground(d, gc, BlackPixel(display, screen_num));
   // Process Window Close Event through event handler so XNextEvent does Not fail
   Atom delWindow = XInternAtom( d, "WM_DELETE_WINDOW", 0 );
   XSetWMProtocols(d , w, &delWindow, 1);

                        /* select kind of events we are interested in */
   XSelectInput(d, w, ExposureMask | KeyPressMask);

                        /* map (show) the window */
   XMapWindow(d, w);
   printf("finished init\n");
                        /* event loop */
   while(1) {
     XNextEvent(d, &e);
                        /* draw or redraw the window */
     if(e.type==Expose) {
       printf("drawing!\n");
       XFillRectangle(d, w, DefaultGC(d, s), 20, 20, 10, 10);
     }
                        /* exit on key press */
     if(e.type==KeyPress)
       break;

     // Handle Windows Close Event
     if(e.type==ClientMessage)
        break;
   }

                        /* destroy our window */
   XDestroyWindow(d, w);

                        /* close connection to server */
   XCloseDisplay(d);

   return 0;
 }
 /*#include <stdio.h>
#include <X11/Xlib.h>

int main() {
  // Perform initialization routines.
  // Connect to the X server.
  Display* display;
  // if (DISPLAY) {
    // printf("found global display\n");
    // if( (display = XOpenDisplay(DISPLAY)) == NULL )
        // return -1;
  // } else {
    if( (display = XOpenDisplay(NULL)) == NULL )
      return -1;
  // }

  // Perform X-related initialization.
  /* check the number of the default screen for our X server. */
//   int screen_num = DefaultScreen(display);
//
//   /* find the width of the default screen of our X server, in pixels. */
//   int screen_width = DisplayWidth(display, screen_num);
//
//   /* find the height of the default screen of our X server, in pixels. */
//   int screen_height = DisplayHeight(display, screen_num);
//
//   Window win;
//
//   int win_width;
//   int win_height;
//
//   int win_x;
//   int win_y;
//
//   win_width = DisplayWidth(display, screen_num)/3;
//   win_height = DisplayHeight(display, screen_num)/3;
//   win_x = win_y = 0;
//   int win_border_width = 3;
// //create our window
//   win = XCreateSimpleWindow(display,
//     RootWindow(display, screen_num),
//     win_x, win_y,
//     win_width, win_height,
//     win_border_width, BlackPixel(display, screen_num),
//     WhitePixel(display, screen_num));
//   //actually make it so window will be drawn
//   XMapWindow(display, win);
//
//   GC gc;
//
//   XGCValues values;
//
//   values.cap_style = CapButt;
//   values.join_style = JoinBevel;
//
//   unsigned long valuemask = GCCapStyle | GCJoinStyle;
//
//   gc = XCreateGC(display, win, valuemask, &values);
//   if (gc < 0) {
//     fprintf(stderr, "XCreateGC: \n");
//   }
//
//   XSetForeground(display, gc, WhitePixel(display, screen_num));
//

//
//   XSetFillStyle(display, gc, FillSolid);
//   /* change the line drawing attributes of this GC to the given values. */
//   /* the parameters are: Display structure, GC, line width (in pixels), */
//   /* line drawing  style, cap (line's end) drawing style, and lines     */
//   /* join style.                                                        */
//   XSetLineAttributes(display, gc, 2, LineSolid, CapRound, JoinRound);
//   /* draw a rectangle whose top-left corner is at '120,150', its width is */
//   /* 50 pixels, and height is 60 pixels.                                  */
//   XDrawRectangle(display, win, gc, 120, 150, 50, 60);
//
//   // While not finished:
//   while(1) {
//     ;
//   }
//   // Receive the next event from the X server.
//   // handle the event, possibly sending various drawing requests to the X server.
//   // If the event was a quit message, exit the loop.
//   // Close down the connection to the X server.
//   // Perform cleanup operations.
// }
// */
