//Declaración de variables Globales para listas.
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "globals.h"

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
GLuint DL_CABIN=0;

int rotation_bigw = 0;

 float xDelta=0;
 float yDelta=0;
 float newX=0;
 float newY=-1; //mira para adentro
 float newZ=0;
 int mouseX=0;
 int mouseY=0;

 camMod camara=interna;

 float radio=60.0;
 bool first=true;



// Variables que controlan la ubicación de la cámara en la Escena 3D
float eye[3] = {0, 0, 0.0};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};
// Variables asociadas a única fuente de luz de la escena
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float light_position[3] = {10.0f, 10.0f, 8.0f};
float light_ambient[4] = {0.05f, 0.05f, 0.05f, 1.0f};
// Color de la esfera en movimiento dentro de la escena
// Variables de control
bool view_grid = true;
bool view_axis = true;
bool edit_panel = false;

