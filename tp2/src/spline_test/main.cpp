///
/// 66.71 Sistemas Graficos
/// Framework de trabajo para el TP1
/// 2do Cuatrimestre 2010
///
/// info@sg6671.com.ar
///

#include <GL/glut.h>
#include <GL/glu.h>
#include <math.h>
#include <cstdlib>
#include <time.h>
#include "spline.h"
#include <algorithm>
#include <iostream>
#include <cstdio>



#define checkImageWidth 64
#define checkImageHeight 64


void makeCheckImage(void)
{
   int i, j, c;

   for (i = 0; i < checkImageHeight; i++) {
      for (j = 0; j < checkImageWidth; j++) {
         c = ((((i&0x8)==0)^((j&0x8))==0))*255;
         checkImage[i][j][0] = (GLubyte) c;
         checkImage[i][j][1] = (GLubyte) c;
         checkImage[i][j][2] = (GLubyte) c;
         checkImage[i][j][3] = (GLubyte) 255;
      }
   }
}


void draw_curve(){

	vector<float> ctlVector;
	vector<float> pointsVector;
	float * coordsArray;
	size_t i;

	ctlVector.resize(2*4);
	copy(&(ctlpoints[0][0]),&(ctlpoints[0][0])+2*4,ctlVector.begin());
	pointsVector=calcularPuntosEquiespaciados2D(ctlVector,0.1);

	coordsArray=new float[pointsVector.size()/4*16];//3Coord,3Norm,2Text
//	lista = glGenLists(1);
	for (i=0; i < pointsVector.size()/4; i++){
		//CoordInferior
		coordsArray[i*16+0]=pointsVector[i*4];
		coordsArray[i*16+1]=pointsVector[i*4+1];
		coordsArray[i*16+2]=0;
		//Normal
		coordsArray[i*16+3]=pointsVector[i*4+2];
		coordsArray[i*16+4]=pointsVector[i*4+3];
		coordsArray[i*16+5]=0;
		//TexCoord
		coordsArray[i*16+6]=i/10.0;
		coordsArray[i*16+7]=0;
		
		//CoordSuperior
		coordsArray[i*16+8]=pointsVector[i*4];
		coordsArray[i*16+9]=pointsVector[i*4+1];
		coordsArray[i*16+10]=3;
		//Normal
		coordsArray[i*16+11]=pointsVector[i*4+2];
		coordsArray[i*16+12]=pointsVector[i*4+3];
		coordsArray[i*16+13]=0;
		
		//TexCoord
		coordsArray[i*16+14]=i/10.0;
		coordsArray[i*16+15]=1;
		
//
	}
//		for(i=0; i<pointsVector.size()/2;i++)
//			cout<<coordsArray[i+0]<<','<<coordsArray[i+1]<<','<<coordsArray[i+2]<<endl;
	

//	glNewList(lista, GL_COMPILE);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texName);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//		glVertexPointer(2,GL_FLOAT,4*sizeof(float),&pointsVector[0]);
		glVertexPointer(3,GL_FLOAT,8*sizeof(float),coordsArray);
		glNormalPointer(GL_FLOAT,8*sizeof(float),&coordsArray[3]);
		glTexCoordPointer(2,GL_FLOAT,8*sizeof(float),&coordsArray[6]);
//		glColor3f(0.5,0.8,0.8);
//		glDrawArrays(GL_POINTS,0,pointsVector.size()/4);
		glColor3f(0.2,0.8,0.8);
		glDrawArrays(GL_TRIANGLE_STRIP,0,pointsVector.size()/4*2);
	glDisable(GL_TEXTURE_2D);
		
//	glEndList();

}

void init_surface()
{
//	init_points();
/*        int u, v;
        for (u=0; u<4; u++){
                for (v=0; v<4; v++){
                        ctlpoints[u][v][0]= 2.0*((GLfloat)u - 1.5);
                        ctlpoints[u][v][1]= 2.0*((GLfloat)u - 1.5);

                if ( (u==1 || u==2) && (v==1 || v==2))
                        ctlpoints[u][v][2] = 3.0;
                else
                        ctlpoints[u][v][2] = -3.0;
                }
        }*/
/*	int lados=10;
	int puntos=26;
        float phi=0;
	float radio=4;
        float step=2*M_PI/(float)lados;
        int i;
	srand(time(NULL));
	for (i=0; i<puntos ; i++){	
            ctlpoints[i][0]=radio*cosf(phi);//+(rand()%5000-5000/2)/1000.0;
		ctlpoints[i][1]=radio*sinf(phi);//+(rand()%5000-5000/2)/1000.0;
//		ctlpoints[i][2]=0;
                phi+=step;
       };*/

}


