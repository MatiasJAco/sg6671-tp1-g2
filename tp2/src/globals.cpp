#include "globals.h"

//Spline
vector<float> ctlVectorSpline;

//Interfaz
deque<GLuint> solidsList;
GLuint dl_curvaSpline;
GLuint dl_curvaBezier;

//Texturas
GLuint texName;
GLubyte checkImage[checkImageHeight][checkImageWidth][4];

//Animacion
int rotation;
bool animation;
float currentPosition;
int target;
