/*
* complete_linear_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Guillaume
 */

#include "movement/complete_linear_movement.h"
#include "hexapode/hexapode_dimensions.h"
#include "side/Side.h"
#include <cmath>

complete_linear_movement::complete_linear_movement(int angle, double distance, int step_number) :
	Movement(complete_linear, direction_front, distance, angle*M_PI/180., step_number)
{
}

void complete_linear_movement::determine_x_paws_position(Side &side)
{
	m_paw_position.front[coord_x]  = side.get_front_paw().m_current_coords.x
									- cos(m_angle) * m_step_distance_x;
	m_paw_position.middle[coord_x] = side.get_middle_paw().m_current_coords.x
									- cos(m_angle) * m_step_distance_x;
	m_paw_position.back[coord_x]   = side.get_back_paw().m_current_coords.x
									- cos(m_angle) * m_step_distance_x;

	if(side.get_side_id() == side_left)
	{
		if(m_sequence_number == 0)
			m_paw_position.front[coord_x]  = goto_position( side.get_front_paw().m_current_coords.x,
															side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(m_sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position( side.get_middle_paw().m_current_coords.x,
															side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(m_sequence_number == 2)
			m_paw_position.back[coord_x]   = goto_position( side.get_back_paw().m_current_coords.x,
															side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else
	{
		if(m_sequence_number == 0)
			m_paw_position.back[coord_x]   = goto_position( side.get_back_paw().m_current_coords.x,
															side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(m_sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position( side.get_middle_paw().m_current_coords.x,
															side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(m_sequence_number == 2)
			m_paw_position.front[coord_x]  = goto_position( side.get_front_paw().m_current_coords.x,
															side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
}

void complete_linear_movement::determine_y_paws_position(Side &side)
{
	m_paw_position.front[coord_y]  = side.get_front_paw().m_current_coords.y
									- sin(m_angle) * m_step_distance_y;
	m_paw_position.middle[coord_y] = side.get_middle_paw().m_current_coords.y
									- sin(m_angle) * m_step_distance_y;
	m_paw_position.back[coord_y]   = side.get_back_paw().m_current_coords.y
									- sin(m_angle) * m_step_distance_y;

	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_y]  = goto_position( side.get_front_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.back[coord_y]   = goto_position( side.get_back_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else if(m_sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_y] = goto_position( side.get_middle_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.middle[coord_y] = goto_position( side.get_middle_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = goto_position( side.get_back_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.front[coord_y]  = goto_position( side.get_front_paw().m_current_coords.y,
															side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
}

void complete_linear_movement::determine_z_paws_position(Side &side)
{
	compute_z_value_for_standard_paw(side, m_incline_coef);

	double x_distance_front  = side.get_front_paw().m_x_center  + cos(m_angle) * m_distance / 2;
	double x_distance_middle = side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2;
	double x_distance_back   = side.get_back_paw().m_x_center   + cos(m_angle) * m_distance / 2;
	double y_distance_all = side.get_side_coef() * m_paw_spreading + sin(m_angle) * m_distance / 2;
	double z_theoretic_front_value = m_incline_coef.A*(x_distance_front + HALF_LENGTH) +
									 m_incline_coef.B*(y_distance_all + side.get_side_coef()*HALF_WIDTH_MIN) +
									 m_incline_coef.C;
	double z_theoretic_middle_value =m_incline_coef.A*(x_distance_middle) +
									 m_incline_coef.B*(y_distance_all + side.get_side_coef()*HALF_WIDTH_MAX) +
									 m_incline_coef.C;
	double z_theoretic_back_value =  m_incline_coef.A*(x_distance_back - HALF_LENGTH) +
									 m_incline_coef.B*(y_distance_all + side.get_side_coef()*HALF_WIDTH_MIN) +
									 m_incline_coef.C;

	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = get_up_paw(z_theoretic_front_value, side.get_front_paw(), m_step_distance_z);
		else
			m_paw_position.back[coord_z]   = get_up_paw(z_theoretic_back_value, side.get_back_paw(), m_step_distance_z);
	}
	else if(m_sequence_number == 1)
	{
		m_paw_position.middle[coord_z] = get_up_paw(z_theoretic_middle_value, side.get_middle_paw(), m_step_distance_z);
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = get_up_paw(z_theoretic_back_value, side.get_back_paw(), m_step_distance_z);
		else
			m_paw_position.front[coord_z]  = get_up_paw(z_theoretic_front_value, side.get_front_paw(), m_step_distance_z);
	}
}

double complete_linear_movement::determine_real_distance(Side &side)
{
	double real_distance_x[3];

	double direction = cos(m_angle) / std::abs(cos(m_angle)); //cos(m_angle) / abs(cos(m_angle)) give the sign (forward + / backward -)
	real_distance_x[position_front]  = direction * (side.get_paws_position().front_paw.x  - side.get_front_paw().m_x_center)  + std::abs(cos(m_angle)) * m_distance / 2;
	real_distance_x[position_middle] = direction * (side.get_paws_position().middle_paw.x - side.get_middle_paw().m_x_center) + std::abs(cos(m_angle)) * m_distance / 2;
	real_distance_x[position_back]   = direction * (side.get_paws_position().back_paw.x   - side.get_back_paw().m_x_center)   + std::abs(cos(m_angle)) * m_distance / 2;

	if(side.get_side_id() == side_left)
	{
		if(m_sequence_number == 0)
			real_distance_x[position_front]  = std::abs(cos(m_angle)) * m_distance/2;
		else if(m_sequence_number == 1)
			real_distance_x[position_middle] = std::abs(cos(m_angle)) * m_distance/2;
		else if(m_sequence_number == 2)
			real_distance_x[position_back]   = std::abs(cos(m_angle)) * m_distance/2;
	}
	else
	{
		if(m_sequence_number == 0)
			real_distance_x[position_back]   = std::abs(cos(m_angle)) * m_distance/2;
		else if(m_sequence_number == 1)
			real_distance_x[position_middle] = std::abs(cos(m_angle)) * m_distance/2;
		else if(m_sequence_number == 2)
			real_distance_x[position_front]  = std::abs(cos(m_angle)) * m_distance/2;
	}

	if(real_distance_x[position_front] < 0)
		real_distance_x[position_front] = 0;
	if(real_distance_x[position_middle] < 0)
		real_distance_x[position_middle] = 0;
	if(real_distance_x[position_back] < 0)
		real_distance_x[position_back] = 0;

	double real_distance_y[3];
	direction = sin(m_angle) / std::abs(sin(m_angle));// sin(m_angle) / abs(sin(m_angle)) give the sign (left + / right -)
	real_distance_y[position_front]  = direction * (side.get_paws_position().front_paw.y  - side.get_side_coef()*m_paw_spreading)  + std::abs(sin(m_angle)) * m_distance / 2;
	real_distance_y[position_middle] = direction * (side.get_paws_position().middle_paw.y - side.get_side_coef()*m_paw_spreading) + std::abs(sin(m_angle)) * m_distance / 2;
	real_distance_y[position_back]   = direction * (side.get_paws_position().back_paw.y   - side.get_side_coef()*m_paw_spreading)   + std::abs(sin(m_angle)) * m_distance / 2;

	if(side.get_side_id() == side_left)
	{
		if(m_sequence_number == 0)
			real_distance_y[position_front]  = std::abs(sin(m_angle)) * m_distance/2;
		else if(m_sequence_number == 1)
			real_distance_y[position_middle] = std::abs(sin(m_angle)) * m_distance/2;
		else if(m_sequence_number == 2)
			real_distance_y[position_back]   = std::abs(sin(m_angle)) * m_distance/2;
	}
	else
	{
		if(m_sequence_number == 0)
			real_distance_y[position_back]   = std::abs(sin(m_angle)) * m_distance/2;
		else if(m_sequence_number == 1)
			real_distance_y[position_middle] = std::abs(sin(m_angle)) * m_distance/2;
		else if(m_sequence_number == 2)
			real_distance_y[position_front]  = std::abs(sin(m_angle)) * m_distance/2;
	}

	if(real_distance_y[position_front] < 0)
		real_distance_x[position_front] = 0;
	if(real_distance_y[position_middle] < 0)
		real_distance_x[position_middle] = 0;
	if(real_distance_y[position_back] < 0)
		real_distance_x[position_back] = 0;

	real_distance_x[position_middle] = std::min(real_distance_x[position_back], real_distance_x[position_middle]);
	real_distance_x[position_front] = std::min(real_distance_x[position_middle], real_distance_x[position_front]);
	if(cos(m_angle) != 0)
		real_distance_x[position_front] = std::abs(real_distance_x[position_front]/cos(m_angle));
	else
		real_distance_x[position_front] = m_distance/2;

	real_distance_y[position_middle] = std::min(real_distance_y[position_back], real_distance_y[position_middle]);
	real_distance_y[position_front] = std::min(real_distance_y[position_middle], real_distance_y[position_front]);
	if(sin(m_angle) != 0)
		real_distance_y[position_front] = std::abs(real_distance_y[position_front]/sin(m_angle));
	else
		real_distance_y[position_front] = m_distance/2;

	return(std::min(real_distance_x[position_front], real_distance_y[position_front]));
}

Paw_position complete_linear_movement::determine_paws_position(Side &side)
{
	determine_x_paws_position(side);
	determine_y_paws_position(side);
	determine_z_paws_position(side);
	return m_paw_position;
}

void complete_linear_movement::compute_variables()
{
	m_step_distance_z = m_distance / 2.0 / (float)m_step_number;
	m_step_distance_x = abs((m_corrected_distance /2) * cos(m_angle) / m_step_number);
	m_step_distance_y = abs((m_corrected_distance /2) * sin(m_angle) / m_step_number);
}

bool complete_linear_movement::is_sequence_finished(Side &side)
{
	if(m_current_step_number >= (m_step_number - 1))
		return 1;
	else
		return 0;
}