// Variables que controlan la ubicación de la cámara en la Escena 3D
float eye[3] = {15.0, 15.0, 5.0};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};

// Variables asociadas a única fuente de luz de la escena
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float light_position[3] = {10.0f, 10.0f, 8.0f};
float light_ambient[4] = {0.05f, 0.05f, 0.05f, 1.0f};

// Color de la esfera en movimiento dentro de la escena
float color_esfera[4] = {0.5f, 0.5f, 0.2f, 1.0f};

// Variable asociada al movimiento de rotación de la esfera alrededor del eje Z
float rotate_sphere = 0;

// Variables de control
bool view_grid = true;
bool view_axis = true;
bool edit_panel = false;


// Handle para el control de las Display Lists
GLuint dl_handle;
#define DL_AXIS (dl_handle+0)
#define DL_GRID (dl_handle+1)
#define DL_AXIS2D_TOP (dl_handle+2)
// Tamaño de la ventana
GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

#define TOP_VIEW_POSX	((int)((float)W_WIDTH*0.60f))
#define TOP_VIEW_W		((int)((float)W_WIDTH*0.40f))
#define TOP_VIEW_POSY	((int)((float)W_HEIGHT*0.60f))
#define TOP_VIEW_H		((int)((float)W_HEIGHT*0.40f))

void OnIdle (void)
{
/*	rotate_sphere += 0.1f;
	if(rotate_sphere > 360.0) rotate_sphere = 0.0;
    glutPostRedisplay();*/
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
    dl_handle = glGenLists(3);
    lista=glGenLists(1);

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth,
		    checkImageHeight,0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

    glClearColor (0.02f, 0.02f, 0.04f, 0.0f);
    glShadeModel (GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);

	// Generación de las Display Lists
	glNewList(DL_AXIS, GL_COMPILE);
		DrawAxis();
	glEndList();
	glNewList(DL_GRID, GL_COMPILE);
		DrawXYGrid();
	glEndList();
	glNewList(DL_AXIS2D_TOP, GL_COMPILE);
		DrawAxis2DTopView();
	glEndList();

	glNewList(lista,GL_COMPILE);
		draw_curve();
	glEndList();

/*	theNurb = gluNewNurbsRenderer();
        init_surface();
        gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 10.0);
        gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);*/
}



void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	///////////////////////////////////////////////////
	// Escena 3D
	Set3DEnv();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt (eye[0], eye[1], eye[2], at[0], at[1], at[2], up[0], up[1], up[2]);
   
	if (view_axis)
		 glCallList(DL_AXIS);
	
	if (view_grid)

		 glCallList(DL_GRID);
	//
	///////////////////////////////////////////////////

	///////////////////////////////////////////////////
	//
	// Draw here
	//
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//	glEnable(GL_AUTO_NORMAL);
//	glEnable(GL_NORMALIZE);

	glPointSize(10.0);
	//	GLfloat knots[] = { 0.0, 0.0, 0.0, 0.0, 1.1, 1.1, 1.1, 1.1};
	//	GLfloat knots[] = { 0.0, 0.0, 0.0, 0.0,0.0,0.0,0.0, 1.0, 1.0, 1.0, 1.0,1.0,1.0,1.0};
	GLfloat knots[] = { 0, 1, 2, 3, 4,5, 6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};

glColor3f(0.6, 0.2, 0.6);
/*
        glPushMatrix();
                gluBeginSurface(theNurb);
                gluNurbsSurface(theNurb,
                        8, knots,
                        8, knots,
                        4 * 3,
                        3,
                        &ctlpoints[0][0][0],
                        4, 4,
                        GL_MAP2_VERTEX_3);
                gluEndSurface(theNurb);
        glPopMatrix();

*/
/*	glPushMatrix();
		gluBeginCurve(theNurb);
		gluNurbsCurve(theNurb,
			12, knots,
			3,
			&ctlpoints[0][0],
			2,
			GL_MAP1_VERTEX_3);
                gluEndCurve(theNurb);
        glPopMatrix();
*/			

glColor3f(0.4, 0.8, 0.4);

glEnableClientState(GL_VERTEX_ARRAY);
glVertexPointer(2,GL_FLOAT,0,ctlpoints);
glDrawArrays(GL_POINTS,0,4);

