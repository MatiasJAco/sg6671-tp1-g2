/*
 *
 *  Created on: 26/03/2011
 *      Author: Matías
 */

#include <math.h>
#define LADO_LENGTH(radio,lados) ((radio)*2*sinf(M_PI/(float)(lados)))

void DrawCylinder(GLuint lados=10, GLfloat radio=0.5, GLfloat altura=1);
void DrawPieza(size_t lados=5, float largo=10, float ancho=1, float * pos_trans=NULL, size_t cant_trans=3);
void DrawWheel(size_t, GLuint);
void drawCeiling();
void drawFloor();
void drawWalls();
void drawSecondWall();
void DrawParteBase();
void drawCorner();
void drawCabina();
void drawBase();
void eyecorrection(float *, float);
void initMouse( GLint xMouse, GLint yMouse);
void mouseCam( GLint xMouse, GLint yMouse);
void mouseButton( int, int, int,int);
void cameraChange();
void initIntCam();
void initExtCam();
