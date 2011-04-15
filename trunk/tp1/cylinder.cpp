#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include "parametricfunctions.h"
#include "globals.h"
#include "cylinder.h"

/*
float LadoSize(float radio, int lados){
	return radio*2*sinf(M_PI/lados);
}
*/

void DrawCylinder(GLuint lados, GLfloat radio, GLfloat altura){
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

void DrawPieza(size_t lados, float largo, float ancho,float * pos_trans, size_t cant_trans ){
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


void drawWalls(){

	glBegin(GL_QUADS);

	        // v0-v3-v4-v6
	        glNormal3f(1,0,0);

	        glVertex3f(1,1,1);

	        glVertex3f(1,-1,1);

	        glVertex3f(1,-1,-1);

	        glVertex3f(1,1,-1);

	        //v0-v5-v6-v1
	        glNormal3f(0,1,0);
	        glVertex3f(1,1,1);
	        glVertex3f(1,1,-1);
	        glVertex3f(-1,1,-1);
	        glVertex3f(-1,1,1);

	        //v1-v6-v7-v2
	        glNormal3f(-1,0,0);
	        glVertex3f(-1,1,1);
	        glVertex3f(-1,1,-1);
	        glVertex3f(-1,-1,-1);
	        glVertex3f(-1,-1,1);

	        //v7-v4-v3-v2
	        glNormal3f(0,-1,0);
	        glVertex3f(-1,-1,-1);
	        glVertex3f(1,-1,-1);
	        glVertex3f(1,-1,1);
	        glVertex3f(-1,-1,1);
	        glEnd();



};
void drawSecondWall(){

	glBegin(GL_QUADS);

	// v0-v3-v4-v6
	glNormal3f(1,0,0);
	glVertex3f(1,1,1);
	glVertex3f(1,-1,1);
	glVertex3f(0.8,-0.8,-1);
	glVertex3f(0.8,0.8,-1);

	//v0-v5-v6-v1
	glNormal3f(0,1,0);
	glVertex3f(1,1,1);
	glVertex3f(0.8,0.8,-1);
	glVertex3f(-0.8,0.8,-1);
	glVertex3f(-1,1,1);

	//v1-v6-v7-v2
	glNormal3f(-1,0,0);
	glVertex3f(-1,1,1);
	glVertex3f(-0.8,0.8,-1);
	glVertex3f(-0.8,-0.8,-1);
	glVertex3f(-1,-1,1);

	// v7-v4-v3-v2
	glNormal3f(0,-1,0);
	glVertex3f(-0.8,-0.8,-1);
	glVertex3f(0.8,-0.8,-1);
	glVertex3f(1,-1,1);
	glVertex3f(-1,-1,1);

	glEnd();


};
void drawFloor(){

	glBegin(GL_QUADS);
    // v4-v7-v6-v5
	glNormal3f(0,0,0);
	glVertex3f(0.8,-0.8,0);
	glVertex3f(-0.8,-0.8,0);
	glVertex3f(-0.8,0.8,0);
	glVertex3f(0.8,0.8,0);

	glEnd();


}

void drawCeiling(){

	glBegin(GL_TRIANGLE_FAN);

	glVertex3f(0,0,1);
	glVertex3f(1,1,0);
	glVertex3f(-1,1,0);
	glVertex3f(-1,-1,0);
	glVertex3f(1,-1,0);
	glVertex3f(1,1,0);



	glEnd();


}


void DrawParteBase(){


	glBegin(DL_PARTBASE);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
		glTranslatef(0.0, -1.0, 0.0);
	    glRotatef(-90, 1.0, 0.0, 0.0);
	    glScalef(0.2, 0.2, 2.0);
	    glCallList(cilindro_base);
    glPopMatrix();
	    //Parte superior
	    glPushMatrix();
	    	glTranslatef(0.0, 1.0, 0.0);
	    	glRotatef(9.5, 1.0, 0.0, 0.0);
	    	glScalef(0.2, 0.2, 6.0);
	    	glCallList(cilindro_base);
	    glPopMatrix();
	    glPushMatrix();
	    	glTranslatef(0.0, -1.0, 0.0);
	    	glRotatef(-9.5, 1.0, 0.0, 0.0);
	    	glScalef(0.2, 0.2, 6.0);
	    	glCallList(cilindro_base);
	    glPopMatrix();
	    //Partes medias
	    glPushMatrix();
	    	glScalef(1.0, 0.7, 1.0);
	    	glTranslatef(0.0, -0.5, 4.0);
	    	glRotatef(-90, 1.0, 0.0, 0.0);
	    	glScalef(0.2, 0.2, 1.0);
	    	glCallList(cilindro_base);
	    glPopMatrix();

	    glPushMatrix();
	    	glScalef(1.0, 1.4, 1.0);
	    	glTranslatef(0.0, -0.5, 2.0);
	    	glRotatef(-90, 1.0, 0.0, 0.0);
	    	glScalef(0.2, 0.2, 1.0);
	    	glCallList(cilindro_base);
	    glPopMatrix();

	glEnd();


}

void drawCorner(){

		glBegin(GL_QUADS);

		// v1-v6-v7-v2
		glNormal3f(-1,0,0);
		glVertex3f(-1,1,1);
		glVertex3f(-1,1,-1);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1,1);

		// v7-v4-v3-v2
		glNormal3f(0,-1,0);
		glVertex3f(-1,-1,-1);
		glVertex3f(1,-1,-1);
		glVertex3f(1,-1,1);
		glVertex3f(-1,-1,1);

	     glEnd();



};

void drawCabina()
{

    //Dibujando cabina

    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.9);
    glScalef(0.4, 0.8, 0.3);
    glCallList(DL_CEILING);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0, 0.0, 1.7);
    glScalef(0.4, 0.8, 0.1);
    glTranslatef(0.0, 0.0, 1);
    glCallList(DL_WALL);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4, 0.7, 0.7);
    glScalef(0.05, 0.05, 0.5);
    glTranslatef(1.0, 1.0, 1.0);
    glRotatef(270, 0.0, 0.0, 1.0);
    glCallList(DL_CORNER);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.3, 0.7, 0.7);
    glScalef(0.05, 0.05, 0.5);
    glTranslatef(1.0, 1.0, 1.0);
    glRotatef(180, 0.0, 0.0, 1.0);
    glCallList(DL_CORNER);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.3, -0.8, 0.7);
    glScalef(0.05, 0.05, 0.5);
    glTranslatef(1.0, 1.0, 1.0);
    glRotatef(90, 0.0, 0.0, 1.0);
    glCallList(DL_CORNER);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-0.4, -0.8, 0.7);
    glScalef(0.05, 0.05, 0.5);
    glTranslatef(1.0, 1.0, 1.0);
    glCallList(DL_CORNER);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.4, 0.8, 1.0);
    glTranslatef(0.0, 0.0, -0.4);
    glCallList(DL_FLOOR);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.4, 0.8, 0.2);
    glTranslatef(0.0, 0.0, -1);
    glCallList(DL_SWALL);
    glPopMatrix();
    glPushMatrix();
    glScalef(0.4, 0.8, 0.37);
    glTranslatef(0.0, 0.0, 1);
    glCallList(DL_WALL);
    glPopMatrix();
}

