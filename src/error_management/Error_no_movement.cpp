/*
 * Error_no_movement.cpp
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#include "Error_actions.h"
#include <cmath>

void Error_actions::action_no_movement_no_changement()
{
	if(m_current_step == wait)
		m_current_step = cancel_incline;

	switch(m_current_step)
	{
	case cancel_incline:
	{
		m_new_parameters.incline_values.pitch = 0.f;
		m_new_parameters.incline_values.roll = 0.f;
		m_new_parameters.center_height = m_purpose_parameters.center_height;
		m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;
		m_current_step = reduce_incline;
		find_solution = 0;
	}
	break;
	case reduce_incline:
	{
		action_no_movement_reduce_incline();
	}
	break;
	case find_paw_spreading_stable_direction:
	{
		action_no_movement_find_paw_spreadind_direction();
	}
	break;
	case find_height_stable_direction:
	{
		action_no_movement_find_height_direction();
	}
	break;
	case get_closer_stable_parameters:
	{
		action_no_movement_get_closer_stable_parameters();
	}
	break;
	default:
	{
		m_current_step = wait;
		m_resolving = false;
		m_finished_to_corrected_error = true;
	}
	}
}

void Error_actions::action_no_movement_reduce_incline()
{
	if(((find_solution == 0) && (m_on_error)) || ((m_purpose_parameters.incline_values.pitch == 0) && (m_purpose_parameters.incline_values.roll == 0)))
	{
		m_current_step = find_paw_spreading_stable_direction;
		m_new_parameters = m_purpose_parameters;
	}
	else if((find_solution == 0) && (!m_on_error))
	{
		dichotomie_pitch.B = 0.f;
		dichotomie_pitch.A = m_purpose_parameters.incline_values.pitch;
		dichotomie_pitch.C = dichotomie_pitch.B + (dichotomie_pitch.A - dichotomie_pitch.B)/2.;
		dichotomie_pitch.last_avaible = 0.f;
		dichotomie_roll.B = 0.f;
		dichotomie_roll.A = m_purpose_parameters.incline_values.roll;
		dichotomie_roll.C = dichotomie_roll.B + (dichotomie_roll.A - dichotomie_roll.B)/2.;
		dichotomie_roll.last_avaible = 0.f;
		m_new_parameters.incline_values.pitch = dichotomie_pitch.C;
		m_new_parameters.incline_values.roll = dichotomie_roll.C;
		find_solution ++;
	}
	else if(find_solution > 1000) //security for round error
	{
		m_new_parameters.incline_values.pitch = dichotomie_pitch.last_avaible;
		m_new_parameters.incline_values.roll = dichotomie_roll.last_avaible;
		set_end_of_solving();
	}
	else if((find_solution >= 12) && (!m_on_error))
	{
		m_new_parameters.incline_values.pitch = dichotomie_pitch.C;
		m_new_parameters.incline_values.roll = dichotomie_roll.C;
		set_end_of_solving();
	}
	else
	{
		m_new_parameters.incline_values.pitch = dichotomie(m_on_error, &dichotomie_pitch);
		m_new_parameters.incline_values.roll = dichotomie(m_on_error, &dichotomie_roll);
		find_solution ++;
	}
}

void Error_actions::action_no_movement_find_paw_spreadind_direction()
{
	if(m_on_error)
	{
		if(find_solution > 17*2)
		{
			find_solution = 0;
			paw_speading_direction = 0;
			m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;
			m_current_step = find_height_stable_direction;
		}
		else
		{
			m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;
			find_direction(find_solution, paw_speading_direction, m_new_parameters.paw_spreading);
		}
	}
	else
	{
		find_solution = 0;
		m_current_step = find_height_stable_direction;
		m_on_error = true;
	}
}

void Error_actions::action_no_movement_find_height_direction()
{

	if(m_on_error)
	{
		if(find_solution > 17*2)
		{
			find_solution = 0;
			height_direction = 0;
			m_new_parameters.center_height = m_purpose_parameters.center_height;
			m_current_step = get_closer_stable_parameters;
		}
		else
		{
			m_new_parameters.center_height = m_purpose_parameters.center_height;
			find_direction(find_solution, height_direction, m_new_parameters.center_height);
		}
	}
	else
	{
		find_solution = 0;
		m_current_step = get_closer_stable_parameters;
		m_on_error = true;
		m_resolving = false;
	}
}

void Error_actions::action_no_movement_get_closer_stable_parameters()
{
	if(paw_speading_direction != 0)
		m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading + paw_speading_direction/std::abs(paw_speading_direction)*SPREADING_STEP;
	else
		m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;

	if(height_direction != 0)
		m_new_parameters.center_height = m_purpose_parameters.center_height + height_direction/std::abs(height_direction)*HEIGHT_STEP;
	else
		m_new_parameters.center_height = m_purpose_parameters.center_height;

	if((height_direction == 0) && (paw_speading_direction == 0))
		m_new_parameters = m_precedent_parameters;

	set_end_of_solving();
}

void Error_actions::action_no_movement_changement()
{
	if(m_current_step == wait)
		m_current_step = stop_sequence; //stupid but must be remember

	if(m_current_step == stop_sequence)
		action_no_movement_get_closer_stop_sequence();
	else
		action_no_movement_no_changement();
}

void Error_actions::action_no_movement_get_closer_stop_sequence()
{
	if(m_on_error)
	{
		bool paw_spreading_pressed = false;
		bool height_pressed = false;

		if(m_purpose_parameters.paw_spreading != m_precedent_parameters.paw_spreading)
			paw_spreading_pressed = true;

		if(m_purpose_parameters.center_height != m_precedent_parameters.center_height)
			height_pressed = true;

		if((paw_spreading_pressed == true) and (height_pressed == true))
		{
			if (find_solution == 0)
			{
				m_new_parameters.center_height = m_precedent_parameters.center_height;
				find_solution++;
			}
			else if(find_solution == 1)
			{
				m_new_parameters.paw_spreading = m_precedent_parameters.paw_spreading;
				find_solution++;
			}
			else
			{
				find_solution = 0;
				m_current_step = cancel_incline;
			}
		}
		else if((paw_spreading_pressed == true) and (find_solution == 0))
		{
			m_new_parameters.paw_spreading = m_precedent_parameters.paw_spreading;
			find_solution ++;
		}
		else if((height_pressed == true) and (find_solution == 0))
		{
			m_new_parameters.center_height = m_precedent_parameters.center_height;
			find_solution++;
		}
		else
		{
			find_solution = 0;
			m_current_step = cancel_incline;
		}
	}
	else
	{
		find_solution = 0;
		m_current_step = wait;
		m_resolving = false;
	}
}
