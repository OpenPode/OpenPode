/*
 * Error_actions.cpp
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#include "Error_actions.h"

Error_actions::Error_actions(Movement_controller* p_movement_controller) : m_finished_to_corrected_error(true),
																		   m_resolving(false), m_current_step(wait),
																		   m_on_error(false), find_solution(0),
																		   paw_speading_direction(0), height_direction(0)
{
	m_movement_controller = p_movement_controller;
	m_movement_controller->set_delegate(this);
}

Error_actions::~Error_actions()
{

}

void Error_actions::valid_parameters()
{
	m_precedent_parameters = m_new_parameters;
}

void Error_actions::valid_parameters_no_error()
{
	m_precedent_parameters = m_purpose_parameters;
}

void Error_actions::purpose_new_parameters(float pitch_stick, float roll_stick, float height, float paw_spreading)
{
	m_purpose_parameters.center_height = height;
	m_purpose_parameters.paw_spreading = paw_spreading;
	m_purpose_parameters.incline_values.pitch = pitch_stick;
	m_purpose_parameters.incline_values.roll = roll_stick;
}

void Error_actions::resolve_error(Movement_type p_movement_type, bool on_error)
{
	m_on_error = on_error;
	m_new_parameters = m_purpose_parameters;//to change !!!!!
	switch(p_movement_type)
	{
		case linear:
		{
			no_action();
		}
		break;
		case complete_linear:
		{
			no_action();
		}
		break;
		case no_movement:
		{
			if(m_current_step == wait)//init no_movement resolve
			{
				m_resolving = true;
				m_finished_to_corrected_error = false;
				find_solution = 0;
			}

			if(m_purpose_parameters == m_precedent_parameters)
				action_no_movement_no_changement();
			else
				action_no_movement_changement();
		}
		break;
		default:
		{
			no_action();
		}
		break;
	}
	set_parameters_on_movement_controller();
}

void Error_actions::reinit(float pitch_stick, float roll_stick, float height, float paw_spreading)
{
	m_precedent_parameters.center_height = height;
	m_precedent_parameters.paw_spreading = paw_spreading;
	m_precedent_parameters.incline_values.pitch = pitch_stick;
	m_precedent_parameters.incline_values.roll = roll_stick;
	m_new_parameters = m_precedent_parameters;
}

void Error_actions::set_parameters_on_movement_controller()
{
	m_movement_controller->set_new_center_height(m_new_parameters.center_height);
	m_movement_controller->set_new_paw_spreading(m_new_parameters.paw_spreading);
	m_movement_controller->set_new_incline(m_new_parameters.incline_values.pitch, m_new_parameters.incline_values.roll);
}

void Error_actions::set_end_of_solving()
{
	m_current_step = wait;
	m_resolving = false;
	m_finished_to_corrected_error = true;
	find_solution = 0;
	paw_speading_direction = 0;
	height_direction = 0;
}

float Error_actions::dichotomie(bool condition, Dichotomie_t* dicho)
{
	if(!condition)
		dicho->B = dicho->C;
	else
		dicho->A = dicho->C;

	dicho->C = dicho->B + (dicho->A - dicho->B)/2.;

	return dicho->C;
}

void Error_actions::find_direction(int &cpt, float &direction, float &new_parameters, int step)
{
	if((cpt % 2) == 0)
		direction = step*(cpt/2.f + 1);
	else
		direction = - step*((cpt+1)/2.f);

	new_parameters = new_parameters + direction;
	cpt++;
}

void Error_actions::no_action()
{
	m_new_parameters = m_purpose_parameters;
	set_end_of_solving();
}
