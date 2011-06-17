#define CONTROL_POINT_ELEMENTS 2 //Posible expancion a 3D
#define ITERACIONES_LIMITE 30
#define STEP 0.3
#include "spline.h"
#include <math.h>
#include <iostream>
#include <cstdio>

//Modulo
int mod(int x, int m) {
    int r = x%m;
    return r<0 ? r+m : r;
}

float distancia2D(vector<float> & v1, vector<float> & v2){
	float dx=v1[0]-v2[0];
	float dy=v1[1]-v2[1];
	return sqrtf(dx*dx+dy*dy);
}


//Puntos
float B1(float u){
	return u*u*u/6.0;
}

float B0(float u){
	return (-3*u*u*u + 3*u*u + 3 *u + 1)/6.0;
}

float B_1(float u){
	return (3*u*u*u -6*u*u + 4 ) /6.0;
}

float B_2(float u){
	return (-u*u*u+3*u*u-3*u+1)/6.0;
}


//Normales
float TB1(float u){
	return u*u/2.0;
}

float TB0(float u){
	return (-3*u*u + 2*u + 1)/2.0;
}

float TB_1(float u){
	return (3*u*u -4*u ) /2.0;
}

float TB_2(float u){
	return (-u*u+2*u-1)/2.0;
}

//Spline Uniforme 2D
vector<float> splinePoint2D(float u, const vector<float> & controlPoints){
	vector<float> outputVector;
	//Parte fea donde calculo los indices;
	//Un vector circular solucionaria el problema;
	float coefB1;
	float coefB0;
	float coefB_1;
	float coefB_2;
	float uFmod;

	u = fmodf(u,controlPoints.size()/2);//u es circular. 

	uFmod = u-(int)u; //fmod(u,1.0)

	int index0=((int)u * CONTROL_POINT_ELEMENTS); 
	int index_1=mod((index0-1*CONTROL_POINT_ELEMENTS),controlPoints.size());
	int index_2=mod((index0-2*CONTROL_POINT_ELEMENTS),controlPoints.size());
	int index1=mod((index0+1*CONTROL_POINT_ELEMENTS),controlPoints.size());


	coefB1=B1(uFmod);
	coefB0=B0(uFmod);
	coefB_1=B_1(uFmod);
	coefB_2=B_2(uFmod);

	//Calculo X
	outputVector.push_back(coefB1*controlPoints[index1++]+coefB0*controlPoints[index0++]+coefB_1*controlPoints[index_1++]+coefB_2*controlPoints[index_2++]);
	//Calculo Y
	outputVector.push_back(coefB1*controlPoints[index1++]+coefB0*controlPoints[index0++]+coefB_1*controlPoints[index_1++]+coefB_2*controlPoints[index_2++]);

	return outputVector;
}

//Spline Uniforme 2D
vector<float> splineNormal2D(float u, const vector<float> & controlPoints){
	vector<float> outputVector;
	//Parte fea donde calculo los indices;
	//Un vector circular solucionaria el problema;
	float coefTB1;
	float coefTB0;
	float coefTB_1;
	float coefTB_2;
	float uFmod;
	float tgX;
	float tgY;

	u = fmodf(u,controlPoints.size()/2);//u es circular. 

	uFmod = u-(int)u; //fmod(u,1.0)

	int index0=((int)u * CONTROL_POINT_ELEMENTS); 
	int index_1=mod((index0-1*CONTROL_POINT_ELEMENTS),controlPoints.size());
	int index_2=mod((index0-2*CONTROL_POINT_ELEMENTS),controlPoints.size());
	int index1=mod((index0+1*CONTROL_POINT_ELEMENTS),controlPoints.size());


	coefTB1=TB1(uFmod);
	coefTB0=TB0(uFmod);
	coefTB_1=TB_1(uFmod);
	coefTB_2=TB_2(uFmod);

	//Calculo X
	tgX=coefTB1*controlPoints[index1++]+coefTB0*controlPoints[index0++]+coefTB_1*controlPoints[index_1++]+coefTB_2*controlPoints[index_2++];
	//Calculo Y
	tgY=coefTB1*controlPoints[index1++]+coefTB0*controlPoints[index0++]+coefTB_1*controlPoints[index_1++]+coefTB_2*controlPoints[index_2++];

	outputVector.push_back(-tgY);
	outputVector.push_back(tgX);
	return outputVector;
}

