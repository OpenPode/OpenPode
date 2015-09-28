/*
 * Paw_math_model.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_MATH_MODEL_H_
#define PAW_MATH_MODEL_H_

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

	Paw_math_model();

protected:

	Angles compute_angles(Coords coords);

	const double r4;
	const double a2;
	const double a3;
};



#endif /* PAW_MATH_MODEL_H_ */
