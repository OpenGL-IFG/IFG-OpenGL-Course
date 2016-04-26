#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

char WINDOW_NAME[] = "Maimunoba 1";
char ICON_NAME[] = "Icon";
Display *display; // display
int screen; // screen
Window main_window; // windows-fanjara
GC gc; // grafika (xazebi,wreebi, ferebi itogshi dezertirebia ra)
unsigned long foreground, background;
int x,y,old_mx=-1,old_my; // deklaracia, itogshi -1 imito ro pirvel daklikebaze xazi ar gaaketos, xo gamige?

void connectX()
{
   display = XOpenDisplay(NULL); // connect to server
   if (display == NULL) {fprintf(stderr, "Cannot connect to X\n");
                         exit(1);} // if not connected
   screen = DefaultScreen(display); // apply display to screen
   foreground = BlackPixel(display, screen); // wina feri
   background = WhitePixel(display, screen); // ukana backgraund feri
}

// standart openwindow function
Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char** argv)
{
    Window new_window; // axali fanjara
    XSizeHints size_hints;
    new_window = XCreateSimpleWindow(display, DefaultRootWindow(display),
                 x, y, width, height, border_width, foreground,
                 background);
   size_hints.x = x;
   size_hints.y = y;
   size_hints.width = width;
   size_hints.height = height;
   size_hints.flags = PPosition | PSize;
   XSetStandardProperties(display, new_window, WINDOW_NAME, ICON_NAME,
                          None, argv, argc, &size_hints);
   XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask |
                                      ExposureMask | PointerMotionMask));
   return (new_window);
}

GC getGC()
{
    GC gc;
    XGCValues gcValues;
    gc = XCreateGC(display, main_window, (unsigned long)0, &gcValues);
    XSetBackground(display, gc, background);
   XSetForeground(display, gc, foreground);
    return (gc);
}

void disconnectX()
{
   XCloseDisplay(display);
   exit(0);
}


 void draw()
{
   XDrawImageString(display, main_window, gc, y/2-strlen("Draw with zigzag lines"), 10, "Draw with zigzag lines",
                    strlen("Draw with zigzag lines"));
   XFlush(display);
}

void doButtonPressEvent(XButtonEvent *pEvent)
{
  int mx,my;
  if (old_mx == -1) // tu pirveli klikia dauyenos old mnishvnelobebs pirveli kilis mnishvneloobebi xolo
  {                 // mere mx,my ebi gadava old mnishvnelobebshi da daixateba uwyveti texilebi
      old_mx = pEvent->x; // konkretulad es buton iventidan igebs x da ys sadac mxda dacheris inventi
      old_my = pEvent->y;
  }
  else {
      int mx,my;
      mx = pEvent->x;
      my = pEvent->y;
      XDrawLine(display, main_window, gc, old_mx, old_my, mx, my); // ixateba xazi ori wertilis koordinatebit
      printf("Mouse clicked at %d,%d\n", mx,my); // logging here
      printf("Button pressed\n");
      old_mx = mx;
      old_my = my;
  }
}

void doKeyPressEvent(XKeyEvent *pEvent)
{
    int key_buffer_size = 10;
    char key_buffer[9];
    XComposeStatus compose_status;
    KeySym key_sym;
    XLookupString(pEvent, key_buffer, key_buffer_size,
                  &key_sym, &compose_status);
   if (key_buffer[0] == 'q') disconnectX(); // tu daachira q gamovides
   else printf("You pressed %c\n", key_buffer[0]); // logi
}


int main (int argc, char** argv){
  XEvent event;
  connectX();
  x=500;
  y=400;
  main_window = openWindow(10,20,x,y,5, argc, argv);
  gc = getGC();
  XMapWindow(display, main_window);
  draw();
  XFlush(display);
  while (True){
    XNextEvent(display, &event);
   switch(event.type){
    case Expose:
      printf("Expose event\n");
      draw();
      break;
    case ButtonPress:
      printf("Button Pressed\n");
      doButtonPressEvent(&event);
      break;
    case KeyPress:
     printf("Key pressed\n");
     doKeyPressEvent(&event);
     break;
   }
  }
}
