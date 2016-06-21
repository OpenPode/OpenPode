/*
 * Servo.cpp
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#include "Servo.h"

static const int offset_table[2][3][3] =
{
		{// left
				{380, 280, 260}, // front
				{395, 345, 240}, // middle
				{340, 305, 350}  // back
		},
		{// right
				{330, 375, 380}, // front
				{290, 340, 355}, // middle
				{315, 345, 305}  // back
		}     // tib  fem  cox
};

Servo::Servo(Side_enum side, Paw_position_enum paw_position, Servo_position_enum servo_position) :
	m_side(side), m_paw_position(paw_position), m_position(servo_position)
{
	m_offset = offset_table[side][paw_position][servo_position];
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
