#include <stdio.h>
#include <stdlib.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

char WINDOW_NAME[] = "Maimunoba 1";
char ICON_NAME[] = "Icon";
Display *display; // display
int screen; // screen
Window main_window; // windows-fanjara
GC gc; // grafika (xazebi,wreebi, ferebi itogshi dezertirebia ra)
unsigned long foreground, background;

// deklaracia, itogshi -1 imito ro pirvel
// daklikebaze xazi ar gaaketos, xo gamige?
int x,y,old_mx=-1,old_my,mx,my,first_mx,first_my;
int c_clicked = 0;
int run_loop = 1;

typedef struct Point2Z {
    float k;
    float b;
} Point2;
typedef struct PointZ {
    int x;
    int y;
} Point;
typedef struct ScanLines {
    Point startP;
    Point endP;
} ScanLine;

int pointIndex=0, point2Index=0;
int pointsSize=1, points2Size=1;
Point2* functions = NULL;
Point2* new_functions = NULL;
Point* points = NULL;
Point* new_points = NULL;


Point2 calculatekb(int x1, int y1, int x2, int y2) {
    Point2 tmpP;
    int dx,dy;
    float k,b;
    dx = x1-x2;
    dy = y1-y2;
    k = (float)dy/(float)dx;
    b = (float)y1-k*(float)x1;
    tmpP.k=k;
    tmpP.b=b;
    return tmpP;
}

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
}

// standart openwindow function
Window openWindow(int x, int y, int width, int height,
                  int border_width, int argc, char** argv)
{
    Window new_window; // axali fanjara
    XSizeHints size_hints;
    new_window = XCreateSimpleWindow(display,
            DefaultRootWindow(display), x, y, width, height,
            border_width, foreground, background);
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
    XDrawImageString(display, main_window, gc, x/2-strlen("Draw with zigzag lines"), 10, "Draw with zigzag lines",
                     strlen("Draw with zigzag lines"));
    XFlush(display);
}

void doButtonPressEvent(XButtonEvent *pEvent)
{
    if (!c_clicked)
    {
        // tu pirveli klikia dauyenos old mnishvnelobebs
        // pirveli kilis mnishvneloobebi xolo
        Point tmpP;
        if (old_mx == -1)
        {
            // mere mx,my ebi gadava old mnishvnelobebshi
            // da daixateba uwyveti texilebi
            // konkretulad es buton iventidan igebs x
            // da ys sadac mxda dacheris inventi
            first_mx = old_mx = pEvent->x;
            first_my = old_my = pEvent->y;
            tmpP.x = first_mx;
            tmpP.y = first_my;
            points[pointIndex] = tmpP;
            pointsSize++;
            pointIndex++;
            new_points = (Point*) realloc(points, pointsSize*sizeof(Point));
            if (new_points!=NULL) {
                printf ("Reallocation failed!");
            }
            points = new_points;
        }
        else
        {
            mx = pEvent->x;
            my = pEvent->y;
            // ixateba xazi ori wertilis koordinatebit
            XDrawLine(display, main_window, gc, old_mx, old_my, mx, my);
            printf("Mouse clicked at %d,%d\n", mx,my); // logging here
            printf("Button pressed\n");
            functions[point2Index] = calculatekb(old_mx,old_my,mx,my);
            points2Size++;
            point2Index++;
            new_functions = (Point2*) realloc(functions, points2Size*sizeof(Point2));
            if (new_functions!=NULL) {
               printf ("Reallocation failed for functions!");
            }
            functions = new_functions;
            old_mx = mx;
            old_my = my;
            tmpP.x = old_mx;
            tmpP.y = old_my;
            points[pointIndex] = tmpP;
            pointsSize++;
            pointIndex++;
            new_points = (Point*) realloc(points, pointsSize*sizeof(Point));
            if (new_points!=NULL) {
                printf ("Reallocation failed!");
            }
            points = new_points;
        }
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
    if (key_buffer[0] == 'c')
    {
        c_clicked=1;
        XDrawLine(display, main_window, gc, first_mx, first_my, mx, my);
        functions[point2Index] = calculatekb(mx,my,first_mx,first_my);
        int i=0;
        while(i<pointsSize-1)
        {
            Point tmp2P;
            tmp2P=points[i];
            printf("X: %d, Y: %d\n", tmp2P.x, tmp2P.y);
            i++;
        }
        i=0;
        while(i<points2Size)
        {
            Point2 tmpP;
            tmpP = functions[i];
            printf("K: %f, B: %f\n", tmpP.k, tmpP.b);
            i++;
        }
    }
    else printf("You pressed %c\n", key_buffer[0]); // logi
}


int main (int argc, char** argv)
{
    XEvent event;
    connectX();
    x=DisplayWidth(display, screen);
    y=DisplayHeight(display, screen);
    main_window = openWindow(10,20,x,y,5, argc, argv);
    gc = getGC();
    XMapWindow(display, main_window);
    new_points = (Point*) realloc(points, pointsSize*sizeof(Point));
    if (new_points!=NULL) {
        printf ("Reallocation failed!");
    }
    points = new_points;
    new_functions = (Point2*) realloc(functions, points2Size*sizeof(Point2));
    if (new_functions!=NULL) {
        printf ("Reallocation failed for functions!");
    }
    functions = new_functions;
    draw();
    XFlush(display);
    while (run_loop)
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
        case KeyPress:
            printf("Key pressed\n");
            doKeyPressEvent(&event);
            break;
        }
    }
}
