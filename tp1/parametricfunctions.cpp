/*
 * spherefunction.cpp
 *
 *  Created on: 26/03/2011
 *      Author: Matías
 */

#include "parametricfunctions.h"



void SpherePoints(float* coordinates,float ratio, float theta,float phi){
	float initx=0.0;
	float inity=0.0;
	float initz=5.0;
	coordinates[0]=initx+ratio*sinf(theta)*cosf(phi);
	coordinates[1]=inity+ratio*sinf(theta)*sinf(phi);
	coordinates[2]=initz+ratio*cosf(theta);
};

void CylinderPoints(float* coordinates,float ratio, float phi,float coorz){
	float initx=0.0;
	float inity=0.0;

	coordinates[0]=initx+ratio*cosf(phi);
	coordinates[1]=inity+ratio*sinf(phi);
	coordinates[2]=coorz;



};