vector<float> calcularPuntosSpline(vector<float> & controlPoints, float step){
	vector<float> outputVector;
	vector<float> auxVector;
	float n=controlPoints.size()/CONTROL_POINT_ELEMENTS;
	float j=0;

	for (j=0;j<n;j+=step){
		auxVector=splinePoint2D(j, controlPoints);
		outputVector.insert( outputVector.end(), auxVector.begin(), auxVector.end() );
	}

	return outputVector;
}



vector<float> calcularPuntosSplineEquiespaciados2D(vector<float> & controlPoints, float paso, float tolerancia){

	bool exitFlag=false;
	vector<float> outputVector;
	float uAux;
	float dAux;

	size_t iteraciones;
	vector<float> auxVector;
	vector<float> cotaInferior; //xcoord,ycoord,u
	vector<float> cotaSuperior; //xcoord,ycoord,u
	vector<float> nodoSiguiente; //xcoord,ycoord,u
	vector<float> ultimoPunto; //xcoord,ycoord,u

	//Inicializo el Algoritmo.
	tolerancia = paso*tolerancia; //tolerancia relativa a absoluta;

	//Calculo el punto 0;
		outputVector=splinePoint2D(0, controlPoints);
		cotaInferior=outputVector;
		cotaInferior.push_back(0);
		ultimoPunto=cotaInferior;	
	
		auxVector=splineNormal2D(uAux, controlPoints);
		outputVector.push_back(auxVector[0]);
		outputVector.push_back(auxVector[1]);

	//Calculo el nodo siguiente;
		nodoSiguiente=splinePoint2D(1, controlPoints);
		nodoSiguiente.push_back(1);
	
	//Arranca el Algoritmo:
	
	//Valido la cota superior:
	
	while (true){
		while (distancia2D(ultimoPunto,nodoSiguiente) < paso ){
			if (nodoSiguiente[2]+1 > controlPoints.size()/2){
				exitFlag=true;
				break; //Condicion de Corte. 
			}
			auxVector=splinePoint2D(nodoSiguiente[2]+1, controlPoints);
			nodoSiguiente[0]=auxVector[0];
			nodoSiguiente[1]=auxVector[1];
			nodoSiguiente[2]+=1;
		}
		if (exitFlag==true){
			break;
		}

		cotaSuperior=nodoSiguiente;
		iteraciones=0;

		//Arranco con la biseccion;
		do{
			uAux=(cotaSuperior[2]+cotaInferior[2])/2;
			auxVector=splinePoint2D(uAux, controlPoints);
			auxVector.push_back(uAux);

			dAux=distancia2D(ultimoPunto,auxVector)-paso;

			if (dAux < 0)
				cotaInferior=auxVector;
			else
				cotaSuperior=auxVector;


		}while( fabsf(dAux) > tolerancia && iteraciones++ < ITERACIONES_LIMITE);

	
		//Guardo los valores.
		outputVector.push_back(auxVector[0]);
		outputVector.push_back(auxVector[1]);



	
		ultimoPunto=auxVector;

		//TODO:Aca Va el calculo de las Normales Antes de Perder a u.
		auxVector=splineNormal2D(uAux, controlPoints);
		outputVector.push_back(auxVector[0]);
		outputVector.push_back(auxVector[1]);
		}
	//Fuerzo a que la interpolación llegue hasta el último nodo.
	//Así garantizo curvas cerradas.

	outputVector.push_back(nodoSiguiente[0]);
	outputVector.push_back(nodoSiguiente[1]);
	
	auxVector=splineNormal2D(uAux, controlPoints);
	outputVector.push_back(auxVector[0]);
	outputVector.push_back(auxVector[1]);

	return outputVector;
	}
