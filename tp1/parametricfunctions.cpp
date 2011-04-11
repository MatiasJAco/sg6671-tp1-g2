/*
 * spherefunction.cpp
 *
 *  Created on: 26/03/2011
 *      Author: Matías
 */

#include <GL/glut.h>
#include <math.h>
#include "parametricfunctions.h"

void SpherePoints(float* coordinates,float ratio, float theta,float phi){
	float initx=0.0;
	float inity=0.0;
	float initz=5.0;
	coordinates[0]=initx+ratio*sinf(theta)*cosf(phi);
	coordinates[1]=inity+ratio*sinf(theta)*sinf(phi);
	coordinates[2]=initz+ratio*cosf(theta);
}

void CylinderPoints(float* coordinates,float ratio, float phi,float coorz){
	float initx=0.0;
	float inity=0.0;

	coordinates[0]=initx+ratio*cosf(phi);
	coordinates[1]=inity+ratio*sinf(phi);
	coordinates[2]=coorz;



}

/*void DrawCylinder(GLuint lados, GLfloat radio, GLfloat altura){
	//lados: vértices en los que se discretiza el círculo.
	GLfloat * puntos;
	GLfloat * normales;

	int i;
	float phi,step;
	
	puntos=new GLfloat[lados*3*2];
	normales=new GLfloat[lados*3];

	//Calcula los Vértices

	step=2*M_PI/lados;
	phi=0;

	for (i=0; i<lados ; i++){
		CylinderPoints(puntos+3*i, radio, phi, 0);
		phi+=step;
		(normales+3*i)[0]=(puntos+3*(lados+i))[0]=(puntos+3*i)[0];
		(normales+3*i)[1]=(puntos+3*(lados+i))[1]=(puntos+3*i)[1];
		(puntos+(lados+i)*3)[2]=altura;
		(normales+3*i)[2]=0;
	};

	//Dibujando:
	
	glEnableClientState(GL_VERTEX_ARRAY);
	
	glVertexPointer(3,GL_FLOAT,0,puntos);
		
		//tapa superior
		glNormal3f(0,0,-1);
		glDrawArrays(GL_POLYGON,0,lados);
		//tapa inferior
		glNormal3f(0,0,1);
		glDrawArrays(GL_POLYGON,lados,lados);
		//laterales:
		glBegin(GL_QUAD_STRIP);
			for (i=0; i<lados; i++){
				glNormal3fv(normales+i*3);
				glArrayElement(i);
				glArrayElement(i+lados);
			}
			glNormal3fv(normales);
			glArrayElement(0);
			glArrayElement(lados);
		glEnd();

	delete puntos;
	delete normales;
	return;
	}	
*/
