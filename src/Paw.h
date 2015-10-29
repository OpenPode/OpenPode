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

#include "PCA9685.h"

class Paw : public Paw_math_model
{
public:

	Paw(Side_enum side, Paw_position_enum paw_position);

	Angles move(double x, double y, double z);

	Coords get_current_position() const;

public:

	Side_enum m_side;
	Paw_position_enum m_position;

	Servo m_servo1;
	Servo m_servo2;
	Servo m_servo3;

	Coords m_current_coords;
};


#endif /* PAW_H_ */
