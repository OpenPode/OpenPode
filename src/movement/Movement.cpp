/*
 * Movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "movement/Movement.h"
#include "side/Side.h"
#include "config.h"
#include <cmath>

Movement::Movement(Movement_type type, Movement_direction direction, float distance, float angle, int step_number) :
	m_type(type), m_direction(direction), m_distance(distance), m_corrected_distance(0.f),
	m_sequence_number(0), m_step_number(step_number), m_current_step_number(0),
	m_step_distance_x(0.f), m_step_distance_y(0.f), m_step_distance_z(0.f),
	m_angle(angle), m_paw_spreading(50), m_paw_spreading_step(NO_MOVEMENT_STEP_DIST)

{

}

//try to go to a position
float Movement::reproach_position(float present, float futur, float p_step_distance)
{
	if((present - futur) <= - p_step_distance)
	{
		return(present + p_step_distance);
	}
	else if((present - futur) >= p_step_distance)
	{
		return(present - p_step_distance);
	}
	else
	{
		return futur;
	}
}

//get up and after down the paw to go to the correct height
float Movement::get_up_paw(float final_height, Paw &paw, float p_step_distance)
{
	float z;

	if(m_current_step_number <= (m_step_number / 2))
	{
		if(paw.m_current_coords.z <= MAX_HEIGHT_GET_UP)
		{
			z = paw.m_current_coords.z + p_step_distance*2.5f;
		}
		else
			z = paw.m_current_coords.z;
	}
	else
	{
		//compute step distance to get a linear movement
		float step_distance_get_up = abs((final_height - paw.m_current_coords.z) / (m_step_number - m_current_step_number));
		z = paw.m_current_coords.z - step_distance_get_up;
	}

	return z;
}

float Movement::just_get_up_paw(Paw &paw, float p_step_distance)
{
	float z;

	if(paw.m_current_coords.z <= MAX_HEIGHT_GET_UP)
		z = paw.m_current_coords.z + p_step_distance*2.5f;
	else
		z = paw.m_current_coords.z;

	return z;
}

float Movement::just_get_down_paw(float final_height, Paw &paw, float p_step_distance)
{
	if((paw.m_current_coords.z - final_height) <= - p_step_distance)
	{
		return(paw.m_current_coords.z + p_step_distance);
	}
	else if((paw.m_current_coords.z - final_height) >= p_step_distance)
	{
		return(paw.m_current_coords.z - p_step_distance);
	}
	else
	{
		return final_height;
	}
}

//must go to the position
float Movement::goto_position(float present, float futur, float nb_step)
{
	//compute step distance to get a linear movement
	if(nb_step > 0)
	{
		float step_distance = abs(present - futur) / nb_step;

		if(futur > present)
		{
			return(present + step_distance);
		}
		else if(futur < present)
		{
			return(present - step_distance);
		}
		else
		{
			return futur;
		}
	}
	else
		return futur;
}

void Movement::set_paw_spreading_step(int step)
{
	m_paw_spreading_step = step;
}

void Movement::compute_z_value_for_standard_paw(Side &side, Incline_coef_t p_incline_coef)
{
	m_paw_position.front[coord_z]   = p_incline_coef.A*(m_paw_position.front[coord_x] + HALF_LENGTH) +
									  p_incline_coef.B*(m_paw_position.front[coord_y] + side.get_side_coef()*HALF_WIDTH_MIN) +
									  p_incline_coef.C;
	m_paw_position.middle[coord_z] =  p_incline_coef.A*m_paw_position.middle[coord_x] +
									  p_incline_coef.B*(m_paw_position.middle[coord_y] + side.get_side_coef()*HALF_WIDTH_MAX) +
									  p_incline_coef.C;
	m_paw_position.back[coord_z]   =  p_incline_coef.A*(m_paw_position.back[coord_x] - HALF_LENGTH) +
									  p_incline_coef.B*(m_paw_position.back[coord_y] + side.get_side_coef()*HALF_WIDTH_MIN) +
									  p_incline_coef.C;
}

void Movement::memorize_parameters(int sequence_number, Incline_coef_t p_incline_coef, float paw_spreading)
{
	m_sequence_number = sequence_number;
	m_incline_coef = p_incline_coef;
	m_paw_spreading = paw_spreading;
}

void Movement::raz_current_step_number()
{
	m_current_step_number = 0;
}

void Movement::update_current_step_number(int p_current_step_number)
{
	m_current_step_number = p_current_step_number;
}

void Movement::update_sequence_number(int p_sequence_number)
{
	m_sequence_number = p_sequence_number;
}

void Movement::increase_current_step_number()
{
	m_current_step_number++;
}
