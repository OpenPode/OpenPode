/*
 * Movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "Movement.h"
#include "Side.h"
#include <cmath>

Movement::Movement(Movement_type type, Movement_direction direction, double distance, double angle, int step_number) :
	m_type(type), m_direction(direction), m_distance(distance), m_angle(angle), m_step_number(step_number),
	m_step_distance_x(0), m_step_distance_z(0), m_corrected_distance(0), m_current_step_number(0)
{

}

