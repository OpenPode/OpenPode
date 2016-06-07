/*
 * Paw_math_model.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_MATH_MODEL_H_
#define PAW_MATH_MODEL_H_

#include "servo/Servo.h"

struct Coords
{
	double x;
	double y;
	double z;
};

struct Angles
{
	double theta1;
	double theta2;
	double theta3;
};

class Paw_math_model
{
public:

	Paw_math_model(Side_enum side);

protected:

	Angles compute_angles(Coords coords);

	double r4;
	double a2;
	double a3;
};



#endif /* PAW_MATH_MODEL_H_ */
