//#include <GL/gl.h>
//#include <GL/glu.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <math.h>
#include "spline_test/spline.h"
//Algunas variables globales usadas para bezier  TODO:Aprolijar

//XXX:Esto esta mal, Las variables ya estan macreadas mas abajo:  W_WIDTH
GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

GLsizei winWidth = 1024;
GLsizei winHeight = 768;
int puntos = 0;
#define MAXVERTICES 60
bool primera=false;

typedef struct
{
float x;
float y;
}tVertice; // vertices
tVertice atVertices[MAXVERTICES]; // vertices
 int gnContVert = 0; //Contadores

GLfloat ctrlpoints[4][3] = {
        { -4.0, -4.0, 0.0}, { -1.0, 2.0, 0.0},
        {1.0, 3.5, 0.0}, {4.0, 4.0, 0.0}};

//Puntos de control de la spline.
vector<float> ctlVectorSpline;

// Variables que controlan la ubicación de la cámara en la Escena 3D
float eye[3] = {15.0, 15.0, 5.0};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};

// Variables asociadas a única fuente de luz de la escena
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float light_position[3] = {10.0f, 10.0f, 8.0f};
float light_ambient[4] = {0.05f, 0.05f, 0.05f, 1.0f};

// Variables de control
bool view_grid = true;
bool view_axis = true;
bool edit_panelA = true;
bool edit_panelB = true;


// Handle para el control de las Display Lists
GLuint dl_handle;
#define DL_AXIS (dl_handle+0)
#define DL_GRID (dl_handle+1)
#define DL_AXIS2D_TOP (dl_handle+2)

// Tamaño de la ventana
//TODO:Tratar de obtener los valores automagicamente con glut.

//XXX:Cambie para que quede proporcional:
#define TOP_VIEWA_POSX	((int)((float)W_WIDTH-W_HEIGHT*0.40f))
#define TOP_VIEWA_W		((int)((float)W_HEIGHT*0.35f))
#define TOP_VIEWA_POSY	((int)((float)W_HEIGHT*0.60f))
#define TOP_VIEWA_H		((int)((float)W_HEIGHT*0.35f))

#define TOP_VIEWB_POSX	((int)((float)W_WIDTH-W_HEIGHT*0.40f))
#define TOP_VIEWB_W		((int)((float)W_HEIGHT*0.35f))
#define TOP_VIEWB_POSY	((int)((float)W_HEIGHT*0.20f))
#define TOP_VIEWB_H		((int)((float)W_HEIGHT*0.35f))


void OnIdle (void)
{
//	rotate_sphere += 0.1;
//	if(rotate_sphere > 360.0) rotate_sphere = 0.0;
//XXX:por ahora vuelvo a activar el post redisplay.
glutPostRedisplay();
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
    int i;
    if(gnContVert >3 ||(primera && gnContVert>2)){
    	if(!primera){
    		for (int j=0;j<gnContVert; j++){
    			ctrlpoints[j][0]=atVertices[j].x;
    			ctrlpoints[j][1]=atVertices[j].y;
    			ctrlpoints[j][2]=0.0;
    		}
    	} else {
    		for (int j=0;j<gnContVert; j++){
    			ctrlpoints[j+1][0]=atVertices[j].x;
    			ctrlpoints[j+1][1]=atVertices[j].y;
    			ctrlpoints[j+1][2]=0.0;
    		}
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
    	for (i = 0; i < gnContVert; i++)
    		glVertex3f(atVertices[i].x,atVertices[i].y,0);
    	glEnd();
    	gnContVert=0;
    //El primer punto de control de la proxima sera el ultimo de la actual.
    	ctrlpoints[0][0]=ctrlpoints[3][0];
    	ctrlpoints[0][1]=ctrlpoints[3][1];
    	ctrlpoints[0][2]=ctrlpoints[3][2];
    	primera=true;
    }
}

void puntosDeBezier(int x, int y){
	int limit;
	if(!primera)
		limit=4;
	else
		limit=3;
	if (gnContVert <limit){
		atVertices[gnContVert].x = convCoorXPanelB(x);
		atVertices[gnContVert].y = convCoorYPanelB(y);
		gnContVert++;
	}
}

void puntosDeSpline(int x, int y){
	ctlVectorSpline.push_back(convCoorXPanelA(x));
	ctlVectorSpline.push_back(convCoorYPanelA(y));
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
	glViewport (TOP_VIEWB_POSX, TOP_VIEWB_POSY, (GLsizei) TOP_VIEWB_W, (GLsizei) TOP_VIEWB_H); 
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
	gluOrtho2D(0, 1, 0, 1);
}

void init(void) 
{
	dl_handle = glGenLists(3);

	glClearColor (0.02, 0.02, 0.04, 0.0);
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
	// Panel 2D para la vista superior
	if (edit_panelA)
	{
		SetPanelTopEnvA();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt (0, 0, 0.5, 0, 0, 0, 0, 1, 0);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
            glEnable(GL_POINT_SMOOTH);
            glEnable (GL_BLEND);

		glPointSize(10.0);
            glColor3f(0, 0.8, 0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,&ctlVectorSpline[0]);
		glDrawArrays(GL_POINTS,0,ctlVectorSpline.size()/2);


		glCallList(DL_AXIS2D_TOP);
	}

	if (edit_panelB)
	{
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
	  case 'g':
		  view_grid = !view_grid;
		  glutPostRedisplay();
		  break;

	  case 'a':
		  view_axis = !view_axis;
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
				puntosDeSpline(xMouse,coorY);
			}
			else{/*posible funcion para 3Dviewport*/}
			}
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

