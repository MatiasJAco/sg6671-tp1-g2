//#include <GL/gl.h>
//#include <GL/glu.h>
#define GLUT_DISABLE_ATEXIT_HACK
#include <GL/glut.h>
#include <math.h>
#include "spline_test/spline.h"
#include "bezier_test/bezier.h"
#include "globals.h"
#include <cstdio>

//Testing 3D:
#define SSOLID_H 1
GLuint dl_3D;

GLfloat window_size[2];
#define W_WIDTH window_size[0]
#define W_HEIGHT window_size[1]

int textureRepeatX=1;
int textureRepeatY=1;

//Algunas variables globales usadas para bezier
int puntos = 0;
bool primera=false;
vector <tVertice> controlBezier; // puntos de control Bezier
 int contControl = 0; //Contadores
 int cuentaTramos = 0;
GLfloat ctrlpointsBez[4][3];

//Puntos de control de la spline.
//vector<float> ctlVectorSpline;


// Variables que controlan la ubicaci�n de la c�mara en la Escena 3D
float eye[3] = {3.0,  -1.2, 2};
float at[3]  = { 0.0,  0.0, 0.0};
float up[3]  = { 0.0,  0.0, 1.0};

// Variables asociadas a �nica fuente de luz de la escena
float light_color[4] = {1.0f, 1.0f, 1.0f, 1.0f};
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

// Tama�o de la ventana


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
         if ((i&0x8)==0){
                if ((j&0x8)==0){
                 checkImage[i][j][0] = (GLubyte) 0;
                 checkImage[i][j][1] = (GLubyte) 0;
                 checkImage[i][j][2] = (GLubyte) 0;
                 checkImage[i][j][3] = (GLubyte) 255;
      }
        else{
         checkImage[i][j][0] = (GLubyte) 0;
         checkImage[i][j][1] = (GLubyte) 0;
         checkImage[i][j][2] = (GLubyte) 255;
         checkImage[i][j][3] = (GLubyte) 255;
      }
        }
        else{
        if ((j&0x8)==0){
         checkImage[i][j][0] = (GLubyte) 255;
         checkImage[i][j][1] = (GLubyte) 0;
         checkImage[i][j][2] = (GLubyte) 0;
         checkImage[i][j][3] = (GLubyte) 255;
   }
        else{
         checkImage[i][j][0] = (GLubyte) 255;
         checkImage[i][j][1] = (GLubyte) 255;
         checkImage[i][j][2] = (GLubyte) 255;
         checkImage[i][j][3] = (GLubyte) 255;
}
}
}
}
}
/*void OnIdle (void)
{
	rotate += 0.1;
	if(rotate > 360.0) rotate_sphere = 0.0;

	glutPostRedisplay();
}
*/
void Avanzar(int value) {

	float displacement_error;
	

	rotation = (rotation + ROTATION_STEP) % 360;
	displacement_error=at[1]-target*SOLIDS_SEPARATION;
	if(fabsf(displacement_error)>DISPLACEMENT_TOLERANCE)
		if (displacement_error < 0){
			at[1]+=DISPLACEMENT_STEP;
			eye[1]+=DISPLACEMENT_STEP;
		}
		else{
			at[1]-=DISPLACEMENT_STEP;
			eye[1]-=DISPLACEMENT_STEP;
		}

	if (animation == true)
		glutTimerFunc(MS,Avanzar,1);

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
	if(!controlBezier.empty()){
    if(contControl >3 ||(primera && contControl>2)){
    	cuentaTramos++;
    	contControl=0;
    	primera=true;
    }
    	for(int t=0;t<cuentaTramos;t++){
    		for (int j=0;j<4; j++){
    			ctrlpointsBez[j][0]=controlBezier[(j+t*3)].x;
    			ctrlpointsBez[j][1]=controlBezier[(j+t*3)].y;
    			ctrlpointsBez[j][2]=0.0;
    		}
    	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &ctrlpointsBez[0][0]);
    	glMapGrid1f(600, 0.0, 1.0);
    	glEnable(GL_MAP1_VERTEX_3);
    	glColor3f(1.0, 1.0, 0.0);
    	glEvalMesh1(GL_LINE, 0, 600);
    /* Dibuja puntos de control*/
    	glPointSize(5.0);

    	glBegin(GL_POINTS);

    		glColor3f(0.0, 1.0, 0.0);
    		glVertex3f(controlBezier[0+t*3].x,controlBezier[0+t*3].y,0);
    		glColor3f(1.0, 0.0, 0.0);
    		glVertex3f(controlBezier[1+t*3].x,controlBezier[1+t*3].y,0);
    		glVertex3f(controlBezier[2+t*3].x,controlBezier[2+t*3].y,0);
    		glColor3f(0.0, 1.0, 0.0);
    		glVertex3f(controlBezier[3+t*3].x,controlBezier[3+t*3].y,0);
    	glEnd();
    	};
	};


}

