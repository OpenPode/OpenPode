/*
 * Error_no_movement.cpp
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#include "Error_actions.h"

void Error_actions::action_no_movement_no_changement()
{
	switch(m_current_step)
	{
	case cancel_incline:
	{
		m_new_parameters.incline_values.pitch = 0;
		m_new_parameters.incline_values.roll = 0;
		m_new_parameters.center_height = m_purpose_parameters.center_height;
		m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;
		m_current_step = reduce_incline;
		find_solution = false;
		std::cout << "cancel_incline" << std::endl;
	}
	break;
	case reduce_incline:
	{
		action_no_movement_reduce_incline();
		std::cout << "reduce_incline" << std::endl;
	}
	break;
	case find_paw_spreading_stable_direction:
	{
		action_no_movement_find_paw_spreadind_direction();
		std::cout << "find_paw_spreading_stable_direction " << std::endl;
	}
	break;
	case find_height_stable_direction:
	{
		std::cout << "find_height_stable_direction" << std::endl;
		action_no_movement_find_height_direction();
	}
	break;
	case find_stable_parameters:
	{
		std::cout << height_direction << std::endl;
		while(1);
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
	if(m_on_error)
	{
		if(find_solution == 0)
		{
			m_current_step = find_paw_spreading_stable_direction;
			m_new_parameters = m_purpose_parameters;
		}
		else
		{
			find_solution ++;
			find_stable_incline();
		}
	}
	else
	{
		if(find_solution == 0)
			m_new_parameters = m_purpose_parameters;

		if(find_solution > 10)
			set_end_of_solving();
		else
		{
			find_solution ++;
			find_stable_incline();
		}
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
			find_paw_spreadind_direction();
			find_solution ++;
		}
	}
	else
	{
		find_solution = 0;
		m_new_parameters.paw_spreading = m_purpose_parameters.paw_spreading;
		m_current_step = find_height_stable_direction;
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
			m_current_step = find_stable_parameters;
		}
		else
		{
			find_height_direction();
			find_solution ++;
		}
	}
	else
	{
		find_solution = 0;
		m_new_parameters.center_height = m_purpose_parameters.center_height;
		m_current_step = find_stable_parameters;
	}
}

void Error_actions::action_no_movement_changement()
{

}
