///
/// 66.71 Sistemas Graficos
/// Framework de trabajo para el TP1
/// 2do Cuatrimestre 2010
///
/// info@sg6671.com.ar
///

#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include "parametricfunctions.h"
#include "globals.h"
#include "functions.h"
#include <iostream>
using namespace std;

// Handle para el control de las Display Lists
GLuint dl_handle;
#define DL_AXIS (dl_handle+0)
#define DL_GRID (dl_handle+1)
#define DL_AXIS2D_TOP (dl_handle+2)

// Tama�o de la ventana
GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

#define TOP_VIEW_POSX	((int)((float)W_WIDTH*0.60f))
#define TOP_VIEW_W		((int)((float)W_WIDTH*0.40f))
#define TOP_VIEW_POSY	((int)((float)W_HEIGHT*0.60f))
#define TOP_VIEW_H		((int)((float)W_HEIGHT*0.40f))

//Variables para animacion
int ms = 180;
int rotation_smallw=0;

//Variables para camara

float eyemod[3]={0,0,0};

void Avanzar(int value) {


	rotation_bigw = (rotation_bigw + 1) % 360;
	//a pesar que multiplico por -3 la rotacion la hace con (-2)
	rotation_smallw=rotation_bigw*(-3);
    glutTimerFunc(ms,Avanzar,1);
    glutPostRedisplay();
}



void DrawAxis()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		// X
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(15.0, 0.0, 0.0);
		// Y
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 15.0, 0.0);
		// Z
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 15.0);
	glEnd();
	glEnable(GL_LIGHTING);
}

void DrawAxis2DTopView()
{
	glDisable(GL_LIGHTING);
	glBegin(GL_LINE_LOOP);
		// X
		glColor3f(0.0f, 0.5f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.1f, 0.1f, 0.1f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 1.0f, 0.0f);
	glEnd();

	glEnable(GL_LIGHTING);
}


void DrawXYGrid()
{
	int i;
	glDisable(GL_LIGHTING);
	glColor3f(0.15f, 0.1f, 0.1f);
	glBegin(GL_LINES);
	for(i=-20; i<21; i++)
	{
		glVertex3f((float)i, -20.0f, 0.0f);
		glVertex3f((float)i,  20.0f, 0.0f);
		glVertex3f(-20.0f, (float)i, 0.0f);
		glVertex3f( 20.0f, (float)i, 0.0f);
	}
	glEnd();
	glEnable(GL_LIGHTING);
}
void Set3DEnv()
{
	glViewport (0, 0, (GLsizei) W_WIDTH, (GLsizei) W_HEIGHT); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) W_WIDTH/(GLfloat) W_HEIGHT, 0.10, 100.0);
}

void SetPanelTopEnv()
{
	glViewport (TOP_VIEW_POSX, TOP_VIEW_POSY, (GLsizei) TOP_VIEW_W, (GLsizei) TOP_VIEW_H); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	gluOrtho2D(-0.10, 1.05, -0.10, 1.05);
}


void init(void) 
{
	float distancias_rueda_grande[]={1,3/7.0,5/7.0};
	float distancias_rueda_chica[]={1,1/2.0};
//Asigno indices a las listas
	dl_handle = glGenLists(13);
	cilindro_base=glGenLists(1);
	pieza_rueda_chica=glGenLists(1);
	pieza_rueda_grande=glGenLists(1);
	rueda_chica=glGenLists(1);
	rueda_grande=glGenLists(1);
	DL_PARTBASE=glGenLists(1);
	DL_WALL=glGenLists(1);
	DL_CORNER=glGenLists(1);
	DL_CEILING=glGenLists(1);
	DL_FLOOR=glGenLists(1);
	DL_SWALL=glGenLists(1);
	DL_CABIN=glGenLists(1);
	glClearColor (0.02f, 0.02f, 0.04f, 0.0f);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

// Generaci�n de las Display Lists
	glNewList(DL_AXIS, GL_COMPILE);
		DrawAxis();
	glEndList();
	glNewList(DL_GRID, GL_COMPILE);
		DrawXYGrid();
	glEndList();
	glNewList(DL_AXIS2D_TOP, GL_COMPILE);
		DrawAxis2DTopView();
	glEndList();

/*Carga las ruedas*/


	glNewList(cilindro_base, GL_COMPILE);
		DrawCylinder(33);
	glEndList();
	
	glNewList(pieza_rueda_grande, GL_COMPILE);
		DrawPieza(15,15,1,distancias_rueda_grande,3);
	glEndList();

	glNewList(rueda_grande, GL_COMPILE);
		DrawWheel(15,pieza_rueda_grande);
	glEndList();

	glNewList(pieza_rueda_chica, GL_COMPILE);
		DrawPieza(9,15/2.0,2/3.0,distancias_rueda_chica,2);
	glEndList();

	glNewList(rueda_chica, GL_COMPILE);
		DrawWheel(9,pieza_rueda_chica);
	glEndList();

	/*Carga partes para la base y cabina*/

	glNewList(DL_PARTBASE, GL_COMPILE);
	DrawParteBase();
	glEndList();
	glNewList(DL_WALL, GL_COMPILE);
	drawWalls();
	glEndList();
	glNewList(DL_CORNER, GL_COMPILE);
	drawCorner();
	glEndList();
	glNewList(DL_CEILING, GL_COMPILE);
	drawCeiling();
	glEndList();
	glNewList(DL_FLOOR, GL_COMPILE);
	drawFloor();
	glEndList();
	glNewList(DL_SWALL, GL_COMPILE);
	drawSecondWall();
	glEndList();
	glNewList(DL_CABIN, GL_COMPILE);
	drawCabina();
	glEndList();
//mouse coords
	glutPassiveMotionFunc(initMouse);

}


