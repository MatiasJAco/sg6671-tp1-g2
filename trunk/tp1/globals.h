//Declaración de variables Globales para listas.

#include <GL/glut.h>

typedef enum  {interna, externa} camMod;

extern GLuint cilindro_base;
extern GLuint pieza_rueda_chica;
extern GLuint pieza_rueda_grande;
extern GLuint rueda_chica;
extern GLuint rueda_grande;
extern GLuint DL_PARTBASE;
extern GLuint DL_WALL;
extern GLuint DL_CORNER;
extern GLuint DL_CEILING;
extern GLuint DL_FLOOR;
extern GLuint DL_SWALL;
extern int rotation_bigw;
extern float xDelta;
extern float yDelta;
extern float newX;
extern float newY;
extern float newZ;
extern int mouseX;
extern int mouseY;
extern int mouseXante;
extern int mouseYante;
extern float alfa;
extern float beta;
extern float radio;
extern bool first;

extern camMod camara;

// Variables que controlan la ubicación de la cámara en la Escena 3D
extern float eye[3];
extern float at[3];
extern float up[3];
// Variables asociadas a única fuente de luz de la escena
extern float light_color[4];
extern float light_position[3];
extern float light_ambient[4];
// Color de la esfera en movimiento dentro de la escena
// Variables de control
extern bool view_grid;
extern bool view_axis;
extern bool edit_panel;
