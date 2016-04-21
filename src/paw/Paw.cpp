/*
 * Paw.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "Paw.h"
#include "hexapode_dimensions.h"
#include "config.h"
#include "bcm2835.h"

#include <iostream>
#include <math.h>

Paw::Paw(Side_enum side, Paw_position_enum position) :
	Paw_math_model(side),
	m_side(side), m_position(position),
	m_coxa(side, position, position_coxa),
	m_femur(side, position, position_femur),
	m_tibia(side, position, position_tibia)
{
	//init paws at default position
	if(side == Side_enum::side_left)
	{
		if(position == Paw_position_enum::position_front)
		{
			m_current_coords = {DEFAULT_X_CENTER_FRONT, DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_FRONT;
		}
		else if(position == Paw_position_enum::position_middle)
		{
			m_current_coords = {DEFAULT_X_CENTER_MIDDLE, DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_MIDDLE;
		}
		else if(position == Paw_position_enum::position_back)
		{
			m_current_coords = {DEFAULT_X_CENTER_BACK, DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_BACK;
		}
	}
	else
	{
		if(position == Paw_position_enum::position_front)
		{
			m_current_coords = {DEFAULT_X_CENTER_FRONT, -DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_FRONT;
		}
		else if(position == Paw_position_enum::position_middle)
		{
			m_current_coords = {DEFAULT_X_CENTER_MIDDLE, -DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_MIDDLE;
		}
		else if(position == Paw_position_enum::position_back)
		{
			m_current_coords = {DEFAULT_X_CENTER_BACK, -DEFAULT_Y, DEFAULT_Z};
			m_x_center = DEFAULT_X_CENTER_BACK;
		}
	}
}

Angles Paw::move(double x, double y, double z)
{
	Angles servo_angles = compute_angles({x, y, z});
	//use Paw_math_model to get servos angles

	//TODO: compute invalid positions

	m_current_coords = {x, y, z};

	return servo_angles;
}

Coords Paw::get_current_position() const
{
	return m_current_coords;
}

bool Paw::is_position_available(int tibias_value, int femur_value, int coxa_value)
{
	if(!m_tibia.is_value_in_the_range(tibias_value))
		return false;
	else if(!m_femur.is_value_in_the_range(femur_value))
		return false;
	else if(!m_coxa.is_value_in_the_range(coxa_value))
		return false;
	else
		return true;
}
