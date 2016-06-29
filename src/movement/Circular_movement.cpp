/*
 * Circular_movement.cpp
 *
 *  Created on: 28 juin 2016
 *      Author: Sarthou
 */

#include "movement/Circular_movement.h"
#include "side/Side.h"
#include "utility/math_utils.h"
#include <cmath>

Circular_movement::Circular_movement(float radius, Movement_direction direction, Rotation_side_enum side, float distance, int step_number) :
	Movement(circular, direction, distance, radius, step_number),
	m_internal_radius(0.f), m_internal_angle(0.f)
{
	m_rotation_side = side;
}

void Circular_movement::determine_x_paws_position(Paw &paw)
{
	if(paw.get_active_sequence_number() == m_sequence_number)
		m_paw_position[coord_x]  = goto_position( paw.get_current_coords().x,
											      paw.get_x_center() + m_step_distance.step_distance_x,
												  m_step_number - m_current_step_number);
	else
		m_paw_position[coord_x]  = paw.get_current_coords().x + m_step_distance.step_distance_x;
}

void Circular_movement::determine_y_paws_position(Paw &paw)
{
	if(paw.get_active_sequence_number() == m_sequence_number)
		m_paw_position[coord_y]  = goto_position(paw.get_current_coords().y,
												 paw.get_side_coef() * m_paw_spreading + m_step_distance.step_distance_y,
												 m_step_number - m_current_step_number);
	else
		m_paw_position[coord_y]  = paw.get_current_coords().y + m_step_distance.step_distance_y;
}

void Circular_movement::determine_z_paws_position(Paw &paw)
{
	float x_distance = paw.get_x_center()  + cosf(m_angle) * m_distance / (float)(m_number_of_sequence-1);
	float y_distance = paw.get_side_coef() * m_paw_spreading + sinf(m_angle) * m_distance / (float)(m_number_of_sequence-1);
	float z_theoretic_value = m_incline_coef.A*(x_distance + paw.get_position_offset().x) +
							  m_incline_coef.B*(y_distance + paw.get_side_coef()*paw.get_position_offset().y) +
							  m_incline_coef.C;

	if(paw.get_active_sequence_number() == m_sequence_number)
	{
		m_paw_position[coord_z]  = get_up_paw(z_theoretic_value, paw, m_step_distance.step_distance_z);
	}
	else
		compute_z_value_for_standard_paw(paw, m_incline_coef);
}

float Circular_movement::determine_real_distance(Paw &paw)
{
	return DEFAULT_DISTANCE;
}

float* Circular_movement::determine_paw_position(Paw &paw)
{
	compute_variables(paw);
	determine_x_paws_position(paw);
	determine_y_paws_position(paw);
	determine_z_paws_position(paw);
	return m_paw_position;
}

void Circular_movement::compute_variables(Paw & paw)
{
	if(m_current_step_number <= m_step_number)
	{
		if(paw.get_active_sequence_number() != m_sequence_number)
		{
			float radius;

			//internal_radius
			radius = m_angle;
			if(m_rotation_side == left_rotation)
				radius = -radius;
			float y = radius - (paw.get_side_coef()*(m_paw_spreading + paw.get_position_offset().y));
			float x = paw.get_current_coords().x + paw.get_position_offset().x;
			m_internal_radius  = sqrtf(x*x + y*y);

			//internal_angle
			if(m_rotation_side == right_rotation)
				radius = -(m_angle + HALF_WIDTH_MAX);
			else
				radius = m_angle + HALF_WIDTH_MAX;
			m_internal_angle = m_distance / radius / m_step_number;
			if(m_direction == direction_back)
				m_internal_angle = -m_internal_angle;

			//distance variation on landmark with internal_radius as X axis
			float delta_x = m_internal_radius * sinf(m_internal_angle);
			float delta_y = m_internal_radius * (1.f - cosf(m_internal_angle));

			//angle between radius and internal_radius
			float m_reference_angle = atan2f(x,y);
			//m_reference_angle = paw.get_side_coef()*m_reference_angle;
			//landmark changement
			m_step_distance.step_distance_y = delta_x*sinf(m_reference_angle) + delta_y*cosf(m_reference_angle);
			m_step_distance.step_distance_x = delta_x*cosf(m_reference_angle) - delta_y*sinf(m_reference_angle);
			m_step_distance.step_distance_z = NO_MOVEMENT_STEP_DIST;
		}
		else
		{
			float radius;

			//internal_radius
			radius = m_angle;
			if(m_rotation_side == right_rotation)
				radius = -radius;
			float y = radius - (paw.get_side_coef()*(m_paw_spreading + paw.get_position_offset().y));
			float x = paw.get_x_center() + paw.get_position_offset().x;
			m_internal_radius  = sqrtf(x*x + y*y);

			//internal_angle
			if(m_rotation_side == right_rotation)
				radius = -(m_angle + HALF_WIDTH_MAX);
			else
				radius = m_angle + HALF_WIDTH_MAX;
			m_internal_angle = m_distance/ 2.f / radius;
			if(m_direction == direction_front)
				m_internal_angle = -m_internal_angle;

			//distance variation on landmark with internal_radius as X axis
			float delta_x = m_internal_radius * sinf(m_internal_angle);
			float delta_y = m_internal_radius * (1.f - cosf(m_internal_angle));

			//angle between radius and internal_radius
			float m_reference_angle = atan2f(x,y);

			//landmark changement
			m_step_distance.step_distance_x = delta_y*sinf(m_reference_angle) + delta_x*cosf(m_reference_angle);
			m_step_distance.step_distance_y = delta_y*cosf(m_reference_angle) - delta_x*sinf(m_reference_angle);
			m_step_distance.step_distance_z = NO_MOVEMENT_STEP_DIST;
		}
	}
	else
	{
		m_step_distance.step_distance_z = 0.f;
		m_step_distance.step_distance_y = 0.f;
		m_step_distance.step_distance_x = 0.f;
	}
}

bool Circular_movement::is_sequence_finished(Paw &paw)
{
	if(m_current_step_number >= (m_step_number - 1))
		return SEQUENCE_FINISH;
	else
		return SEQUENCE_IN_PROGRESS;
}
