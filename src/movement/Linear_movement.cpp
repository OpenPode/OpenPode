/*
* Linear_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "movement/Linear_movement.h"
#include "side/Side.h"
#include <cmath>

Linear_movement::Linear_movement(Movement_direction direction, float distance, int step_number) :
	Movement(linear, direction, distance, 0.f, step_number)
{

}

void Linear_movement::determine_x_paws_position(Paw &paw)
{
	if(paw.m_active_sequence_number == m_sequence_number)
		m_paw_position[coord_x]  = goto_position(paw.coords.m_current_coords.x,
												 paw.m_x_center + m_direction * m_distance / (float)(m_number_of_sequence-1),
												 m_step_number - m_current_step_number);
	else
		m_paw_position[coord_x]  = paw.coords.m_current_coords.x  - m_direction * m_step_distance.step_distance_x;
}

void Linear_movement::determine_y_paws_position(Paw &paw)
{
	if(paw.m_active_sequence_number == m_sequence_number)
		m_paw_position[coord_y]  = reproach_position(paw.coords.m_current_coords.y,
													 paw.get_side_coef() * m_paw_spreading,
													 m_step_distance.step_distance_z);
	else
		m_paw_position[coord_y] = paw.coords.m_current_coords.y;
}

void Linear_movement::determine_z_paws_position(Paw &paw)
{
	float x_distance  = paw.m_x_center + m_direction * m_distance / (float)(m_number_of_sequence-1);
	float y_distance = paw.get_side_coef() * m_paw_spreading;
	float z_theoretic_value = m_incline_coef.A*(x_distance + paw.m_position_offset.x) +
							  m_incline_coef.B*(y_distance + paw.get_side_coef()*paw.m_position_offset.y) +
							  m_incline_coef.C;

	if(paw.m_active_sequence_number == m_sequence_number)
		m_paw_position[coord_z]  = get_up_paw(z_theoretic_value, paw, m_step_distance.step_distance_z);
	else
		compute_z_value_for_standard_paw(paw, m_incline_coef);
}

float Linear_movement::determine_real_distance(Paw &paw)
{
	float real_distance;

	if(paw.m_active_sequence_number == m_sequence_number)
		real_distance = m_distance/(float)(m_number_of_sequence-1);
	else
		real_distance  = m_direction*(paw.get_current_position().x  - paw.m_x_center)  + m_distance / (float)(m_number_of_sequence-1);

	if(m_direction * real_distance < 0)
		real_distance = 0.f;

	return (real_distance);
}

void Linear_movement::compute_variables()
{
	m_step_distance.step_distance_z = m_distance / (float)(m_number_of_sequence-1) / m_step_number;
	m_step_distance.step_distance_x = m_corrected_distance / m_step_number;
}

float* Linear_movement::determine_paw_position(Paw &paw)
{
	determine_x_paws_position(paw);
	determine_y_paws_position(paw);
	determine_z_paws_position(paw);
	return m_paw_position;
}

bool Linear_movement::is_sequence_finished(Paw &paw)
{
	if(m_current_step_number >= (m_step_number - 1))
		return SEQUENCE_FINISH;
	else
		return SEQUENCE_IN_PROGRESS;
}

