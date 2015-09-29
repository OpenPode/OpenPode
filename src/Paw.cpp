/*
 * Paw.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw.h"

#include <iostream>

Paw::Paw(Side side) : m_side(side), m_servo1(1), m_servo2(2), m_servo3(3)
{

}

void Paw::move(double x, double y, double z)
{
	Angles servo_angles = compute_angles({x, y, z});

	/*std::cout << servo_angles.theta1 << std::endl
			  << servo_angles.theta2 << std::endl
			  << servo_angles.theta3 << std::endl;*/

	m_servo1.move(servo_angles.theta1);
	m_servo2.move(servo_angles.theta2);
	m_servo3.move(servo_angles.theta3);
}


