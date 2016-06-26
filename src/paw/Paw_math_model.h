/*
 * Paw_math_model.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_MATH_MODEL_H_
#define PAW_MATH_MODEL_H_

#include "servo/Servo.h"
#include "utility/Vector.h"

/*struct Coords // deprecated, use Vector3f instead
{
	float x;
	float y;
	float z;
};*/

struct Angles
{
	float theta1;
	float theta2;
	float theta3;
};

class Paw_math_model
{
public:

	Paw_math_model(Side_enum side);

protected:

	Angles compute_angles(Vector3f coords);

	float r4;
	float a2;
	float a3;
};



#endif /* PAW_MATH_MODEL_H_ */
