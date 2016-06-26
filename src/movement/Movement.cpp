/*
 * Movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "movement/Movement.h"
#include "side/Side.h"
#include <cmath>

Movement::Movement(Movement_type type, Movement_direction direction, float distance, float angle, int step_number) :
	m_type(type), m_direction(direction), m_distance(distance), m_corrected_distance(0.f),
	m_sequence_number(0), m_number_of_sequence(1), m_step_number(step_number), m_current_step_number(0),
	m_angle(angle), m_paw_spreading(50), m_paw_spreading_step(NO_MOVEMENT_STEP_DIST),
	in_correction(false)
#ifdef ERROR_ACTION
	,nb_of_solves(0)
#endif
{
	m_step_distance.step_distance_x = 0.f;
	m_step_distance.step_distance_y = 0.f;
	m_step_distance.step_distance_z = 0.f;
}

//try to go to a position
float Movement::reproach_position(float present, float futur, float p_step_distance)
{
	if((present - futur) <= - p_step_distance)
		return(present + p_step_distance);
	else if((present - futur) >= p_step_distance)
		return(present - p_step_distance);
	else
		return futur;
}

//get up and after down the paw to go to the correct height
float Movement::get_up_paw(float final_height, Paw &paw, float p_step_distance)
{
	float z;

	if(m_current_step_number <= (m_step_number / 2))
	{
#ifdef ERROR_ACTION
		if(nb_of_solves == 0)
		{
#endif
			if(paw.get_current_coords().z <= MAX_HEIGHT_GET_UP)
				z = paw.get_current_coords().z + p_step_distance;
			else
				z = paw.get_current_coords().z;
#ifdef ERROR_ACTION
		}
		else if(nb_of_solves == 1)
		{
			if(final_height < paw.get_current_coords().z)
				z = paw.get_current_coords().z;
		}
		else if(nb_of_solves == 2)
		{
			if(final_height < paw.get_current_coords().z - p_step_distance)
				z = paw.get_current_coords().z - p_step_distance;
		}
		else if(nb_of_solves == 3)
		{
			if(final_height < paw.get_current_coords().z - 2*p_step_distance)
				z = paw.get_current_coords().z - 2*p_step_distance;
		}
#endif
	}
	else
	{
		//compute step distance to get a linear movement
		float step_distance_get_up = abs((final_height - paw.get_current_coords().z) / (m_step_number - m_current_step_number));
		z = paw.get_current_coords().z - step_distance_get_up;
	}

	return z;
}

float Movement::just_get_up_paw(Paw &paw, float p_step_distance, float normal_height)
{
	float z;

#ifdef ERROR_ACTION
		if(nb_of_solves == 0)
		{
#endif
			if(paw.get_current_coords().z <= MAX_HEIGHT_GET_UP)
				z = paw.get_current_coords().z + p_step_distance;
			else
				z = paw.get_current_coords().z;
#ifdef ERROR_ACTION
		}
		else if(nb_of_solves == 1)
		{
			if(normal_height < paw.get_current_coords().z)
				z = paw.get_current_coords().z;
		}
		else if(nb_of_solves == 2)
		{
			if(normal_height < paw.get_current_coords().z - p_step_distance)
				z = paw.get_current_coords().z - p_step_distance;
		}
		else if(nb_of_solves == 3)
		{
			if(normal_height < paw.get_current_coords().z - 2*p_step_distance)
				z = paw.get_current_coords().z - 2*p_step_distance;
		}
#endif
	return z;
}

float Movement::just_get_down_paw(float final_height, Paw &paw, float p_step_distance)
{
	if((paw.get_current_coords().z - final_height) <= - p_step_distance)
		return(paw.get_current_coords().z + p_step_distance);
	else if((paw.get_current_coords().z - final_height) >= p_step_distance)
		return(paw.get_current_coords().z - p_step_distance);
	else
		return final_height;
}

//must go to the position
float Movement::goto_position(float present, float futur, float nb_step)
{
	//compute step distance to get a linear movement
	if(nb_step > 0)
	{
		float step_distance = abs(present - futur) / nb_step;

		if(futur > present)
			return(present + step_distance);
		else if(futur < present)
			return(present - step_distance);
		else
			return futur;
	}
	else
		return futur;
}

void Movement::set_paw_spreading_step(int step)
{
	m_paw_spreading_step = step;
}

void Movement::set_number_of_sequence(int p_nb_of_sequence)
{
	m_number_of_sequence = p_nb_of_sequence;
}

void Movement::compute_z_value_for_standard_paw(Paw &paw, Incline_coef_t p_incline_coef)
{
	m_paw_position[coord_z]   = p_incline_coef.A*(m_paw_position[coord_x] + paw.get_position_offset().x) +
								p_incline_coef.B*(m_paw_position[coord_y] + paw.get_side_coef()*paw.get_position_offset().y) +
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

#ifdef ERROR_ACTION
void Movement::set_nb_of_solve(int nb)
{
	nb_of_solves = nb;
}

void Movement::reset_nb_of_solve()
{
	nb_of_solves = 0;
}
#endif
