/*
 *
 *  Created on: 26/03/2011
 *      Author: Mat�as
 */

#include <math.h>
#define LADO_LENGTH(radio,lados) ((radio)*2*sinf(M_PI/(float)(lados)))

void DrawCylinder(GLuint lados=10, GLfloat radio=0.5, GLfloat altura=1);

/**
 * Dibuja una pieza base de la rueda seg�n las especificaciones
 *  recibidas por par�metro.
 ***/
void DrawPieza(size_t lados=5, float largo=10, float ancho=1, float * pos_trans=NULL, size_t cant_trans=3);

void DrawWheel(size_t, GLuint);
void drawCeiling();
void drawFloor();
void drawWalls();

/**
 * Dibuja las paredes cercanas al piso de la cabina.
 **/
void drawSecondWall();

/**
 * Dibuja una parte de la Base utilizando cilindros.
 **/
void DrawParteBase();


/**
 * Dibuja una esquina correspondiente a la cabina , la cual une
 * al techo con las paredes y permite tener ventanas.
 * **/
void drawCorner();

void drawCabina();
void drawBase();

/**
 * Calcula la posici�n de la cabina que comienza en la posici�n
 * mas cercana a z=0. Esta posici�n se utiliza para ubicar la c�mara
 *  dentro de la cabina.
 * **/
void eyecorrection(float *, float);
/**
 * Inicializa variables utilizadas para seguir la posici�n
 * del cursor del mouse.
 * **/
void initMouse( GLint xMouse, GLint yMouse);

/**
 * Utilizando la posici�n actual del mouse, obtiene las
 * diferencias respecto a una posici�n anterior y mapea estas diferencias
 * en un angulo (alfa y beta)para obtener las coordenadas correspondientes
 * en una esfera.
 * **/
void mouseCam( GLint xMouse, GLint yMouse);

void mouseButton( int, int, int,int);

/**
 * Realiza el cambio de c�mara seg�n se haya presionado la tecla �c� o no.
 * **/
void cameraChange();
void initIntCam();
void initExtCam();
