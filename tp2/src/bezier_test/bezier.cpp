/*
 * bezier.cpp
 *
 *  Created on: 02/06/2011
 *      Author: Matías
 */

#include "bezier.h"

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

void dibujaSupBezier(tVertice* ptosControl, float paso, float angulo, int tramos){
	GLfloat Vertices [24];
	tVertice ptosControlUsados [4];
	int puntosTomados=4;

//	tVertice p0,p1,p2,p3;

//	p0.x=0.0;
//	p0.y=0.0;
//	p1.x=3.0;
//	p1.y=1.0;
//	p2.x=2.0;
//	p2.y=2.0;
//	p3.x=4.0;
//	p3.y=4.0;

//
//	ptosControl[0]=p0;
//	ptosControl[1]=p1;
//	ptosControl[2]=p2;
//	ptosControl[3]=p3;

	angulo=10;
	for(int i=0;i<tramos;i++){
		//Carga puntos del tramo
		ptosControlUsados[0] = ptosControl[0+(i*3)];
		ptosControlUsados[1] = ptosControl[1+(i*3)];
		ptosControlUsados[2] = ptosControl[2+(i*3)];
		ptosControlUsados[3] = ptosControl[3+(i*3)];

	int i=0;
	for(float giro =angulo;giro <= 360;giro+=angulo){
		for(float u=0.0;u<= 1.0 ;u+=paso){
			//Primer punto para dibujar
			tPunto punto0 =calcularPuntoDeBezier(u, ptosControlUsados);
			punto0=rotarPunto((giro-angulo), punto0);
			//Primer punto rotado
			tPunto punto2= rotarPunto(giro, punto0);
			//Guardo los puntos
			Vertices[0+(i*6)]=punto0.x;
			Vertices[1+(i*6)]=punto0.y;
			Vertices[2+(i*6)]=punto0.z;
			Vertices[3+(i*6)]=punto2.x;
			Vertices[4+(i*6)]=punto2.y;
			Vertices[5+(i*6)]=punto2.z;
			i++;
			if(i==puntosTomados){
				//Graficar
				GLubyte allIndices [12]= {0, 1, 3, 2, 2, 3, 5, 4, 4, 5, 7, 6};
				glEnableClientState(GL_VERTEX_ARRAY);
				glVertexPointer(3, GL_FLOAT, 0, Vertices);
				glDrawElements(GL_QUADS, 12, GL_UNSIGNED_BYTE, allIndices);
				glDisableClientState(GL_VERTEX_ARRAY);
				//Mete los ultimos 2 puntos en las primeras posiciones
				Vertices[0]=punto0.x;
				Vertices[1]=punto0.y;
				Vertices[2]=punto0.z;
				Vertices[3]=punto2.x;
				Vertices[4]=punto2.y;
				Vertices[5]=punto2.z;

				i=1;
				puntosTomados=3;
			}
		}
		i=0;
		puntosTomados=4;

	}
	}
}