void drawSolidRevolution(){
	glEnable(GL_COLOR_MATERIAL);
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
//        glDisable(GL_LIGHTING);
	glColor3f(1.0,1.0,0.5);
	glPushMatrix();
//	        glScalef(2.0,2.0,2.0);
        	dibujaSupBezier(&controlBezier[0],0.125,10,cuentaTramos);
	glPopMatrix();  
//	glEnable(GL_LIGHTING);
}

void drawSolidSweep(vector<float> & ctlVector){

	vector<float> pointsVector;
	float * coordsArray;
	size_t i;
	float step=0.1;
	vector<float> ultimoPunto; //xcoord,ycoord
	vector<float> penultimoPunto; //xcoord,ycoord
	float textureStep;

	
	pointsVector=calcularPuntosSplineEquiespaciados2D(ctlVector,step);
	coordsArray=new float[pointsVector.size()/4*16];//3Coord,3Norm,2Text

	//coeficiente de texturas:
	ultimoPunto.push_back(pointsVector[pointsVector.size()-4]);
	ultimoPunto.push_back(pointsVector[pointsVector.size()-3]);
	penultimoPunto.push_back(pointsVector[pointsVector.size()-8]);
	penultimoPunto.push_back(pointsVector[pointsVector.size()-7]);

	textureStep=step*textureRepeatX/((pointsVector.size()/4 -2) * step + distancia2D(ultimoPunto,penultimoPunto));

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
		coordsArray[i*16+6]=i*textureStep;
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
		coordsArray[i*16+14]=i*textureStep;
		coordsArray[i*16+15]=textureRepeatY;
		
	}
	//Corrijo la ultima textura:
	--i;
	coordsArray[i*16+6]=textureRepeatX;
	coordsArray[i*16+14]=textureRepeatX;
	

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
		glDrawArrays(GL_TRIANGLE_STRIP, 0, pointsVector.size()/4*2);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	delete coordsArray;

}

