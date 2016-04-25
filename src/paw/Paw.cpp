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
	m_prepare_coords = {0,0,0};
	if(side == Side_enum::side_left)
	{
		m_side_coef = +1;
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
		m_side_coef = -1;
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

void Paw::prepare_to_move(double position[3])
{
	m_prepare_coords = {position[coord_x], position[coord_y], position[coord_z]};

	m_servo_angles = compute_angles({position[coord_x], position[coord_y], position[coord_z]});
	//use Paw_math_model to get servos angles
	determine_servos_paw_time();
}

void Paw::valid_move()
{
	m_current_coords = m_prepare_coords;
}

Coords Paw::get_current_position() const
{
	return m_current_coords;
}

bool Paw::test_machenical_stop_limit(int tibias_value, int femur_value, int coxa_value)
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

bool Paw::test_model_limit(int tibias_angle, int femur_angle, int coxa_angle)
{
	if(tibias_angle != tibias_angle)
		return false;
	else if(femur_angle != femur_angle)
		return false;
	else if(coxa_angle != coxa_angle)
		return false;
	else
		return true;
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

void Paw::determine_servos_paw_time()
{
	servos_time_table[position_tibia] = (int)(- m_side_coef * (m_servo_angles.theta3*(180./M_PI)+90.) * Servo::resolution + m_tibia.get_offset());
	servos_time_table[position_femur] = (int)(  m_side_coef *  m_servo_angles.theta2*(180./M_PI) * Servo::resolution      + m_femur.get_offset());
	servos_time_table[position_coxa]  = (int)(-(m_servo_angles.theta1*(180./M_PI) - m_side_coef*90.) * Servo::resolution  + m_coxa.get_offset());
}
