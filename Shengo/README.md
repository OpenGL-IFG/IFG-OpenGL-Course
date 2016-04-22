# IFG-OpenGL-Course

change library adress in both files 
from "#include <GLUT/glut.h>" to "#include <GL/glut.h>"

to compile: 
$gcc -o cube -lX11 -lGLU -lGL -lglut cube.c
$gcc -o light -lX11 -lGLU -lGL -lglut light.c;

if needed for glut.h :
#apt-get install freeglut3-dev
