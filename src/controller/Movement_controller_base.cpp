//============================================================================
// Name        : Movement_controller_base.cpp
// Author      : Sarthou
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "controller/Movement_controller_base.h"
#include "config.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include "movement/Circular_movement.h"
#include <cmath>
#include <stdlib.h>

Movement_controller_base::Movement_controller_base() : m_movement(nullptr), m_delegate(nullptr),
						m_current_step_number(0), m_step_number(0), new_movement(false),
						m_paw_spreading(DEFAULT_PAW_SPREADING), m_center_height(DEFAULT_HEIGHT),
					   m_movement_x_lin_value(0), m_movement_y_lin_value(0.f),
					   m_incline_pitch_lin_value(0.f), m_incline_roll_lin_value(0.f),
					   m_movement_type(linear)

{
	m_movement = new No_movement();
}

Movement_controller_base::~Movement_controller_base()
{
	delete m_movement;
}

bool Movement_controller_base::is_a_new_movement()
{
	return new_movement;
}

void Movement_controller_base::get_new_movement(int current_step_number, int step_number)
{
	new_movement = false;

	m_current_step_number = current_step_number;
	m_step_number = step_number;

	get_control_values();

	update_movement();

	custom_get_new_movement();

	update_incline();
}

void Movement_controller_base::update_movement()
{
	if((m_movement_x_lin_value == 0.f) and (m_movement_y_lin_value == 0.f))
	{
		if(m_movement != nullptr)
			delete m_movement;

		m_movement = new No_movement();
		new_movement = true;
	}
	else if(((m_current_step_number <= 1) && (m_movement->m_type == no_movement)) || (m_movement->m_type != no_movement))
	{
		if(m_movement != nullptr)
			delete m_movement;

		if(m_movement_type == linear)
		{
			int step_number = abs((int)(MAX_STEP_NUMBER - sqrtf(m_movement_y_lin_value*m_movement_y_lin_value + m_movement_x_lin_value*m_movement_x_lin_value)*MAX_STEP_NUMBER + MIN_STEP_NUMBER));
			if(step_number < 12)
				step_number = 12;

			float angle = atan2(m_movement_x_lin_value, m_movement_y_lin_value)*180/M_PI;
			m_movement = new complete_linear_movement(angle, DEFAULT_DISTANCE, step_number);
			new_movement = true;
		}
		else if(m_movement_type == circular)
		{
			int step_number = abs((int)(MAX_STEP_NUMBER/4.f - std::abs(m_movement_y_lin_value)*MAX_STEP_NUMBER/4.f + MIN_STEP_NUMBER));
			if(step_number < 12)
				step_number = 12;

			float radius = (1 - std::abs(m_movement_x_lin_value))*MAX_RADIUS;

			Movement_direction direction;
			if(m_movement_y_lin_value >= 0)
				direction = direction_front;
			else
				direction = direction_back;

			Rotation_side_enum side;
			if(m_movement_x_lin_value >= 0)
				side = left_rotation;
			else
				side = right_rotation;

			m_movement = new Circular_movement(radius, direction, side, DEFAULT_DISTANCE/4., step_number);
			new_movement = true;
		}
	}
}

void Movement_controller_base::update_incline()
{
	float incline_coef = std::abs(m_incline_roll_lin_value) + std::abs(m_incline_pitch_lin_value);
	if((int)incline_coef == 0)
		incline_coef = 1.;

	m_incline_coef.A = ((m_center_height + CENTER_TO_GROUND_OFFSET) / (-HALF_LENGTH)) * m_incline_pitch_lin_value * std::abs(m_incline_pitch_lin_value) / incline_coef;
	m_incline_coef.B = ((m_center_height + CENTER_TO_GROUND_OFFSET) / (-HALF_WIDTH_MAX)) * m_incline_roll_lin_value * std::abs(m_incline_roll_lin_value) / incline_coef;;
	m_incline_coef.C = m_center_height;
}

void Movement_controller_base::go_back_to_default_position()
{
	m_paw_spreading = DEFAULT_PAW_SPREADING;
	m_center_height = DEFAULT_HEIGHT;
	if(m_delegate != nullptr)
		m_delegate->reinit(m_incline_pitch_lin_value, m_incline_roll_lin_value, m_center_height, m_paw_spreading);
}

void Movement_controller_base::set_new_paw_spreading(float p_paw_spreading)
{
	m_paw_spreading = p_paw_spreading;
}

void Movement_controller_base::set_new_center_height(float p_center_height)
{
	m_center_height = p_center_height;
}

void Movement_controller_base::set_new_incline(float p_pitch_stick, float p_roll_stick)
{
	m_incline_roll_lin_value = p_roll_stick;
	m_incline_pitch_lin_value = p_pitch_stick;
	update_incline();
}

void Movement_controller_base::set_delegate(Movement_controller_delegate* p_delegate)
{
	m_delegate = p_delegate;
}




