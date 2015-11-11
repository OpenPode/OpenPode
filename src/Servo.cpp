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
				{345, 275, 255}, // front
				{385, 335, 290}, // middle
				{345, 280, 360}  // back
		},
		{// right
				{285, 365, 410}, // front
				{280, 340, 355}, // middle
				{315, 345, 300}  // back
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
