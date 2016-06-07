/*
 * Paw_math_model.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw_math_model.h"
#include "hexapode/hexapode_dimensions.h"
#include <cmath>

Paw_math_model::Paw_math_model(Side_enum side)
{
	a2 = FEMUR_LENGTH;
	a3 = TIBIA_LENGTH;

	if(side==Side_enum::side_left)
		r4 = -TIBIA_ORIGIN_OFFSET;
	else
		r4 = TIBIA_ORIGIN_OFFSET;
}

Angles Paw_math_model::compute_angles(Coords coords)
{
	Angles result;

	float eps1 = 1 , eps3 = -1;

	float F1 = coords.x*coords.x + coords.y*coords.y;
	float s1 = ( coords.x*r4 + eps1*coords.y*sqrtf( F1-r4*r4 ) ) / F1;
	float c1 = (-coords.y*r4 + eps1*coords.x*sqrtf( F1-r4*r4 ) ) / F1;

	float F2 = c1*coords.x + s1*coords.y;
	float c3 = ( F2*F2 + coords.z*coords.z - a2*a2 - a3*a3 ) / ( 2.0*a2*a3 );
	float s3 = eps3*sqrtf( 1 - c3*c3 );

	float F3 = a2 + a3*c3;
	float F4 =      a3*s3;
	float c2 = ( F2*F3 + coords.z*F4 ) / ( F2*F2 + coords.z*coords.z );
	float s2 = (-F2*F4 + coords.z*F3 ) / ( F2*F2 + coords.z*coords.z );

	result.theta1 = atan2f(s1,c1);
	result.theta2 = atan2f(s2,c2);
	result.theta3 = atan2f(s3,c3);

	return result;
}
