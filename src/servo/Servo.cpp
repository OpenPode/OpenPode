/*
 * Servo.cpp
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#include "Servo.h"
#include "config.h"

Servo::Servo(Side_enum side, Paw_position_enum paw_position, Servo_position_enum servo_position) :
	m_side(side), m_paw_position(paw_position), m_position(servo_position)
{
	m_offset = OFFSET_TABLE[side][paw_position][servo_position];
}

int Servo::get_offset() const
{
	return m_offset;
}

bool Servo::is_value_in_the_range(int value)
{
	if((value <= max_ratio) and (value >= min_ratio))
		return true;
	else
		return false;
}
