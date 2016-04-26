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

void Error_actions::purpose_new_parameters(double x_stick_incline, double y_stick_incline, double height, double paw_spreading)
{
	m_purpose_parameters.center_height = height;
	m_purpose_parameters.paw_spreading = paw_spreading;
	m_purpose_parameters.incline_values.x_stick = x_stick_incline;
	m_purpose_parameters.incline_values.y_stick = y_stick_incline;
}

void Error_actions::resolve_error(Movement_type p_movement_type)
{
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

		}
		break;
		default:
		{

		}
		break;
	}

}

void Error_actions::reinit(double x_stick_incline, double y_stick_incline, double height, double paw_spreading)
{

}
