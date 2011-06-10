#include <GL/glut.h>
#include <vector>
#include <deque>
#define checkImageWidth 64
#define checkImageHeight 64

using namespace std;

extern vector<float> ctlVectorSpline;
extern deque<GLuint> solidsList;
extern GLuint dl_curvaSpline;
extern GLuint dl_curvaBezier;
extern GLuint texName;
extern GLubyte checkImage[checkImageHeight][checkImageWidth][4];
extern int rotation = 0;
extern bool animation = true;
extern float currentPosition = 0;
extern int target = 0;
