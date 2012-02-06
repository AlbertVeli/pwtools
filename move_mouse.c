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

int main(void)
{
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
