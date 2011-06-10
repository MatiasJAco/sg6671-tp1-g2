#include <vector>
using namespace std;

vector<float> calcularPuntosSpline(vector<float> & controlPoints, float step=0.3);

vector<float> calcularPuntosSplineEquiespaciados2D(vector<float> & controlPoints, float paso, float tolerancia=0.02);
vector<float> splinePoint2D(float u, const vector<float> & controlPoints);
vector<float> splineNormal2D(float u, const vector<float> & controlPoints);

float distancia2D(vector<float> & v1, vector<float> & v2);
