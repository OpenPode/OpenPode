/*
* complete_linear_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Guillaume
 */

#include "complete_linear_movement.h"
#include "hexapode_dimensions.h"
#include "Side.h"
#include <cmath>

complete_linear_movement::complete_linear_movement(int angle, double distance, int step_number) :
	Movement(complete_linear, direction_front, distance, angle*M_PI/180., step_number)
{
}

void complete_linear_movement::determine_x_paws_position(Side &side, int sequence_number)
{
	m_paw_position.front[coord_x]  = side.get_paws_position().front_paw.x
									- cos(m_angle) * m_step_distance_x * m_current_step_number;
	m_paw_position.middle[coord_x] = side.get_paws_position().middle_paw.x
									- cos(m_angle) * m_step_distance_x * m_current_step_number;
	m_paw_position.back[coord_x]   = side.get_paws_position().back_paw.x
									- cos(m_angle) * m_step_distance_x * m_current_step_number;

	if(side.get_side_id() == side_left)
	{
		if(sequence_number == 0)
			m_paw_position.front[coord_x]  = goto_position( side.get_front_paw().m_current_coords.x,
															side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position( side.get_middle_paw().m_current_coords.x,
															side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.back[coord_x]   = goto_position( side.get_back_paw().m_current_coords.x,
															side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else
	{
		if(sequence_number == 0)
			m_paw_position.back[coord_x]   = goto_position( side.get_back_paw().m_current_coords.x,
															side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position( side.get_middle_paw().m_current_coords.x,
															side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.front[coord_x]  = goto_position( side.get_front_paw().m_current_coords.x,
															side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
}

void complete_linear_movement::determine_y_paws_position(Side &side, int sequence_number, double paw_spreading)
{
	m_paw_position.front[coord_y]  = side.get_front_paw().m_current_coords.y -  sin(m_angle) * m_step_distance_y;
	m_paw_position.middle[coord_y] = side.get_middle_paw().m_current_coords.y - sin(m_angle) * m_step_distance_y;
	m_paw_position.back[coord_y]   = side.get_back_paw().m_current_coords.y -   sin(m_angle) * m_step_distance_y;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_y]  = goto_position( side.get_front_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.back[coord_y]   = goto_position( side.get_back_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_y] = goto_position( side.get_middle_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.middle[coord_y] = goto_position( side.get_middle_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
	else
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = goto_position( side.get_back_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.front[coord_y]  = goto_position( side.get_front_paw().m_current_coords.y,
															side.get_side_coef() * paw_spreading - sin(m_angle) * m_distance / 2,
															m_step_number - m_current_step_number);
	}
}

void complete_linear_movement::determine_z_paws_position(Side &side, int sequence_number, double front_height, double back_height)
{
	m_paw_position.front[coord_z]  = front_height*(m_paw_position.front[coord_x] + HALF_LENGTH) + back_height;
	m_paw_position.middle[coord_z] = front_height*m_paw_position.middle[coord_x] + back_height;
	m_paw_position.back[coord_z]   = front_height*(m_paw_position.back[coord_x] - HALF_LENGTH) + back_height;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = get_up_paw(front_height*(side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2 + HALF_LENGTH) + back_height,
														side.get_front_paw(), m_step_distance_z);
		else
			m_paw_position.back[coord_z]   = get_up_paw(front_height*(side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2 - HALF_LENGTH) + back_height,
														side.get_back_paw(), m_step_distance_z);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_z] = get_up_paw(front_height*(side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2) + back_height,
														side.get_middle_paw(), m_step_distance_z);
		else
			m_paw_position.middle[coord_z] = get_up_paw(front_height*(side.get_middle_paw().m_x_center + cos(m_angle) * m_distance / 2) + back_height,
														side.get_middle_paw(), m_step_distance_z);
	}
	else
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = get_up_paw(front_height*(side.get_back_paw().m_x_center + cos(m_angle) * m_distance / 2 - HALF_LENGTH) + back_height,
														side.get_back_paw(), m_step_distance_z);
		else
			m_paw_position.front[coord_z]  = get_up_paw(front_height*(side.get_front_paw().m_x_center + cos(m_angle) * m_distance / 2 + HALF_LENGTH) + back_height,
														side.get_front_paw(), m_step_distance_z);
	}
}

double complete_linear_movement::determine_real_distance(Side &side)
{
	double real_distance[3];
	real_distance[position_front]  = (side.get_paws_position().front_paw.x - cos(m_angle) * (side.get_front_paw().m_x_center - m_distance / 2));
	real_distance[position_middle] = (side.get_paws_position().middle_paw.x - cos(m_angle) * (side.get_middle_paw().m_x_center - m_distance / 2));
	real_distance[position_back]   = (side.get_paws_position().back_paw.x - cos(m_angle) * (side.get_back_paw().m_x_center - m_distance / 2));

	if(side.get_side_id() == side_left)
	{
		if(side.get_current_sequence_number() == 0)
			real_distance[position_front]  = cos(m_angle) * m_distance;
		else if(side.get_current_sequence_number() == 1)
			real_distance[position_middle] = cos(m_angle) * m_distance;
		else
			real_distance[position_back]   = cos(m_angle) * m_distance;
	}
	else
	{
		if(side.get_current_sequence_number() == 0)
			real_distance[position_back]   = cos(m_angle) * m_distance;
		else if(side.get_current_sequence_number() == 1)
			real_distance[position_middle] = cos(m_angle) * m_distance;
		else
			real_distance[position_front]  = cos(m_angle) * m_distance;
	}

	if(cos(m_angle) * real_distance[position_front] < 0)
		real_distance[position_front] = 0;
	if(cos(m_angle) * real_distance[position_middle] < 0)
		real_distance[position_middle] = 0;
	if(cos(m_angle) * real_distance[position_back] < 0)
		real_distance[position_back] = 0;

	real_distance[position_middle] = std::min(abs(real_distance[position_front]), abs(real_distance[position_middle]));
	//return (std::min(abs(real_distance[position_middle]), abs(real_distance[position_back])));
	return abs(m_distance);
}

Paw_position complete_linear_movement::determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading)
{
	determine_x_paws_position(side, sequence_number);
	determine_y_paws_position(side, sequence_number, paw_spreading);
	determine_z_paws_position(side, sequence_number, front_height, back_height);
	return m_paw_position;
}

void complete_linear_movement::compute_variables()
{
	m_step_distance_z = m_distance / 2.0 / m_step_number;
	m_step_distance_x = abs((m_corrected_distance / 2.0) * cos(m_angle) / m_step_number);
	m_step_distance_y = abs((m_corrected_distance / 2.0) * sin(m_angle) / m_step_number);
	cout << m_step_distance_x << " : " << m_step_distance_y << endl;
}

