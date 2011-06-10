//#include <GL/gl.h>
//#include <GL/glu.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <math.h>
#include "spline_test/spline.h"
#include "bezier_test/bezier.h"
#include "globals.h"


//Testing 3D:
#define SSOLID_H 1
GLuint dl_3D;
//Algunas variables globales usadas para bezier  TODO:Aprolijar

GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

int puntos = 0;

#define MAXVERTICES 60
bool primera=false;


tVertice atVertices[MAXVERTICES]; // vertices
 int gnContVert = 0; //Contadores
 int cuentaTramos = 0;
 int cuentaControl = 0;

GLfloat ctrlpoints[4][3] = {
        { -4.0, -4.0, 0.0}, { -1.0, 2.0, 0.0},
        {1.0, 3.5, 0.0}, {4.0, 4.0, 0.0}};

//Puntos de control de la spline.
//vector<float> ctlVectorSpline;


// Variables que controlan la ubicación de la cámara en la Escena 3D
float eye[3] = {15.0, 15.0, 5.0};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};

// Variables asociadas a única fuente de luz de la escena
float light_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float light_position[4] = {1.5f, 0.0f, 0.0f,0.0f};
float light_ambient[4] = {0.05f, 0.05f, 0.05f, 1.0f};

// Variables de control
bool view_grid = true;
bool view_axis = true;
bool edit_panelA = true;
bool edit_panelB = true;
bool superficieBezier=false;

// Handle para el control de las Display Lists
GLuint dl_handle;
#define DL_AXIS (dl_handle+0)
#define DL_GRID (dl_handle+1)
#define DL_AXIS2D_TOP (dl_handle+2)
//#define DL_SBEZIER (dl_handle+3)

// Tamaño de la ventana
//TODO:Tratar de obtener los valores automagicamente con glut.

#define TOP_VIEWA_POSX	((int)((float)W_WIDTH-W_HEIGHT*0.40f))
#define TOP_VIEWA_W		((int)((float)W_HEIGHT*0.35f))
#define TOP_VIEWA_POSY	((int)((float)W_HEIGHT*0.60f))
#define TOP_VIEWA_H		((int)((float)W_HEIGHT*0.35f))

#define TOP_VIEWB_POSX	((int)((float)W_WIDTH-W_HEIGHT*0.40f))
#define TOP_VIEWB_W		((int)((float)W_HEIGHT*0.35f))
#define TOP_VIEWB_POSY	((int)((float)W_HEIGHT*0.20f))
#define TOP_VIEWB_H		((int)((float)W_HEIGHT*0.35f))

//TEXTURA

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

void OnIdle (void)
{
//	rotate_sphere += 0.1;
//	if(rotate_sphere > 360.0) rotate_sphere = 0.0;
//XXX:por ahora vuelvo a activar el post redisplay.
//glutPostRedisplay();
}
bool isInRangeA(int x, int y){
	bool result = false;
	if((x >= TOP_VIEWA_POSX)&&(x <= TOP_VIEWA_POSX+TOP_VIEWA_W)&&(y >= TOP_VIEWA_POSY)&&(y <=(TOP_VIEWA_POSY+TOP_VIEWA_H)))
		result=true;
	return result;
}
bool isInRangeB(int x, int y){
	bool result = false;
	if((x >= TOP_VIEWB_POSX)&&(x <= TOP_VIEWB_POSX+TOP_VIEWB_W)&&(y >= TOP_VIEWB_POSY)&&(y <=(TOP_VIEWB_POSY+TOP_VIEWB_H)))
		result=true;
	return result;
}

bool isInRange(int x, int y){
	bool result=false;
	if( isInRangeA(x,y) || isInRangeB(x,y) )
		result = true;
	return result;
}


float convCoorXPanelA(GLint xMouse){
	float valor;
	valor=((float)(xMouse-TOP_VIEWA_POSX))/TOP_VIEWA_W;
	return valor;
}
float convCoorXPanelB(GLint xMouse){
	float valor;
	valor=((float)(xMouse-TOP_VIEWB_POSX))/TOP_VIEWB_W;
	return valor;
}
float convCoorYPanelA(GLint yMouse){
	float valor;
	valor=((float)(yMouse-TOP_VIEWA_POSY))/TOP_VIEWA_H;
	return valor;
}
float convCoorYPanelB(GLint yMouse){
	float valor;
	valor=((float)(yMouse-TOP_VIEWB_POSY))/TOP_VIEWB_H;
	return valor;
}



