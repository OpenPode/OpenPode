/*
 * Paw.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw.h"

#include <iostream>

Paw::Paw(Side side) : m_side(side)
{

}

void Paw::move(Coords coords)
{
	Angles servo_angles = compute_angles(coords);

	std::cout << servo_angles.theta1 << std::endl
			  << servo_angles.theta2 << std::endl
			  << servo_angles.theta3 << std::endl;
}


