
#include <GL/glut.h>
#include "parametricfunctions.h"
#include "globals.h"

/*
float LadoSize(float radio, int lados){
	return radio*2*sinf(M_PI/lados);
}
*/
#define LADO_LENGTH(radio,lados) ((radio)*2*sinf(M_PI/(float)(lados)))

void DrawCylinder(GLuint lados=9, GLfloat radio=0.5, GLfloat altura=1){
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

void DrawPieza(size_t lados=5, float largo=10, float ancho=1,float * pos_trans=NULL, size_t cant_trans=3 ){
	size_t i;
	float def_pos[]={1,3/7.0,5/7.0};
	float phi=180/(float)lados;

	if (pos_trans==NULL)
		pos_trans=def_pos;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
		glPushMatrix();
			for (i=0;i<cant_trans;i++){
				glPushMatrix();
					glTranslated(0,largo*pos_trans[i],0);
					glRotatef(phi,1,0,0);
					glScalef(ancho,ancho,LADO_LENGTH(largo*pos_trans[i], lados));
					glCallList(cilindro_base);
				glPopMatrix();
			}

		glPopMatrix();
		glPushMatrix();
			glRotatef(-90,1,0,0);
//          glTranslated(0,0,0.5);
			glScalef(ancho,ancho,largo);
			glCallList(cilindro_base);
		glPopMatrix();
	glPopMatrix();
	return;
}                                                  

void DrawWheel(size_t lados, GLuint pieza){
	int i;
	glPushMatrix();
		glRotatef(360/(float)lados-90,1,0,0);//correccion de giro
		for (i=0;i<lados;i++){
	    	glRotatef(360/(float)lados,1,0,0);
			glCallList(pieza);
		}
	glPopMatrix();
	return;
}
