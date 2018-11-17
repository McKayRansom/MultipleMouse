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
#include <time.h>
#include <sys/time.h>
#include <signal.h>
//#include <cmath>

int sigaction(int signum, const struct sigaction *act, struct sigaction *oLdact);


const char *key_name[] = {
    "first",
    "second (or middle)",
    "third",
    "fourth",  // :D
    "fivth"    // :|
};

int abz(int val)
{
	return -val*(val<0)+val*(val>0);
}

int main(int argc, char **argv)
{
    int cur_x1 = 0;
    int cur_y1 = 0;
    int cur_x2 = 0;
    int cur_y2 = 0;
    int dx = 0;
    int dy = 0;
    Display *display;
    XEvent xevent;
    Window window;
    Window window_returned = 0;
    int init_x = 0;
    int init_y = 0;
    int wind_x = 0;
    int wind_y = 0;
    unsigned int mask_return = 0;
    

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

//   XGrabKeyboard(display,
//                 window,
//                 1,
//                 GrabModeAsync,
//                 GrabModeAsync,
//                 CurrentTime);
//
//
	XQueryPointer(display, 
		window, 
		&window_returned,
                &window_returned, 
                &init_x, 
                &init_y, 
                &wind_x, 
                &wind_y,
                &mask_return);

	printf("starting x and y: %d, %d\n" , init_x, init_y);
	int back = 0;
	int looping = 1;

cur_x1 = init_x;
cur_y1 = init_y;
    while(looping)
    {

	while(XPending(display) > 0 )
	{
printf("loop\n");
		XNextEvent(display, &xevent);
        	switch (xevent.type)
		{
            	case MotionNotify:	printf("Mouse move      : [%d, %d]\n", xevent.xmotion.x_root, xevent.xmotion.y_root);
                	cur_x1 = xevent.xmotion.x_root;		
                	cur_y1 = xevent.xmotion.y_root;
                 	break;
            	case ButtonPress:
                	printf("Button pressed  : %s\n", key_name[xevent.xbutton.button - 1]);
                	XWarpPointer(display, window, None, 0, 0, 0, 0, 100, 100);
                	XSetInputFocus(display, window, RevertToNone, CurrentTime);
                	looping = 0;
                	break;
            	case ButtonRelease:
                	printf("Button released : %s\n", key_name[xevent.xbutton.button - 1]);
                	break;
        	}
        	
     }
//printf("cur: %d, init: %d\n",cur_x1, init_x);

	XWarpPointer(display, window, None, 0, 0, 0, 0, init_x-cur_x1, init_y-cur_y1);
}
// int milliseconds = 100;  struct timespec ts;  ts.tv_sec = milliseconds/1000;  ts.tv_nsec = 0; 
//(milliseconds % 1000) * 1000000;  nanosleep(&ts, NULL); // printf("anything\n");
	for(int i = 1; i < 100; i++);

//	if(back)
//        	XWarpPointer(display, window, None, 0, 0, 0, 0, 100, 0);
//        else
//        	XWarpPointer(display, window, None, 0, 0, 0, 0, -100, 0);
//
	back = !back;
//	XRefresh();
//	}

    return 0;
}

int sigaction(int signum, const struct sigaction *act, struct sigaction *oLdact)
{

}