void drawBase()
{

    glCallList(DL_PARTBASE);
    glPushMatrix();
    glTranslatef(-2.0, 0.0, 0.0);
    glCallList(DL_PARTBASE);
    glPopMatrix();
    //Unions.
    glPushMatrix();
    	glTranslatef(0.0, 1.0, 0.0);
    	glRotatef(-90, 0.0, 0.0, 1.0);
    	glRotatef(90, 1.0, 0.0, 0.0);
    	glScalef(0.2, 0.2, 2.0);
    	glCallList(cilindro_base);
    glPopMatrix();

    glPushMatrix();
    	glTranslatef(0.0, -1.0, 0.0);
    	glRotatef(90, 0.0, 0.0, 1.0);
    	glRotatef(-90, 1.0, 0.0, 0.0);
    	glScalef(0.2, 0.2, 2.0);
    	glCallList(cilindro_base);
    glPopMatrix();
    glPushMatrix();
    	glColor3f(1.0,0.0,0.0);
    	glTranslatef(0.0, 0.0, 6.0);
    	glRotatef(90, 0.0, 0.0, 1.0);
    	glRotatef(-90, 1.0, 0.0, 0.0);
    	glScalef(0.2, 0.2, 2.0);
    	glCallList(cilindro_base);
    glPopMatrix();

}
