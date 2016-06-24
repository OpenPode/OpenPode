/*
 * Paw.cpp
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#include "paw/Paw.h"
#include "hexapode/hexapode_dimensions.h"
#include "config.h"
#include "drivers/broadcom/bcm2835.h"
#include "error_management/Error_detection.h"
#include "utility/math_utils.h"
#include <iostream>
#include <math.h>

Paw::Paw(Side_enum side, Paw_position_enum position, Error_detection* p_error_detection, float p_x_offset, float p_y_offset, int p_active_sequence_number) :
	Paw_math_model(side),
	m_side(side), m_position(position),
	m_coxa(side, position, position_coxa),
	m_femur(side, position, position_femur),
	m_tibia(side, position, position_tibia),
	m_active_sequence_number(p_active_sequence_number),
	m_error_detection(p_error_detection)
{
	//init paws at default position
	coords.m_prepare_coords = {0.f,0.f,0.f};
	m_position_on_hexapode.x_offset = p_x_offset;
	m_position_on_hexapode.y_offset = p_y_offset;

	if(side == Side_enum::side_left)
		m_side_coef = +1;
	else
		m_side_coef = -1;

	if(position == Paw_position_enum::position_front)
		m_x_center = DEFAULT_X_CENTER_FRONT;
	else if(position == Paw_position_enum::position_middle)
		m_x_center = DEFAULT_X_CENTER_MIDDLE;
	else if(position == Paw_position_enum::position_back)
		m_x_center = DEFAULT_X_CENTER_BACK;

	coords.m_current_coords = {m_x_center, m_side_coef*DEFAULT_Y, DEFAULT_Z};
	coords.m_last_coords = coords.m_current_coords;
}

void Paw::prepare_to_move(float position[3])
{
	coords.m_prepare_coords = {position[coord_x], position[coord_y], position[coord_z]};

	m_servo_angles = compute_angles({position[coord_x], position[coord_y], position[coord_z]});
	//use Paw_math_model to get servos angles
	determine_servos_paw_time();

	m_error_detection->set_paw(*this);
}

void Paw::valid_move()
{
	coords.m_last_coords = coords.m_current_coords;
	coords.m_current_coords = coords.m_prepare_coords;
}

Coords Paw::get_current_position() const
{
	return coords.m_current_coords;
}

Coords Paw::get_last_position() const
{
	return coords.m_last_coords;
}

void Paw::determine_servos_paw_time()
{
	servos_time_table[position_tibia] = (int)(- m_side_coef * (util::to_deg(m_servo_angles.theta3)+90.f) * Servo::resolution + m_tibia.get_offset());
	servos_time_table[position_femur] = (int)(  m_side_coef * (util::to_deg(m_servo_angles.theta2)) * Servo::resolution      + m_femur.get_offset());
	servos_time_table[position_coxa]  = (int)(-(util::to_deg(m_servo_angles.theta1) - m_side_coef*90.f) * Servo::resolution  + m_coxa.get_offset());
}
