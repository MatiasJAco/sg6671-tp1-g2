#include <GL/glut.h>
#include <vector>
#include <deque>
#define checkImageWidth 64
#define checkImageHeight 64
#define MS 100
#define ROTATION_STEP 3
#define SOLIDS_SEPARATION 2.0
#define DISPLACEMENT_STEP SOLIDS_SEPARATION/20.0
#define DISPLACEMENT_TOLERANCE DISPLACEMENT_STEP/2.0

using namespace std;

extern vector<float> ctlVectorSpline;
extern deque<GLuint> solidsList;
extern GLuint dl_curvaSpline;
extern GLuint dl_curvaBezier;
extern GLuint texName;
extern GLubyte checkImage[checkImageHeight][checkImageWidth][4];
extern int rotation;
extern bool animation;
extern int target;
