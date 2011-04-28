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

	GLfloat Vertices [24]={1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0,
			1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0,
			-1.0, 1.0, -1.0, -1.0, -1.0, -1.0};
	GLubyte allIndices [16]= {0, 3, 4, 5, 0, 5, 6, 1, 1, 6, 7, 2, 7, 4, 3, 2};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawElements(GL_QUADS, 16, GL_UNSIGNED_BYTE, allIndices);
	glDisableClientState(GL_VERTEX_ARRAY);




};
void drawSecondWall(){

	GLfloat Vertices [24]={1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0,
			1.0, 1.0, -1.0, 1.0, 0.8, -0.8, -1.0, 0.8, 0.8, -1.0,
			-0.8, 0.8, -1.0, -0.8, -0.8, -1.0};
	GLubyte allIndices [16]= {0, 3, 4, 5, 0, 5, 6, 1, 1, 6, 7, 2, 7, 4, 3, 2};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawElements(GL_QUADS, 16, GL_UNSIGNED_BYTE, allIndices);
	glDisableClientState(GL_VERTEX_ARRAY);

};
void drawFloor(){

	GLfloat Vertices [12]= {0.8, -0.8, 0.0 , -0.8, -0.8, 0.0, -0.8,
			0.8, 0.0, 0.8, 0.8, 0.0};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_VERTEX_ARRAY);

}

void drawCeiling(){
	GLfloat Vertices [18]= {0.0, 0.0, 1.0, 1.0, 1.0, 0.0, -1.0,
	1.0, 0.0, -1.0, -1.0, 0.0, 1.0, -1.0, 0.0, 1.0, 1.0, 0.0};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
	glDisableClientState(GL_VERTEX_ARRAY);

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

	GLfloat Vertices [18]={-1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0,
			1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0};
	GLubyte allIndices [8]= {0, 4, 5, 1, 5, 3, 2, 1};
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, Vertices);
	glDrawElements(GL_QUADS, 8, GL_UNSIGNED_BYTE, allIndices);
	glDisableClientState(GL_VERTEX_ARRAY);
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

void eyecorrection(float* e, float b){
	//Movimiento de rueda chica
	float phi=((((-2*b)-90))*M_PI)/180.0;
	int radioRuedaChica=7;
	int radioRuedaGrande=15;
	e[1]=radioRuedaChica*cosf(phi);
	e[2]=radioRuedaChica*sinf(phi);

	//Muevo en z.
	e[2]=e[2] + 15;
	float theta=(((b-90))*M_PI)/180.0;
	//Movimiento por grande
	float y=radioRuedaGrande*cosf(theta);
	float z=radioRuedaGrande*sinf(theta);

	//Sumo la traslacion por rueda grande.
	e[1]=e[1] + y;
	e[2]=e[2] + z+15;

	//Bajo a la cabina
	e[2]=e[2] - 4;



};

void mouseCam( GLint xMouse, GLint yMouse){

//	if( button==GLUT_LEFT_BUTTON && action == GLUT_DOWN){
		if(first){
			mouseX=xMouse;
			mouseY=yMouse;
			mouseXante=xMouse;
			mouseYante=yMouse;
			first =false;
		}else{
			int difAnteX=xMouse-mouseXante;
			int difAnteY=yMouse-mouseYante;
			int difX=xMouse-mouseX;
			int difY=yMouse-mouseY;
			xDelta=(float)((difX)% 360);
			yDelta=(float)((difY)% 360);

			float alfa=xDelta;
			alfa=(alfa*M_PI)/180.0;
			float beta=yDelta;
			beta=(beta*M_PI)/180.0;
			if(difAnteX!=0 && difAnteY!=0){
				newX=radio*cosf(alfa);
				newY=radio*cosf(beta);
				newZ=radio*sinf(beta);
			};
			if(difAnteX==0 && difAnteY!=0){
				newY=radio*cosf(beta);
				newZ=radio*sinf(beta);
			};
			if(difAnteX!=0 && difAnteY==0){
				newX=radio*cosf(alfa);
				newY=radio*sinf(alfa);
			};
			mouseXante=xMouse;
			mouseYante=yMouse;

		};


	};