void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///////////////////////////////////////////////////
	// Escena 3D
	Set3DEnv();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if (view_axis)
		 glCallList(DL_AXIS);

	eyecorrection(eyemod,rotation_bigw);

	eyecorrection(eyemod,rotation_bigw);

	if (camara==interna)
		gluLookAt (eyemod[0]+eye[0], eyemod[1]+eye[1],eyemod[2]+eye[2],eyemod[0]+newX,eyemod[1]+newY,eyemod[2]+newZ , up[0], up[1], up[2]);
	else
		gluLookAt (newX, newY, newZ, 0, 0, 30, up[0], up[1], up[2]);
	
	if (view_grid)
		 glCallList(DL_GRID);
	//
	///////////////////////////////////////////////////

    ///////////////////////////////////////////////////
	//
	// Draw here
	//	
	//


	int i=3;
	int j;
	int k;
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);



	glTranslatef(0,0,30);
	
	//Dibuja la Base
	glPushMatrix();
		glColor3f(0.4, 0.4, 0.4);
		glTranslatef(+5,0,-30);
		glScalef(5, 5, 5);
		drawBase();
	glPopMatrix();
	
	//Rota la rueda Grande
	glRotatef(rotation_bigw,1,0,0);

	glPushMatrix();
	//Itera para las 3 ruedas chicas
		for(j=0;j<i;j++){
		glRotatef(360/(float)i,1,0,0);
		glPushMatrix();
	//Dibuja el eje de las ruedas chicas
		glTranslatef(0,0,-15);
			glPushMatrix();
				glTranslatef(-4,0,0);
				glRotatef(90,0,1,0);
				glScalef(2/3.0,2/3.0,8);
				glColor3f(0, 1, 1);
				glCallList(cilindro_base);
			glPopMatrix();
				glPushMatrix();
					glRotatef(rotation_smallw,1,0,0);
					glPushMatrix();
						for(k=0;k<i;k++){
			//Rota la Rueda Chica	
							glRotatef(360/(float)i,1,0,0);
			//Dibuja los ejes de las canastas
							glPushMatrix();
								glTranslatef(0,0,-15/2.0);
								glPushMatrix();
								glPushAttrib(GL_CURRENT_BIT);
									glColor3f(0.3, 0.5, 0.25);
									glTranslatef(-3,0,0);
									glRotatef(90,0,1,0);
									glScalef(2/3.0*2/3.0,2/3.0*2/3.0,6);
									glCallList(cilindro_base);
								glPopAttrib();
								glPopMatrix();
//Canasta
        	                                                glPushMatrix();
	                                                        glPushAttrib(GL_CURRENT_BIT);
                	                                                glColor3f(1.0,0.3,0.1);
                        	                                	glRotatef(-360/(float)i*(k+1+j+1)+(2*rotation_bigw),1,0,0);
                                	                                glScalef((LADO_LENGTH(15/2.0,9))/1.8,(LADO_LENGTH(15/2.0,9))/1.8,(LADO_LENGTH(15/2.0,9)/1.8));
                                        	                        glTranslatef(0,0,-2);
                                        	                        glCallList(DL_CABIN);
                                                	        glPopAttrib();
	                                                        glPopMatrix();
							glPopMatrix();
							
						}
	//Dibuja la rueda chica
					glPopMatrix();
					
					glPushAttrib(GL_CURRENT_BIT);
						glColor3f(0.3, 0.5, 0.25);
						glTranslatef(-3,0,0);
						glCallList(rueda_chica);
						glTranslatef(6,0,0);
						glCallList(rueda_chica);
                                        glPopAttrib();
				glPopMatrix();
			glPopMatrix();
		}
		glPopMatrix();
	//Dibuja larueda Grande
		glColor3f(0, 1, 1);
		glTranslatef(-4,0,0);
		glCallList(rueda_grande);
		glTranslatef(8,0,0);
		glCallList(rueda_grande);

	glPopMatrix();


	//
	///////////////////////////////////////////////////


	///////////////////////////////////////////////////
	// Panel 2D para la vista superior
	if (edit_panel)
	{
		SetPanelTopEnv();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (0, 0, 0.5, 0, 0, 0, 0, 1, 0);
		glCallList(DL_AXIS2D_TOP);
	}
	//
	///////////////////////////////////////////////////
	
	

	glutSwapBuffers();
}

void reshape (int w, int h)
{
   	W_WIDTH  = (float)w;
	W_HEIGHT = (float)h;
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case 'q':
         exit(0);
         break;
	case 'a':
		ms++;
		break;
	case 's':
		ms--;
		break;
	case 'c':
		cameraChange();
		break;
     default:
         break;
   }
}





int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (1024, 768); 
   glutInitWindowPosition (0, 0);
   
   glutCreateWindow (argv[0]);
   glutFullScreen();
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutTimerFunc(ms,Avanzar,1);
   glutPassiveMotionFunc(mouseCam);
   glutMouseFunc(mouseButton);
   glutMainLoop();
   return 0;
}