void dibujaSupBezier(tVertice* ptosControl, float paso, float angulo, int tramos){
	vector<float> Vertices ;
	float* coordsArray;
	tVertice ptosControlUsados [4];

	tPunto planoNormal;
	planoNormal.x = 1;
	planoNormal.y = 0;
	planoNormal.z = 0;
	angulo=10;
	float textureStepX = (angulo/360)*textureRepeatX;
	float textureStepY = (paso/tramos)*textureRepeatY;
	float posTexturestepX = 0;

	for(float giro =angulo;giro <= 360;giro+=angulo){
	for(int j=0;j<tramos;j++){
		//Carga puntos del tramo
		ptosControlUsados[0] = ptosControl[0+(j*3)];
		ptosControlUsados[1] = ptosControl[1+(j*3)];
		ptosControlUsados[2] = ptosControl[2+(j*3)];
		ptosControlUsados[3] = ptosControl[3+(j*3)];

		for(float u=0.0;u<= 1.0 ;u+=paso){
			//Primer punto para dibujar
			tPunto punto0 =calcularPuntoDeBezier(u, ptosControlUsados);
			punto0=rotarPunto((giro-angulo), punto0);
			//Calculo normal del punto
			tPunto normal0 = calcularNormal(u,ptosControlUsados,planoNormal);
			normal0=rotarPunto((giro-angulo), normal0);
			//Primer punto rotado
			tPunto punto2 = rotarPunto(giro, punto0);
			tPunto normal2 = rotarPunto(giro, normal0);
			//Guardo los puntos

			Vertices.push_back(punto0.x);
			Vertices.push_back(punto0.y);
			Vertices.push_back(punto0.z);
			Vertices.push_back(normal0.x);
			Vertices.push_back(normal0.y);
			Vertices.push_back(normal0.z);
			Vertices.push_back(punto2.x);
			Vertices.push_back(punto2.y);
			Vertices.push_back(punto2.z);
			Vertices.push_back(normal2.x);
			Vertices.push_back(normal2.y);
			Vertices.push_back(normal2.z);
		}
	}
	//Graficar
	coordsArray=new float[(Vertices.size()/6)*8];//3Coord,3Norm,2Text
	for(size_t i=0;i<Vertices.size()/12;i++){
		//Punto
		coordsArray[0+(i*16)] = Vertices[0+(i*12)];
		coordsArray[1+(i*16)] = Vertices[1+(i*12)];
		coordsArray[2+(i*16)] = Vertices[2+(i*12)];
		//Normal
		coordsArray[3+(i*16)] = Vertices[3+(i*12)];
		coordsArray[4+(i*16)] = Vertices[4+(i*12)];
		coordsArray[5+(i*16)] = Vertices[5+(i*12)];
		//Coordenada Textura.
		coordsArray[6+(i*16)] = posTexturestepX;
		coordsArray[7+(i*16)] = i*textureStepY;

		//Punto
		coordsArray[8+(i*16)] = Vertices[6+(i*12)];
		coordsArray[9+(i*16)] = Vertices[7+(i*12)];
		coordsArray[10+(i*16)] = Vertices[8+(i*12)];
		//Normal
		coordsArray[11+(i*16)] = Vertices[9+(i*12)];
		coordsArray[12+(i*16)] = Vertices[10+(i*12)];
		coordsArray[13+(i*16)] = Vertices[11+(i*12)];
		//Coordenada Textura.
		coordsArray[14+(i*16)] = posTexturestepX + textureStepX;
		coordsArray[15+(i*16)] = i*textureStepY;
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
//						glTranslatef(-0.5,-0.5,0);
	glDrawArrays(GL_TRIANGLE_STRIP,0,Vertices.size()/6);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

		Vertices.clear();
		posTexturestepX+=textureStepX;
	}
	glDisableClientState(GL_VERTEX_ARRAY);
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
	if (contControl <limit){
		tVertice nuevoPunto;
		nuevoPunto.x = convCoorXPanelB(x);
		nuevoPunto.y = convCoorYPanelB(y);
		controlBezier.push_back(nuevoPunto);
		contControl++;
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
	///
	///Spline 3D
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
			glCallList(dl_3D);
		glPushMatrix();
			for (it=solidsList.begin(), itEnd=solidsList.end(); it!=itEnd; it++){
				
				glPushMatrix();
					glRotatef(rotation,0,0,1);
					glCallList(*it);
				glPopMatrix();

				glTranslatef(0,SOLIDS_SEPARATION,0);
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
		glEnable(GL_COLOR_MATERIAL);
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
		  dl_3D=glGenLists(1);
	  	  if (!ctlVectorSpline.empty()){
		  	solidsList.push_back(glGenLists(1));
		  	glNewList(solidsList.back(), GL_COMPILE);
		  		drawSolidSweep(ctlVectorSpline);
				//normalsTest(ctlVectorSpline);
                  	glEndList();
			ctlVectorSpline.clear();
			drawPanelACurve();
			target=solidsList.size()-1;
		  }
              glutPostRedisplay();
		  break;

	  case '3':

		dl_3D=glGenLists(1);
	
		if ( !controlBezier.empty() ){
			solidsList.push_back(glGenLists(1));
		  	glNewList(solidsList.back(), GL_COMPILE);
		  		drawSolidRevolution();
		  	glEndList();
		  	cuentaTramos = 0;
		  	contControl=0;
		  	primera=false;
			controlBezier.clear();
			curvaBezier();
			target=solidsList.size()-1;
		  }
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
	  case '5':
		  textureRepeatX++;
		  printf("Horizontal : %i\n",textureRepeatX);
		  break;
	  case '6':
		  if(textureRepeatX>1)
	  		  textureRepeatX--;
		  printf("Horizontal : %i\n",textureRepeatX);
		  break;
	  case '7':
		  textureRepeatY++;
		  printf("Vertical : %i\n",textureRepeatY);
		  break;
	  case 'y':
		  if(textureRepeatY>1)
			  textureRepeatY--;
		  printf("Vertical : %i\n",textureRepeatY);
		  break;
	  case '8':
		  if (animation == true)
			  animation=false;
		  else{
			animation=true;
			eye[0] = 3.0;
			eye[1] = -1.2;
			eye[2] = 3.0;
			at[0]  = 0.0;
			at[1]  = 0.0;
			at[2]  = 0.0;
			up[0]  = 0.0;
			up[1]  = 0.0;
			up[2]  = 1.0;
		  }
	  case '9':
		  if(target>0){
			  target--;
		  }
		  break;
	  case '0':
		  if(target<solidsList.size()-1 && !solidsList.empty()){
			  target++;
		  }
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
//   glutIdleFunc(OnIdle);
   glutTimerFunc(MS,Avanzar,1);
   glutMainLoop();
   return 0;
}

