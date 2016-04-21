/*
* Linear_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "Linear_movement.h"
#include "hexapode_dimensions.h"
#include "Side.h"
#include <cmath>

Linear_movement::Linear_movement(Movement_direction direction, double distance, int step_number) :
	Movement(linear, direction, distance, 0, step_number)
{

}

void Linear_movement::determine_x_paws_position(Side &side, int sequence_number)
{
	m_paw_position.front[coord_x]  = side.get_front_paw().m_current_coords.x  - m_direction * m_step_distance_x;
	m_paw_position.middle[coord_x] = side.get_middle_paw().m_current_coords.x - m_direction * m_step_distance_x;
	m_paw_position.back[coord_x]   = side.get_back_paw().m_current_coords.x   - m_direction * m_step_distance_x;

	if(side.get_side_id() == side_left)
	{
		if(sequence_number == 0)
			m_paw_position.front[coord_x]  = goto_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center + m_direction * m_distance / 2, m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center + m_direction * m_distance / 2, m_step_number - m_current_step_number);
		else
			m_paw_position.back[coord_x]   = goto_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center + m_direction * m_distance / 2, m_step_number - m_current_step_number);
	}
	else
	{
		if(sequence_number == 0)
			m_paw_position.back[coord_x]   = goto_position( side.get_back_paw().m_current_coords.x,
															side.get_back_paw().m_x_center + m_direction * m_distance / 2,
															m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position( side.get_middle_paw().m_current_coords.x,
															side.get_middle_paw().m_x_center + m_direction * m_distance / 2,
															m_step_number - m_current_step_number);
		else
			m_paw_position.front[coord_x]  = goto_position( side.get_front_paw().m_current_coords.x,
															side.get_front_paw().m_x_center + m_direction * m_distance / 2,
															m_step_number - m_current_step_number);
	}
}

void Linear_movement::determine_y_paws_position(Side &side, int sequence_number, double paw_spreading)
{
	m_paw_position.front[coord_y] = side.get_front_paw().m_current_coords.y;
	m_paw_position.middle[coord_y] = side.get_middle_paw().m_current_coords.y;
	m_paw_position.back[coord_y] = side.get_back_paw().m_current_coords.y;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_y]  = reproach_position( side.get_front_paw().m_current_coords.y, side.get_side_coef() * paw_spreading,
																m_step_distance_z);
		else
			m_paw_position.back[coord_y]   = reproach_position( side.get_back_paw().m_current_coords.y, side.get_side_coef() * paw_spreading,
																m_step_distance_z);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_y] = reproach_position( side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading - 0),
																m_step_distance_z);
		else
			m_paw_position.middle[coord_y] = reproach_position( side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading - 0),
																m_step_distance_z);
	}
	else
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = reproach_position( side.get_back_paw().m_current_coords.y, side.get_side_coef() * paw_spreading,
																m_step_distance_z);
		else
			m_paw_position.front[coord_y]  = reproach_position( side.get_front_paw().m_current_coords.y, side.get_side_coef() * paw_spreading,
																m_step_distance_z);
	}
}

void Linear_movement::determine_z_paws_position(Side &side, int sequence_number, double a, double b, double c)
{
	double z_value = 0; // ax + by + c

	m_paw_position.front[coord_z]  = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + b*(m_paw_position.front[coord_y] + side.get_side_coef()*HALF_WIDTH_MIN)  + c;
	m_paw_position.middle[coord_z] = a*m_paw_position.middle[coord_x] 				 + b*(m_paw_position.middle[coord_y] + side.get_side_coef()*HALF_WIDTH_MAX) + c;
	m_paw_position.back[coord_z]   = a*(m_paw_position.back[coord_x] - HALF_LENGTH)  + b*(m_paw_position.back[coord_y] + side.get_side_coef()*HALF_WIDTH_MIN)   + c;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
		{
			z_value = a*(m_paw_position.front[coord_x]+ HALF_LENGTH) + b*(m_paw_position.front[coord_y] + HALF_WIDTH_MIN) + c;
			m_paw_position.front[coord_z]  = get_up_paw(z_value, side.get_front_paw(), m_step_distance_z);
		}
		else
		{
			z_value = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + b*(m_paw_position.back[coord_y] - HALF_WIDTH_MIN) + c;
			m_paw_position.back[coord_z]   = get_up_paw(z_value, side.get_back_paw(), m_step_distance_z);
		}
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
		{
			z_value = a*(m_paw_position.middle[coord_x]) + b*(m_paw_position.middle[coord_y] + HALF_WIDTH_MAX) + c;
			m_paw_position.middle[coord_z] = get_up_paw(z_value, side.get_middle_paw(), m_step_distance_z);
		}
		else
		{
			z_value = a*(m_paw_position.middle[coord_x]) + b*(m_paw_position.middle[coord_y] - HALF_WIDTH_MAX) + c;
			m_paw_position.middle[coord_z] = get_up_paw(z_value, side.get_middle_paw(), m_step_distance_z);
		}
	}
	else
	{
		if(side.get_side_id() == side_left)
		{
			z_value = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + b*(m_paw_position.back[coord_y] + HALF_WIDTH_MIN) + c;
			m_paw_position.back[coord_z]   = get_up_paw(z_value, side.get_back_paw(), m_step_distance_z);
		}
		else
		{
			z_value = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + b*(m_paw_position.front[coord_y] - HALF_WIDTH_MIN) + c;
			m_paw_position.front[coord_z]  = get_up_paw(z_value, side.get_front_paw(), m_step_distance_z);
		}
	}
}

double Linear_movement::determine_real_distance(Side &side)
{
	double real_distance[3];
	real_distance[position_front]  = (side.get_paws_position().front_paw.x - m_direction * (side.get_front_paw().m_x_center - m_distance / 2));
	real_distance[position_middle] = (side.get_paws_position().middle_paw.x - m_direction * (side.get_middle_paw().m_x_center - m_distance / 2));
	real_distance[position_back]   = (side.get_paws_position().back_paw.x - m_direction * (side.get_back_paw().m_x_center - m_distance / 2));

	if(side.get_side_id() == side_left)
	{
		if(side.get_current_sequence_number() == 0)
			real_distance[position_front]  = m_direction * m_distance;
		else if(side.get_current_sequence_number() == 1)
			real_distance[position_middle] = m_direction * m_distance;
		else
			real_distance[position_back]   = m_direction * m_distance;
	}
	else
	{
		if(side.get_current_sequence_number() == 0)
			real_distance[position_back]   = m_direction * m_distance;
		else if(side.get_current_sequence_number() == 1)
			real_distance[position_middle] = m_direction * m_distance;
		else
			real_distance[position_front]  = m_direction * m_distance;
	}

	if(m_direction * real_distance[position_front] < 0)
		real_distance[position_front] = 0;
	if(m_direction * real_distance[position_middle] < 0)
		real_distance[position_middle] = 0;
	if(m_direction * real_distance[position_back] < 0)
		real_distance[position_back] = 0;

	real_distance[position_middle] = std::min(abs(real_distance[position_front]), abs(real_distance[position_middle]));
	return (std::min(abs(real_distance[position_middle]), abs(real_distance[position_back])));
}

void Linear_movement::compute_variables()
{
	m_step_distance_z = m_distance / 2.0 / m_step_number;
	m_step_distance_x = m_corrected_distance / m_step_number;
}

Paw_position Linear_movement::determine_paws_position(Side &side, int sequence_number, double a, double b, double c, double paw_spreading)
{
	determine_x_paws_position(side, sequence_number);
	determine_y_paws_position(side, sequence_number, paw_spreading);
	determine_z_paws_position(side, sequence_number, a, b, c);
	return m_paw_position;
}

bool Linear_movement::is_sequence_finished(Side &side, int sequence_number)
{
	if(m_current_step_number >= (m_step_number - 1))
		return 1;
	else
		return 0;
}