void curvaBezier(){

    if(gnContVert >3 ||(primera && gnContVert>2)){
    	cuentaTramos++;
    	gnContVert=0;
    	primera=true;
    }
    	for(int t=0;t<cuentaTramos;t++){
    		for (int j=0;j<4; j++){
    			ctrlpoints[j][0]=atVertices[(j+t*3)].x;
    			ctrlpoints[j][1]=atVertices[(j+t*3)].y;
    			ctrlpoints[j][2]=0.0;
    		}
    	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpoints[0][0]);
    	glMapGrid1f(600, 0.0, 1.0);
    	glEnable(GL_MAP1_VERTEX_3);
    	glColor3f(1.0, 1.0, 0.0);
    	glEvalMesh1(GL_LINE, 0, 600);
    /* Dibuja puntos de control*/
    	glPointSize(5.0);
    	glColor3f(1.0, 1.0, 0.0);
    	glBegin(GL_POINTS);
    	for (int i = 0; i < 4; i++)
    		glVertex3f(atVertices[i+t*3].x,atVertices[i+t*3].y,0);
    	glEnd();
    	};



}

void drawSolidSweep(vector<float> & ctlVector){

	vector<float> pointsVector;
	float * coordsArray;
	size_t i;

	
	pointsVector=calcularPuntosSplineEquiespaciados2D(ctlVector,0.1);

	coordsArray=new float[pointsVector.size()/4*16];//3Coord,3Norm,2Text

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
		coordsArray[i*16+10]=SSOLID_H;
		//Normal
		coordsArray[i*16+11]=pointsVector[i*4+2];
		coordsArray[i*16+12]=pointsVector[i*4+3];
		coordsArray[i*16+13]=0;
		
		//TexCoord
		coordsArray[i*16+14]=i/10.0;
		coordsArray[i*16+15]=SSOLID_H;
		
	}
	

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texName);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glVertexPointer(3,GL_FLOAT,8*sizeof(float),coordsArray);
	glNormalPointer(GL_FLOAT,8*sizeof(float),&coordsArray[3]);
	glTexCoordPointer(2,GL_FLOAT,8*sizeof(float),&coordsArray[6]);
		
	glColor3f(1,1,1);
		
	glPushMatrix();
		glTranslatef(-0.5,-0.5,0);	
		glDrawArrays(GL_TRIANGLE_STRIP,0,pointsVector.size()/4*2);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	delete coordsArray;

}


void normalsTest(vector<float> & ctlVector){
        vector<float> base;
	vector<float> normal;
        size_t i;
	glPushMatrix();
	glTranslatef(-0.5,-0.5,0);	
	glBegin(GL_LINES);
	for(i=0;i<ctlVector.size()/2;i++){
		base=splinePoint2D(i,ctlVector);
		normal=splineNormal2D(i,ctlVector);
		
		glVertex2fv(&base[0]);
		glVertex2f(base[0]+normal[0],base[1]+normal[1]);
	}
	glEnd();
	glPopMatrix();
}


