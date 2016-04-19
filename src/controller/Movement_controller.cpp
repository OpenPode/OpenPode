//============================================================================
// Name        : Movement_controller.cpp
// Author      : Sarthou
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Movement_controller.h"
#include "bcm2835.h"
#include "hexapode_dimensions.h"

#include "Linear_movement.h"
#include "No_movement.h"
#include "complete_linear_movement.h"
#include <cmath>
#include <stdlib.h>

Movement_controller::Movement_controller() : m_movement(nullptr), // init to no_movement
					   m_paw_spreading(80), m_center_height(-50),
					   m_current_step_number(0), m_step_number(0),
					   m_A_coef_incline(0), m_B_coef_incline(0),
					   m_movement_x_value(0), m_movement_y_value(0),
					   m_incline_value(0),
					   m_up_pressed(false), m_down_pressed(false),
					   m_move_apart_pressed(false), m_tighten_pressed(false)

{
	m_movement = new No_movement();
}

Movement_controller::~Movement_controller()
{
	delete m_movement;
}

void Movement_controller::run_controller()
{
	m_PS4_controller.process_input();
}

void Movement_controller::get_control_values()
{
	m_movement_x_value = m_PS4_controller.m_jsl_x_value;
	m_movement_y_value = m_PS4_controller.m_jsl_y_value;
	m_incline_value = m_PS4_controller.m_jsr_y_value;
	m_up_pressed = m_PS4_controller.m_is_r2_press;
	m_down_pressed = m_PS4_controller.m_is_r1_press;
	m_move_apart_pressed = m_PS4_controller.m_is_l2_press;
	m_tighten_pressed = m_PS4_controller.m_is_l1_press;
}

bool Movement_controller::get_new_movement(int current_step_number, int step_number)
{
	bool have_changed = false;

	m_current_step_number = current_step_number;
	m_step_number = step_number;

	get_control_values();

	if((abs(m_movement_x_value) <= 2000) and (abs(m_movement_y_value) <= 2000))
	{
		if(m_movement != nullptr)
			delete m_movement;

		m_movement = new No_movement();
		have_changed = true;
	}
	else if(((m_current_step_number <= 1) && (m_movement->m_type == no_movement)) || (m_movement->m_type != no_movement))
	{
		int step_number = abs(140 - sqrt(m_movement_y_value*m_movement_y_value + m_movement_x_value*m_movement_x_value)/32000.*140.+12);
		if(step_number < 12)
			step_number = 12;

		if(m_movement != nullptr)
			delete m_movement;

		m_movement = new complete_linear_movement(  atan2(m_movement_x_value, m_movement_y_value)*180/M_PI, 40, step_number);
		have_changed = true;
	}

	if(m_move_apart_pressed)
	{
		if(m_paw_spreading <= (TIBIA_LENGTH + FEMUR_LENGTH))
			m_paw_spreading += 0.5;
	}
	else if(m_tighten_pressed)
	{
		if(m_paw_spreading >= TIBIA_ORIGIN_OFFSET)
		m_paw_spreading -= 0.5;
	}

	if(m_up_pressed)
	{
		if(m_center_height <= 0)
			m_center_height += 0.5;
	}
	else if(m_down_pressed)
	{
		if(m_center_height >= (-TIBIA_LENGTH - FEMUR_LENGTH))
			m_center_height -= 0.5;
	}

	m_A_coef_incline = ((m_center_height - CENTER_TO_GROUND_OFFSET) / (HALF_LENGTH + 80.)) * (m_incline_value/m_PS4_controller.max_pot_value);
	m_B_coef_incline = m_center_height;

	return have_changed;
}
