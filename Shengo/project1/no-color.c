#include <stdio.h>
#include <X11/Xutil.h>
#include <X11/Xlib.h>
#include "math.h"

char WINDOW_NAME[] = "Project 1";
char ICON_NAME[] = "Icon";

Display *display; // display
int screen; // screen
Window main_window; // windows(fanjara)
GC gc; // graphic elements

unsigned long foreground, background;

int width=700, height=700;

int mainArray[700][700] = { {0} };
int crossXArray[700] = { {0} };

struct Point{

        int firstclick;

	int x0, y0;		/*start*/	
	int x_prev, y_prev;	/*previous*/
	int x_curr, y_curr;	/*current*/

	int polygon;		/* Polygon is created? */

	int y_min, y_max;


}P;

void init(){

P.x_prev = 1;
P.firstclick =1;
P.polygon = 0;

P.y_min = 0;
P.y_max = 700;

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


void calcLinePoints(int x1, int y1, int x2, int y2);
void fillPolygon();


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
	
	
	printf("Gadasacemebi: x1: %d, x2: %d, y1: %d, y2: %d \n", P.x_prev, P.x_curr, P.y_prev, P.y_curr);
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
		calcLinePoints(P.x_curr, P.y_curr, P.x0, P.y0);		

		XDrawImageString(display, main_window, gc, 10, 45, "Polygon Created",
                    strlen("Polygon Created"));
		printf("Polygon created \n ");

		XDrawImageString(display, main_window, gc, 10, 60, "f - to fill polygon",
                    strlen("f - to fill polygon"));
 
		//XFlush(display);

	}
	
	if (key_buffer[0] == 'f' && P.polygon == 1)
	{
		//fillPolygon();
		printf("Polygon Filled \n ");
		XDrawImageString(display, main_window, gc, 10, 70, "Filling Polygon",
                    strlen("Filling Polygon"));
		
	fillPolygon();
	
	}
		
	else printf("You pressed %c\n", key_buffer[0]); // log pushed keys
}


void calcLinePoints(int x1, int y1, int x2, int y2)
{

	printf("x1: %d, x2: %d, y1: %d, y2: %d \n", x1, x2, y1, y2);
	
	float x, y;
	int ind_x, ind_y;

	
	float a;
	float b;
	float incr=0;

	a = (float)(y2-y1)/(float)(x2-x1);
	b = (float)y1 - (float)x1 * a;

        printf("Points %f :: %f \n", a, b);
	//printf("Shesadareblebi: %f :: %f \n", x1, x2);
	
	if(x1 > x2) { 
		int tmp;
		tmp =x1; x1=x2; x2=tmp;
		}
	
	
		x=x1;

		while(x < x2)
			{

			y = a * x + b;
			
			ind_x = (int)x; ind_y = (int)y; 
			mainArray[ind_y][ind_x] = 1;
			//printf("Filled Array\n");
			//printf("Array at %d:%d = %d \n", ind_y, ind_x, mainArray[ind_y][ind_x]);
			
			if(a!=0)  {incr = 1/a;}
			
			printf("~~~incr =%f ; a= %f \n", incr, a);
			
			if(incr<0) incr=-incr;
			x=x+incr;
			}
	
}

fillPolygon(){


int x;
int y;

for(y=0; y<700; y++)
{
	int flag=1;
	for(x=0; x<700; x++)
	{  	
		if( mainArray[y][x] == 1 && flag == 1) 
		{
			crossXArray[y]+=1;
			if(mainArray[y][x] == mainArray[y][x+1] ) {flag=0;}
			//XDrawPoint(display, main_window, gc, x+40, y);
		}
	}
	flag=1;
}


usleep(5000);
int draw = 0;
//
for(y=700; y>0; y--)
	{
	for(x=0; x<700; x++)
		{       

			//			
			if( mainArray[y][x] == 1 )
			{ 
				if(draw == 0) 
				{draw=1;}
			  	else {draw=0;}

			  printf("Changed\n");	
			  //usleep(1000);	
				
			}
			

			if(draw == 1 )
			{			
				if(crossXArray[y] == 1) 
					{
					XDrawPoint(display, main_window, gc, x, y);
					
					}

				else
				{
				printf("Filling\n");			
				XDrawPoint(display, main_window, gc, x, y);
				//usleep(1);
				XFlush(display);
				}

				if( mainArray[y][x+1] == 1 && mainArray[y][x+2] != 1 )
				{ 
					break;
				}
			}	
			//
		}
	draw=0;	
	}
//
/*
for(y=0; y<700; y++)
	{  
			printf("~ %d \n" , crossXArray[y]);	
	}


*/
XDrawImageString(display, main_window, gc, 10, 80, "Done Filling",
                    strlen("Done Filling"));
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