void drawPanelACurve(){
	
	vector<float> panelASpline;
	glEnable(GL_POINT_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(5.0);
	glColor3f(0, 0.8, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,&ctlVectorSpline[0]);
		glDrawArrays(GL_POINTS,0,ctlVectorSpline.size()/2);

	panelASpline=calcularPuntosSpline(ctlVectorSpline);
	glColor3f(1, 1, 0.5);
		glVertexPointer(2,GL_FLOAT,0,&panelASpline[0]);
		glDrawArrays(GL_LINE_LOOP,0,panelASpline.size()/2);
}

void puntosDeBezier(int x, int y){
	int limit;
	if(!primera)
		limit=4;
	else
		limit=3;
	if (gnContVert <limit){
		atVertices[cuentaControl].x = convCoorXPanelB(x);
		atVertices[cuentaControl].y = convCoorYPanelB(y);
		gnContVert++;
		cuentaControl++;

	}
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
		glColor3f(0.0, 0.5, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(0.1, 0.1, 0.1);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(1.0, 0.0, 0.0);
		glVertex3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 1.0, 0.0);
	glEnd();

	glEnable(GL_LIGHTING);
}


void DrawXYGrid()
{
	int i;
	glDisable(GL_LIGHTING);
	glColor3f(0.15, 0.1, 0.1);
	glBegin(GL_LINES);
	for(i=-20; i<21; i++)
	{
		glVertex3f(i, -20.0, 0.0);
		glVertex3f(i,  20.0, 0.0);
		glVertex3f(-20.0, i, 0.0);
		glVertex3f( 20.0, i, 0.0);
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

void SetPanelTopEnvA()
{
	glViewport (TOP_VIEWA_POSX, TOP_VIEWA_POSY, (GLsizei) TOP_VIEWA_W, (GLsizei) TOP_VIEWA_H);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
}

void SetPanelTopEnvB()
{
	glViewport (TOP_VIEWB_POSX, TOP_VIEWB_POSY,
		     	(GLsizei) TOP_VIEWB_W, (GLsizei) TOP_VIEWB_H);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
}

void init(void)
{
	//Iniciacion de Textura
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

	
	//Asignacion de las listas:
	dl_handle = glGenLists(3);
	dl_curvaSpline=glGenLists(1);
	dl_curvaBezier=glGenLists(1);
//	dl_3D=glGenLists(1);//XXX:VARIABLE DE PRUEBA



	glClearColor (0.02, 0.02, 0.04, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_ambient);
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
//	glNewList(DL_SBEZIER, GL_COMPILE);
//			DrawSupBezier();
//	glEndList();
}



void display(void)
{
	deque<GLuint>::iterator it;
	deque<GLuint>::iterator itEnd;


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
	///Bezier3D
	if(superficieBezier){
				glEnable(GL_COLOR_MATERIAL);
					glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
		glColor3f(1.0,1.0,0.5);
		glDisable(GL_LIGHTING);
		glScalef(2.0,2.0,2.0);
		dibujaSupBezier(atVertices,0.125,10,cuentaTramos);

		glEnable(GL_LIGHTING);
	}
	///Spline 3D
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
//			glCallList(dl_3D);
		glPushMatrix();
			for (it=solidsList.begin(), itEnd=solidsList.end(); it!=itEnd; it++){
				glCallList(*it);
				glTranslatef(0,2,0);
			}
		glPopMatrix();

	////////////////////////////////////////////////////
	//light:
		glBegin(GL_POINTS);
		glVertex3fv(light_position);
		glEnd();




	///////////////////////////////////////////////////
	// Panel 2D para la vista superior
	if (edit_panelA)
	{
		
		glDisable(GL_COLOR_MATERIAL);
		SetPanelTopEnvA();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (0, 0, 0.5, 0, 0, 0, 0, 1, 0);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
//		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		/*Round Points??*/


		glDisable(GL_LIGHTING);
		glCallList(dl_curvaSpline);
		glEnable(GL_LIGHTING);
		glCallList(DL_AXIS2D_TOP);
	}

	if (edit_panelB)
	{
//		glEnable(GL_COLOR_MATERIAL);
//			glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
		SetPanelTopEnvB();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (0, 0, 0.5, 0, 0, 0, 0, 1, 0);
		glDisable(GL_LIGHTING);
		curvaBezier();
		glEnable(GL_LIGHTING);
		glCallList(DL_AXIS2D_TOP);
	}
	//
	///////////////////////////////////////////////////


	glutSwapBuffers();
}

void reshape (int w, int h)
{
   	W_WIDTH  = w;
	W_HEIGHT = h;
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

        case '4':
		  //XXX:Prueba Spline
		  
//		  dl_3D=glGenLists(1);//XXX:VARIABLE DE PRUEBA
		  if (!ctlVectorSpline.empty()){
		  	solidsList.push_back(glGenLists(1));
		  	glNewList(solidsList.back(), GL_COMPILE);
		  		drawSolidSweep(ctlVectorSpline);
				//normalsTest(ctlVectorSpline);
                  glEndList();
			
			ctlVectorSpline.clear();
			drawPanelACurve();
		  }
              glutPostRedisplay();
		  break;

	  case '3':
		  superficieBezier =! superficieBezier;
		  glutPostRedisplay();
		  break;

	  case '1':
		  edit_panelA = !edit_panelA;
		  glutPostRedisplay();
		  break;
	  case '2':
		  edit_panelB = !edit_panelB;
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


void mousePtPlot (GLint button, GLint action, GLint xMouse, GLint yMouse) {
	int coorY=W_HEIGHT-yMouse;
	if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
			//Si esta en el panel B,almacena puntos para Bezier
			if(isInRangeB(xMouse,coorY)){
				puntosDeBezier(xMouse,coorY);
			}
			else if(isInRangeA(xMouse,coorY)){
				//Cargo el punto;
				ctlVectorSpline.push_back(convCoorXPanelA(xMouse));
				ctlVectorSpline.push_back(convCoorYPanelA(coorY));
				//Rehago la display list
				glNewList(dl_curvaSpline, GL_COMPILE);
					drawPanelACurve();
				glEndList();
			}
			else{/*posible funcion para 3Dviewport*/}
			}
		glutPostRedisplay();
		}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   W_WIDTH=glutGet(GLUT_SCREEN_WIDTH);
   W_HEIGHT=glutGet(GLUT_SCREEN_HEIGHT);
   glutInitWindowSize (W_WIDTH, W_HEIGHT);
   glutInitWindowPosition (0, 0);

   glutCreateWindow (argv[0]);
   glutFullScreen();
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mousePtPlot);
   glutIdleFunc(OnIdle);
   glutMainLoop();
   return 0;
}

