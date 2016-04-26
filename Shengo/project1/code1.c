#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>

char WINDOW_NAME[] = "Project 1";
char ICON_NAME[] = "Icon";

Display *display; // display
int screen; // screen
Window main_window; // windows(fanjara)
GC gc; // graphic elements

unsigned long foreground, background;

int old_mx=-1,old_my; // deklaracia, itogshi -1 imito ro pirvel daklikebaze xazi ar gaaketos, xo gamige?
int width=700, height=700;

int mainArray[700][700] = { {0 } };

struct Point{

        int firstclick;
	int x0, y0;		/*start*/	
	int x_prev, y_prev;	/*previous*/
	int x_curr, y_curr;	/*current*/

	int polygon;		/* Polygon is created? */


}P;

void init(){

P.x_prev = 1;
P.firstclick =1;
P.polygon = 0;

}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////                        ////////////////////////////////////
//////////////////////    Casual Functions    /////////////////////////////////////
/////////////////////                        //////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////



void connectX()
{
   display = XOpenDisplay(NULL); // connect to server
   if (display == NULL) {fprintf(stderr, "Cannot connect to X server \n");
                         exit(1);} // if not connected
   screen = DefaultScreen(display); // apply display to screen
   foreground = BlackPixel(display, screen); 
   background = WhitePixel(display, screen); 
}

//open window 
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


 void draw() /*some info on screen*/
{
	XDrawImageString(display, main_window, gc, 10, 10, "Click points to draw polygon",
                    strlen("Click points to draw polygon"));

	XDrawImageString(display, main_window, gc, 10, 30, "C - create polygon, Q - quit",
                    strlen("C - create polygon, Q - quit"));
 
	XFlush(display);


}



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////


void doButtonPressEvent(XButtonEvent *pEvent)
{
if(P.polygon != 1)
{
	if(P.firstclick == 1)
	{
	P.firstclick = 0; 
	P.x0 = pEvent->x;
	P.y0 = pEvent->y;
	P.x_prev = P.x0 ;
	P.y_prev = P.y0 ;
	
	}
	else{
	
	P.x_curr = pEvent->x;
	P.y_curr = pEvent->y;
	
	XDrawLine(display, main_window, gc, P.x_prev, P.y_prev, P.x_curr, P.y_curr); // draw line 
      	printf("Mouse clicked at %d,%d\n", P.x_curr, P.y_curr);
	printf("Polygon Created");



	calcLinePoints(P.x_prev, P.y_prev, P.x_curr, P.y_curr);


	P.x_prev = P.x_curr ;
	P.y_prev = P.y_curr ;

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


	if (key_buffer[0] == 'q') disconnectX(); // q - quit
	if (key_buffer[0] == 'c') 
	{
		P.polygon=1;
		XDrawLine(display, main_window, gc, P.x_curr, P.y_curr, P.x0, P.y0);// create pylygon	

		XDrawImageString(display, main_window, gc, 10, 45, "Polygon Created",
                    strlen("Polygon Created"));

		XDrawImageString(display, main_window, gc, 10, 60, "Polygon filling coming soon. . .",
                    strlen("Polygon filling coming soon. . ."));
 
		//XFlush(display);

	}

	else printf("You pressed %c\n", key_buffer[0]); // log pushed keys
}


void calcLinePoints(int x1, int y1, int x2, int y2)
{

	x1=(float)-x1; x2=(float)-x2; y1=(float)-y1; y2=(float)-y1;
	
	float x, y;
	int ind_x, ind_y;

	//int a = (x2-x1)/(y2-y1);
	//int b = y1 - x1 * a;
	printf("0000000000000000 \n");
	//printf("Points %d :: %d \n", x1, x2);



	float a = (y2-y1)/(x2-x1);
	float b = y1 - (x1 * a);

        printf("Points %f :: %f \n", a, b);
	printf("Shesadareblebi: %d :: %d \n", x1, x2);

	if(x1 > x2)
	{
		x=x2;
		while(x < x1)
			{
			y = a * x + b;
			//mainArray[x][y] = 1;
			//printf("????????");
			printf("Line Points %d::%d\n", (int)x, (int)y);
			
			/*ind_x = (int)x; ind_y = (int)y; 
			mainArray[ind_x][ind_y] = 1;
			modi aq davisvenot */ 

			x+=1;
			}
	}
	else
	{
		x=x1;
		while(x < x2)
			{
			y = a * x + b;
			//mainArray[x][y] = 1;
			//printf("???????");
			printf("Line Points %d::%d\n", (int)x, (int)y);
			
			/*ind_x = (int)x; ind_y = (int)y; 
			mainArray[ind_x][ind_y] = 1;
			modi aq davisvenot */ 

			x+=1;
			}
	}

}




int main (int argc, char** argv){
  
	init();

	XEvent event;
	connectX();
	
	main_window = openWindow(10,20,width,height,5, argc, argv);
	gc = getGC();
	XMapWindow(display, main_window); /*maps the window and all of its subwindows*/


  
	while (True){ /*while clicking*/
		
		XNextEvent(display, &event);
		draw();

		switch(event.type){
		
		case Expose: //es raari ver movwvi//
     			printf("Expose event\n");
      			//draw();  /rato?
      			break;

		case ButtonPress:
			//printf("Button Pressed123123\n");
			doButtonPressEvent(&event);
			break;
		case KeyPress:
			printf("Key pressed\n");
			doKeyPressEvent(&event);
			break;
   				}
  		}
}
