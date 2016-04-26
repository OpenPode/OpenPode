/*
 * Error_actions.cpp
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#include "Error_actions.h"

Error_actions::Error_actions(Movement_controller* p_movement_controller) : finished_to_corrected_error(true)
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

void Error_actions::purpose_new_parameters(double pitch_stick, double roll_stick, double height, double paw_spreading)
{
	m_purpose_parameters.center_height = height;
	m_purpose_parameters.paw_spreading = paw_spreading;
	m_purpose_parameters.incline_values.pitch = pitch_stick;
	m_purpose_parameters.incline_values.roll = roll_stick;
}

void Error_actions::resolve_error(Movement_type p_movement_type)
{
	m_new_parameters = m_purpose_parameters;//to change !!!!!
	switch(p_movement_type)
	{
		case linear:
		{

		}
		break;
		case complete_linear:
		{

		}
		break;
		case no_movement:
		{
			if(m_purpose_parameters == m_precedent_parameters)
			{
				action_no_movement_no_changement();
			}
			else
				action_no_movement_changement();
		}
		break;
		default:
		{

		}
		break;
	}

}

void Error_actions::action_no_movement_no_changement()
{

}

void Error_actions::action_no_movement_changement()
{

}

void Error_actions::reinit(double pitch_stick, double roll_stick, double height, double paw_spreading)
{
	std::cout << "reinit " << paw_spreading;
	m_precedent_parameters.center_height = height;
	m_precedent_parameters.paw_spreading = paw_spreading;
	m_precedent_parameters.incline_values.pitch = pitch_stick;
	m_precedent_parameters.incline_values.roll = roll_stick;
}
