//Declaración de variables Globales para listas.
//Si al final quedan pocas las pasamos al main.
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>

GLuint cilindro_base=0;
GLuint pieza_rueda_chica=0;
GLuint pieza_rueda_grande=0;
GLuint rueda_chica=0;
GLuint rueda_grande=0;
GLuint DL_PARTBASE=0;
GLuint DL_WALL=0;
GLuint DL_CORNER=0;
GLuint DL_CEILING=0;
GLuint DL_FLOOR=0;
GLuint DL_SWALL=0;


int rotation_bigw = 0;

 float xDelta=0;
 float yDelta=0;
 float newX=0;
 float newY=0;
 float newZ=0;
 int mouseX=0;
 int mouseY=0;
 int mouseXante=0;
 int mouseYante=0;

 float radio=50.0;
 bool first=true;