glEnable(GL_TEXTURE_2D);
glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
glBindTexture(GL_TEXTURE_2D, texName);
	glCallList(lista);

   glBegin(GL_QUADS);
   glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, -1.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 1.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 1.0, 0.0);
   glTexCoord2f(1.0, 0.0); glVertex3f(0.0, -1.0, 0.0);

   glTexCoord2f(0.0, 0.0); glVertex3f(1.0, -1.0, 0.0);
   glTexCoord2f(0.0, 1.0); glVertex3f(1.0, 1.0, 0.0);
   glTexCoord2f(1.0, 1.0); glVertex3f(2.41421, 1.0, -1.41421);
   glTexCoord2f(1.0, 0.0); glVertex3f(2.41421, -1.0, -1.41421);
   glEnd();
	

glDisable(GL_TEXTURE_2D);
	



/*
	glVertexPointer(2,GL_FLOAT,4*sizeof(float),&pointsVector[0]);
	glColor3f(0.5,0.8,0.8);
	glDrawArrays(GL_POINTS,0,pointsVector.size()/4);
	glColor3f(0.2,0.8,0.8);
	glDrawArrays(GL_LINE_STRIP,0,pointsVector.size()/4);
*/
////Test////

/*
int xx=0;
float *pointsArray;
pointsArray=new float[pointsVector.size()];
for(xx=0;xx<pointsVector.size();xx++){
	pointsArray[xx]=pointsVector[xx];
};
*/	

//glDrawArrays(GL_LINE_LOOP,0,pointsVector.size()/2);

	/*
	glBegin(GL_POINTS);
		glVertex3f(1.0,1.0,1.0);
	glEnd();

*/

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

	  case 'u':
		  view_grid = !view_grid;
		  glutPostRedisplay();
		  break;

	  case 'r':
		  view_axis = !view_axis;
		  glutPostRedisplay();
		  break;

	  case 'e':
		  edit_panel = !edit_panel;
		  glutPostRedisplay();
		  break;

	  case '2':
		  eye[0] = 0.0;
		  eye[1] = 0.0;
		  eye[2] = 15.0;

		  at[0] = 0.0;
		  at[1] = 0.0;
		  at[2] = 0.0;

		  up[0] = 0.0;
		  up[1] = 1.0;
		  up[2] = 0.0;
		  glutPostRedisplay();
		  break;

	  case '3':
		  eye[0] = 6.0;
		  eye[1] = 6.0;
		  eye[2] = 2.0;

		  at[0] = 0.0;
		  at[1] = 0.0;
		  at[2] = 0.0;

		  up[0] = 0.0;
		  up[1] = 0.0;
		  up[2] = 1.0;
		  glutPostRedisplay();
		  break;

	case 'w':
		eye[0]+=1;
		  glutPostRedisplay();
		  break;
	case 's':
		eye[0]-=1;
		  glutPostRedisplay();
		  break;
	case 'a':
		eye[1]-=1;
		  glutPostRedisplay();
		  break;
	case 'd':
		eye[1]+=1;
		  glutPostRedisplay();
		  break;
	case 'z':
		eye[2]-=1;
		  glutPostRedisplay();
		  break;
	case 'x':
		eye[2]+=1;
		  glutPostRedisplay();
		  break;
	case 't':
		at[0]+=1;
		  glutPostRedisplay();
		  break;
	case 'g':
		at[0]-=1;
		  glutPostRedisplay();
		  break;
	case 'f':
		at[1]-=1;
		  glutPostRedisplay();
		  break;
	case 'h':
		at[1]+=1;
		  glutPostRedisplay();
		  break;
	case 'c':
		at[2]-=1;
		  glutPostRedisplay();
		  break;
	case 'v':
		at[2]+=1;
		  glutPostRedisplay();
		  break;
	case 'i':
		up[0]+=1;
		  glutPostRedisplay();
		  break;
	case 'k':
		up[0]-=1;
		  glutPostRedisplay();
		  break;
	case 'j':
		up[1]-=1;
		  glutPostRedisplay();
		  break;
	case 'l':
		up[1]+=1;
		  glutPostRedisplay();
		  break;
	case 'n':
		up[2]-=1;
		  glutPostRedisplay();
		  break;
	case 'm':
		up[2]+=1;
		  glutPostRedisplay();
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
   glutIdleFunc(OnIdle);
   glutMainLoop();
   return 0;
}
