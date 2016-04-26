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
	}
	break;
	case reduce_incline:
	{
		action_no_movement_reduce_incline();
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
			m_current_step = find_stable_parameters_direction;
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

void Error_actions::action_no_movement_changement()
{

}
