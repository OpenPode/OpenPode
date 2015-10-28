/*
 * Servo.h
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#ifndef SERVO_H_
#define SERVO_H_

#include "enums.h"

class Servo
{
public:

	Servo(Side_enum side, Paw_position_enum paw_position, Servo_position_enum servo_position);

	static const int min_ratio = 205;
	static const int max_ratio = 441;

	int get_offset() const;

protected:

	Side_enum m_side;
	Paw_position_enum m_paw_position;
	Servo_position_enum m_position;

	int m_offset;
};



#endif /* SERVO_H_ */
