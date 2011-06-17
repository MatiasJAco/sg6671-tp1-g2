/*
 * bezier.cpp
 *
 *  Created on: 02/06/2011
 *      Author: Matías
 */

#include "bezier.h"
#include "src/globals.h"

tPunto productoCruzado(tPunto a , tPunto b){
	tPunto resultado;
	resultado.x = (a.y * b.z -a.z * b.y);
	resultado.y = (a.z * b.x - a.x * b.z);
	resultado.z = (a.x * b.y - a.y * b.x);
	return resultado;
}


tPunto calcularNormal(float u, tVertice* ptsControl, tPunto planoNormal ){
	//Coeficientes (derivadas de las bases de Berstein)
	float a = (-3)*pow((1-u),2);
	float b = 3*pow((1-u),2) + (-6)*u*(1-u);
	float c = 6*u*(1-u) + (-3)*pow(u,2);
	float d = 3*pow(u,2);
	tVertice ptoCurva;
	tPunto ptoResultante;
	ptoCurva.x=ptsControl[0].x*a+ptsControl[1].x*b+ptsControl[2].x*c+ptsControl[3].x*d;
	ptoCurva.y=ptsControl[0].y*a+ptsControl[1].y*b+ptsControl[2].y*c+ptsControl[3].y*d;
	ptoResultante.x=0.0;
	ptoResultante.y=ptoCurva.x;
	ptoResultante.z=ptoCurva.y;
	tPunto vectNormal = productoCruzado(ptoResultante, planoNormal);
	return vectNormal;

}

tPunto calcularPuntoDeBezier(float u, tVertice* ptsControl){
	//Coeficientes
	float a = pow((1-u),3);
	float b = (pow((1-u),2))*(3*u);
	float c = (1-u)*(3*(pow(u,2)));
	float d = pow(u,3);
	tVertice ptoCurva;
	tPunto ptoResultante;
	ptoCurva.x=ptsControl[0].x*a+ptsControl[1].x*b+ptsControl[2].x*c+ptsControl[3].x*d;
	ptoCurva.y=ptsControl[0].y*a+ptsControl[1].y*b+ptsControl[2].y*c+ptsControl[3].y*d;

	ptoResultante.x=0.0;
	ptoResultante.y=ptoCurva.x;
	ptoResultante.z=ptoCurva.y;
	return ptoResultante;
}

tPunto rotarPunto(float alpha, tPunto p){
	tPunto result;
	alpha=((alpha+90)*M_PI)/180.0;
	float initx=0.0;
	float inity=0.0;
	float ratio=sqrtf(powf(p.x,2)+powf(p.y,2));
	result.x=initx+ratio*cosf(alpha);
	result.y=inity+ratio*sinf(alpha);
	result.z=p.z;

	return result;
}


