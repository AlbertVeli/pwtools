/* This program is free software. It comes without any warranty, to
 * the extent permitted by applicable law. You can redistribute it
 * and/or modify it under the terms of the Do What The Fuck You Want
 * To Public License, Version 2, as published by Sam Hocevar. See
 * http://sam.zoy.org/wtfpl/COPYING for more details.
 *
 *
 * Randomly move mouse to prevent screen to go blank.
 *
 * Setting Orange, the 35th day of Chaos in the YOLD 3178
 *
 * Albert Veli
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/Xmu/WinUtil.h>

Display *display = NULL;

/* /\* Click mouse at current position. */
/*  * 1 = left button */
/*  * 2 = middle button */
/*  * 3 = right button */
/*  *\/ */
/* int mouse_click(int button) */
/* { */
/*    XEvent event; */

/*    memset(&event, 0x00, sizeof(event)); */

/*    event.type = ButtonPress; */
/*    event.xbutton.button = button; */
/*    event.xbutton.same_screen = True; */

/*    XQueryPointer(display, RootWindow(display, DefaultScreen(display)), */
/*                  &event.xbutton.root, &event.xbutton.window, */
/*                  &event.xbutton.x_root, &event.xbutton.y_root, */
/*                  &event.xbutton.x, &event.xbutton.y, &event.xbutton.state); */

/*    event.xbutton.subwindow = event.xbutton.window; */

/*    while(event.xbutton.subwindow) { */
/*       event.xbutton.window = event.xbutton.subwindow; */

/*       XQueryPointer(display, event.xbutton.window, */
/*                     &event.xbutton.root, &event.xbutton.subwindow, */
/*                     &event.xbutton.x_root, &event.xbutton.y_root, */
/*                     &event.xbutton.x, &event.xbutton.y, &event.xbutton.state); */
/*    } */

/*    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) { */
/*       fprintf(stderr, "%s: Error sending button press event\n", __FUNCTION__); */
/*    } */

/*    XFlush(display); */

/*    usleep(100000); */

/*    event.type = ButtonRelease; */
/*    event.xbutton.state = 0x100; */

/*    if(XSendEvent(display, PointerWindow, True, 0xfff, &event) == 0) { */
/*       fprintf(stderr, "%s: Error sending button release event\n", __FUNCTION__); */
/*    } */

/*    XFlush(display); */

/*    return 0; */
/* } */


/* Get mouse coordinates relative root window */
int mouse_get_coords(int *x, int *y)
{
   XEvent event;

   memset(&event, 0x00, sizeof(event));

   event.type = ButtonPress;
   event.xbutton.same_screen = True;

   XQueryPointer(display, RootWindow(display, DefaultScreen(display)), &event.xbutton.root, &event.xbutton.window, &event.xbutton.x_root, &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y, &event.xbutton.state);

   *x = event.xbutton.x;
   *y = event.xbutton.y;

   return 0;
}


/* Move mouse to coordinates (wanted_x, wanted_y) */
void mouse_move_abs(int wanted_x, int wanted_y)
{
   int tmp;

   XWarpPointer(display, None, RootWindow(display, DefaultScreen(display)), 0, 0, 0, 0, wanted_x, wanted_y);

   /* Not moved until XQueryPointer is called for some reason */
   mouse_get_coords(&tmp, &tmp);
}


/* /\* Select coordinates *\/ */
/* int mouse_mark_spot(int *x, int *y) */
/* { */
/*    Cursor cursor; */
/*    XEvent event; */
/*    Window root = RootWindow(display, DefaultScreen(display)); */
/*    int buttons = 0; */

/*    *x = -1; */

/*    /\* Make the target cursor *\/ */
/*    cursor = XCreateFontCursor(display, XC_crosshair); */

/*    /\* Grab the pointer using target cursor, letting it room all over *\/ */
/*    if (XGrabPointer(display, root, False, */
/*                     ButtonPressMask|ButtonReleaseMask, GrabModeSync, */
/*                     GrabModeAsync, root, cursor, CurrentTime) != GrabSuccess) { */
/*       fprintf(stderr, "%s: couldn't grab cursor\n", __FUNCTION__); */
/*       return 1; */
/*    } */

/*    /\* Let the user select a window... *\/ */
/*    while ((*x == -1) || (buttons != 0)) { */
/*       /\* allow one more event *\/ */
/*       XAllowEvents(display, SyncPointer, CurrentTime); */
/*       XWindowEvent(display, root, ButtonPressMask | ButtonReleaseMask, &event); */
/*       switch (event.type) { */
/*       case ButtonPress: */
/*          if (*x == -1) { */
/*             *x = event.xbutton.x; */
/*             *y = event.xbutton.y; */
/*          } */
/*          buttons++; */
/*          break; */
/*       case ButtonRelease: */
/*          if (buttons > 0) /\* there may have been some down before we started *\/ */
/*             buttons--; */
/*          break; */
/*       } */
/*    } */

/*   XUngrabPointer(display, CurrentTime); */

/*   return 0; */
/* } */


/* /\* Select windows, from xwd, ftp://ftp.x.org/pub/current/src/app/ *\/ */
/* Window mouse_select_window(void) */
/* { */
/*    int status; */
/*    Cursor cursor; */
/*    XEvent event; */
/*    Window target_win = None, root = RootWindow(display, DefaultScreen(display)); */
/*    int buttons = 0; */

/*    /\* Make the target cursor *\/ */
/*    cursor = XCreateFontCursor(display, XC_crosshair); */

/*    /\* Grab the pointer using target cursor, letting it room all over *\/ */
/*    status = XGrabPointer(display, root, False, */
/*                          ButtonPressMask|ButtonReleaseMask, GrabModeSync, */
/*                          GrabModeAsync, root, cursor, CurrentTime); */
/*    if (status != GrabSuccess) { */
/*       fprintf(stderr, "%s: Error grabbing mouse\n", __FUNCTION__); */
/*       return 0; */
/*    } */

/*    /\* Let the user select a window... *\/ */
/*    while ((target_win == None) || (buttons != 0)) { */
/*       /\* allow one more event *\/ */
/*       XAllowEvents(display, SyncPointer, CurrentTime); */
/*       XWindowEvent(display, root, ButtonPressMask|ButtonReleaseMask, &event); */
/*       switch (event.type) { */
/*       case ButtonPress: */
/*          if (target_win == None) { */
/*             target_win = event.xbutton.subwindow; /\* window selected *\/ */
/*             if (target_win == None) target_win = root; */
/*          } */
/*          buttons++; */
/*          break; */
/*       case ButtonRelease: */
/*          if (buttons > 0) /\* there may have been some down before we started *\/ */
/*             buttons--; */
/*          break; */
/*       } */
/*    } */

/*    if (target_win != None) { */
/*       int dummyi; */
/*       unsigned int dummy; */

/*       if (XGetGeometry (display, target_win, &root, &dummyi, &dummyi, */
/*                         &dummy, &dummy, &dummy, &dummy) && */
/*           target_win != root) { */
/*          target_win = XmuClientWindow(display, target_win); */
/*       } */
/*    } */

/*    XUngrabPointer(display, CurrentTime);      /\* Done with pointer *\/ */

/*    return(target_win); */
/* } */


int main(void)
{
   /* int x, y; */
   /* int i; */

   display = XOpenDisplay(NULL);
   if(!display) {
      fprintf(stderr, "Couldn't open main display\n");
      return 1;
   }

   while (1) {
      sleep(5);
      mouse_move_abs(100 + (rand() % 100), 100 + (rand() % 100));
   }

   XCloseDisplay(display);

   return 0;
}
