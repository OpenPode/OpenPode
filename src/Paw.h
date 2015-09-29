/*
 * Paw.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_H_
#define PAW_H_

#include "Paw_math_model.h"
#include "Servo.h"

enum class Side
{
	left,
	right
};

class Paw : public Paw_math_model
{
public:

	Paw(Side side);

	void move(double x, double y, double z);

protected:

	Side m_side;

	Servo m_servo1;
	Servo m_servo2;
	Servo m_servo3;
};


#endif /* PAW_H_ */
