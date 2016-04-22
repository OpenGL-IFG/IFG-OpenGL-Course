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
int x,y; // deklaracia, itogshi -1 imito ro pirvel daklikebaze xazi ar gaaketos, xo gamige?
int radiusi=1,tst=0;

void connectX()
{
    display = XOpenDisplay(NULL); // connect to server
    if (display == NULL)
    {
        fprintf(stderr, "Cannot connect to X\n");
        exit(1);
    } // if not connected
    screen = DefaultScreen(display); // apply display to screen
    foreground = BlackPixel(display, screen); // wina feri
    background = WhitePixel(display, screen); // ukana backgraund feri
    printf("forground: %d, background: %d",foreground,background);
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
    XSelectInput(display, new_window, (ButtonPressMask | KeyPressMask | ButtonReleaseMask |
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
    XDrawImageString(display, main_window, gc, y/2-strlen("Draw Like A Boss"), 10, "Draw Like A Boss",
                     strlen("Draw Like A Boss"));
    XFlush(display);
}

void doButtonPressEvent(XButtonEvent *pEvent)
{
    int mx,my;
    tst=1;
    mx = pEvent->x;
    my = pEvent->y;
    mx -= radiusi;
    my -= radiusi;
    XDrawArc(display, main_window, gc, mx, my, 2*radiusi, 2*radiusi, 0, 360*64);
    XFillArc(display, main_window, gc, mx, my, 2*radiusi, 2*radiusi, 0, 360*64);
    printf("Mouse clicked at %d,%d\n", mx,my); // logging here
    printf("Button pressed\n");
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
    else if (key_buffer[0] == '>')
    {
        if ( radiusi<50 )
        {
            radiusi += 1;
            printf("Radius raised by 1. Radius: %d", radiusi);
        }
    }
    else if (key_buffer[0] == '<')
    {
        if ( radiusi>1 )
        {
            radiusi -= 1;
            printf("Radius decreased by 1. Radius: %d", radiusi);
        }
    }
    else printf("You pressed %c\n", key_buffer[0]); // logi
}

void doMotionNotifyEvent(XMotionEvent *pEvent)
{
    int xd,yd;
    char hitLoc[20];
    xd = pEvent->x;
    yd = pEvent->y;
    if ( tst )
    {
        XDrawArc(display, main_window, gc, xd, yd, 2*radiusi, 2*radiusi, 0, 360*64);
        XFillArc(display, main_window, gc, xd, yd, 2*radiusi, 2*radiusi, 0, 360*64);
    }

}

int main (int argc, char** argv)
{
    XEvent event;
    connectX();
    x=500;
    y=400;
    main_window = openWindow(10,20,x,y,5, argc, argv);
    gc = getGC();
    XMapWindow(display, main_window);
    draw();
    XFlush(display);
    while (True)
    {
        XNextEvent(display, &event);
        switch(event.type)
        {
        case Expose:
            printf("Expose event\n");
            draw();
            break;
        case ButtonPress:
            printf("Button Pressed\n");
            doButtonPressEvent(&event);
            break;
        case ButtonRelease:
            printf("Button released\n");
            tst=0;
            break;
        case KeyPress:
            printf("Key pressed\n");
            doKeyPressEvent(&event);
            break;
        case MotionNotify:
            doMotionNotifyEvent(&event);
            break;
        }
    }
}
