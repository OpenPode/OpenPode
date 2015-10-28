/*
 * Paw.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw.h"
#include "bcm2835.h"

#include <iostream>
#include <math.h>

Paw::Paw(Side_enum side, Paw_position_enum position) :
	m_side(side), m_position(position),
	m_servo1(side, position, position_coxa),
	m_servo2(side, position, position_femur),
	m_servo3(side, position, position_tibias)
{

}

Angles Paw::move(double x, double y, double z)
{
	Angles servo_angles = compute_angles({x, y, z});

	/*std::cout << servo_angles.theta1*180.0/M_PI << std::endl
			  << servo_angles.theta2*180.0/M_PI << std::endl
			  << servo_angles.theta3*180.0/M_PI << std::endl;*/

	/*m_servo1.move(servo_angles.theta1);
	m_servo2.move(servo_angles.theta2);
	m_servo3.move(servo_angles.theta3);*/

	return servo_angles;
}


