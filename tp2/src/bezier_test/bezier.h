/*
 * bezier.h
 *
 *  Created on: 02/06/2011
 *      Author: Matías
 */

#ifndef BEZIER_H_
#define BEZIER_H_
#include "math.h"
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <GL/glu.h>
#include <cstdlib>
#include <time.h>
//#include "spline.h"
#include <algorithm>
#include <iostream>
#include <cstdio>
typedef struct
{
float x;
float y;
}tVertice; // vertices
typedef struct

{
float x;
float y;
float z;
}tPunto; // vertices

void dibujaSupBezier(tVertice* ptosControl, float paso, float angulo, int tramos);
tPunto calcularPuntoDeBezier(float u, tVertice* ptsControl);
tPunto calcularNormal(float u, tVertice* ptsControl, tPunto planoNormal );
tPunto rotarPunto(float alpha, tPunto p);


#endif /* BEZIER_H_ */
