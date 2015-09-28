/*
 * Paw_math_model.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw_math_model.h"
#include <cmath>

Paw_math_model::Paw_math_model() : r4(2.1), a2(7.3), a3(12.8)
{
}

Angles Paw_math_model::compute_angles(Coords coords)
{
	Angles result;

	double F1 = pow(coords.x,2) + pow(coords.y,2);
	double s1 = ( coords.x*r4 + coords.y*sqrt( F1-pow(r4,2) ) ) / F1;
	double c1 = (-coords.y*r4 + coords.x*sqrt( F1-pow(r4,2) ) ) / F1;

	double F2 = c1*coords.x + s1*coords.y;
	double c3 = ( pow(F2,2) + pow(coords.z,2) - pow(a2,2) - pow(a3,2) ) / ( 2.0*a2*a3 );
	double s3 = sqrt( 1 - pow(c3,2) );

	double F3 = a2 + a3*c3;
	double F4 =      a3*s3;
	double c2 = ( F2*F3 + coords.z*F4 ) / ( pow(F2,2) + pow(coords.z,2) );
	double s2 = (-F2*F4 + coords.z*F3 ) / ( pow(F2,2) + pow(coords.z,2) );

	result.theta1 = atan(s1/c1);
	result.theta2 = atan(s2/c2);
	result.theta3 = atan(s3/c3);

	return result;
}




