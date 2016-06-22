/*
* complete_linear_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Guillaume
 */

#include "movement/complete_linear_movement.h"
#include "hexapode/hexapode_dimensions.h"
#include "side/Side.h"
#include "utility/math_utils.h"
#include <cmath>

complete_linear_movement::complete_linear_movement(int angle, float distance, int step_number) :
	Movement(complete_linear, direction_front, distance, util::to_rad(angle), step_number)
{
}

void complete_linear_movement::determine_x_paws_position(Paw &paw)
{
	if(paw.m_active_sequence_number == m_sequence_number)
		m_paw_position[coord_x]  = goto_position( paw.m_current_coords.x,
												  paw.m_x_center + cosf(m_angle) * m_distance / (float)(m_number_of_sequence-1),
												  m_step_number - m_current_step_number);
	else
		m_paw_position[coord_x]  = paw.m_current_coords.x - cosf(m_angle) * m_step_distance.step_distance_x;
}

void complete_linear_movement::determine_y_paws_position(Paw &paw)
{
	if(paw.m_active_sequence_number == m_sequence_number)
		m_paw_position[coord_y]  = goto_position(paw.m_current_coords.y,
												 paw.get_side_coef() * m_paw_spreading + sinf(m_angle) * m_distance / (float)(m_number_of_sequence-1),
												 m_step_number - m_current_step_number);
	else
		m_paw_position[coord_y]  = paw.m_current_coords.y - sinf(m_angle) * m_step_distance.step_distance_y;
}

void complete_linear_movement::determine_z_paws_position(Paw &paw)
{
	float x_distance = paw.m_x_center  + cosf(m_angle) * m_distance / (float)(m_number_of_sequence-1);
	float y_distance = paw.get_side_coef() * m_paw_spreading + sinf(m_angle) * m_distance / (float)(m_number_of_sequence-1);
	float z_theoretic_value = m_incline_coef.A*(x_distance + paw.m_position_on_hexapode.x_offset) +
							  m_incline_coef.B*(y_distance + paw.get_side_coef()*paw.m_position_on_hexapode.y_offset) +
							  m_incline_coef.C;

	if(paw.m_active_sequence_number == m_sequence_number)
	{
		m_paw_position[coord_z]  = get_up_paw(z_theoretic_value, paw, m_step_distance.step_distance_z);
	}
	else
		compute_z_value_for_standard_paw(paw, m_incline_coef);
}

float complete_linear_movement::determine_real_distance(Paw &paw)
{
	float real_distance_x;

	float direction = cosf(m_angle) / std::abs(cosf(m_angle)); //cos(m_angle) / abs(cos(m_angle)) give the sign (forward + / backward -)
	real_distance_x = direction * (paw.get_current_position().x  - paw.m_x_center)  + std::abs(cosf(m_angle)) * m_distance / (float)(m_number_of_sequence-1);

	if(paw.m_active_sequence_number == m_sequence_number)
		real_distance_x = std::abs(cosf(m_angle)) * m_distance/(float)(m_number_of_sequence-1);

	if(real_distance_x < 0)
		real_distance_x = 0;

	float real_distance_y;
	direction = sinf(m_angle) / std::abs(sinf(m_angle));// sin(m_angle) / abs(sin(m_angle)) give the sign (left + / right -)
	real_distance_y  = direction * (paw.get_current_position().y - paw.get_side_coef()*m_paw_spreading)  + std::abs(sinf(m_angle)) * m_distance / (float)(m_number_of_sequence-1);

	if(paw.m_active_sequence_number == m_sequence_number)
		real_distance_y = std::abs(sinf(m_angle)) * m_distance/(float)(m_number_of_sequence-1);

	if(real_distance_y < 0)
		real_distance_y = 0;

	if(cosf(m_angle) != 0)
		real_distance_x = std::abs(real_distance_x/cosf(m_angle));
	else
		real_distance_x = m_distance/(float)(m_number_of_sequence-1);

	if(sinf(m_angle) != 0)
		real_distance_y = std::abs(real_distance_y/sinf(m_angle));
	else
		real_distance_y = m_distance/(float)(m_number_of_sequence-1);

	return(std::min(real_distance_x, real_distance_y));
}

float* complete_linear_movement::determine_paw_position(Paw &paw)
{
	determine_x_paws_position(paw);
	determine_y_paws_position(paw);
	determine_z_paws_position(paw);
	return m_paw_position;
}

void complete_linear_movement::compute_variables()
{
	if(m_current_step_number <= m_step_number)
	{
		m_step_distance.step_distance_z = NO_MOVEMENT_STEP_DIST;//m_distance / (float)m_step_number;
		m_step_distance.step_distance_x = abs((m_corrected_distance /(float)(m_number_of_sequence-1)) * cosf(m_angle) / m_step_number);
		m_step_distance.step_distance_y = abs((m_corrected_distance /(float)(m_number_of_sequence-1)) * sinf(m_angle) / m_step_number);
	}
	else
	{
		m_step_distance.step_distance_z = 0.f;
		m_step_distance.step_distance_y = 0.f;
		m_step_distance.step_distance_x = 0.f;
	}
}

bool complete_linear_movement::is_sequence_finished(Paw &paw)
{
	if(m_current_step_number >= (m_step_number - 1))
		return SEQUENCE_FINISH;
	else
		return SEQUENCE_IN_PROGRESS;
}

