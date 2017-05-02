//============================================================================
// Name        : Movement_controller_PS4.cpp
// Author      : Sarthou
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "controller/Movement_controller_PS4.h"
#include "drivers/broadcom/bcm2835.h"
#include "config.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include "movement/Circular_movement.h"
#include <cmath>
#include <stdlib.h>

Movement_controller_PS4::Movement_controller_PS4() :
					   m_movement_x_value(0), m_movement_y_value(0),
					   m_incline_pitch_value(0), m_incline_roll_value(0),
					   m_up_pressed(false), m_down_pressed(false),
					   m_move_apart_pressed(false), m_tighten_pressed(false),
					   m_turn_back_default_pressed(false), m_turn_back_default_last_state(false),
					   m_square_pressed(false), m_circle_pressed(false),
					   m_change_movement_pressed(false), m_change_movement_was_pressed(false)

{
}

Movement_controller_PS4::~Movement_controller_PS4()
{
}

float Movement_controller_PS4::make_more_linear(int stick_value) //out between [-1 ; 1]
{
	if(stick_value > 0)
	{
		stick_value -= m_PS4_controller.stick_offset;
		if(stick_value < 0)
			stick_value = 0;
	}
	else if(stick_value < 0)
	{
		stick_value += m_PS4_controller.stick_offset;
		if(stick_value > 0)
			stick_value = 0;
	}
	return (stick_value/(m_PS4_controller.max_stick_value - m_PS4_controller.stick_offset));
}

void Movement_controller_PS4::make_sticks_more_linear()
{
	m_movement_x_lin_value = make_more_linear(m_movement_x_value);
	m_movement_y_lin_value = make_more_linear(m_movement_y_value);
	m_incline_pitch_lin_value = make_more_linear(m_incline_pitch_value);
	m_incline_roll_lin_value = make_more_linear(m_incline_roll_value);
}

void Movement_controller_PS4::run_controller()
{
	m_PS4_controller.process_input();
}

void Movement_controller_PS4::get_control_values()
{
	m_movement_x_value = m_PS4_controller.get_js_left_x();
	m_movement_y_value = m_PS4_controller.get_js_left_y();
	m_incline_pitch_value = m_PS4_controller.get_js_right_y();
	m_incline_roll_value = m_PS4_controller.get_js_right_x();
	m_up_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_R2);
	m_down_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_R1);
	m_move_apart_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_L2);
	m_tighten_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_L1);
	m_turn_back_default_last_state = m_turn_back_default_pressed;
	m_turn_back_default_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_OPTIONS);
	m_square_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_SQUARE);
	m_circle_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_CIRCLE);
	m_change_movement_was_pressed = m_change_movement_pressed;
	m_change_movement_pressed = m_PS4_controller.is_key_press(PS4_Key::KEY_L3);
}

int Movement_controller_PS4::get_led()
{
	if((m_square_pressed) && (not m_circle_pressed))
		return 2;
	else if((m_circle_pressed) && (not m_square_pressed))
		return 1;
	else
		return 0;
}

void Movement_controller_PS4::get_new_movement(int current_step_number, int step_number)
{
	new_movement = false;

	m_current_step_number = current_step_number;
	m_step_number = step_number;

	get_control_values();
	//make_sticks_more_linear();
	//verify_movement_type_changement();

	update_movement();

	//update_paw_spreading();
	//update_center_height();

	/*if((m_turn_back_default_pressed == true) and (m_turn_back_default_last_state == false) and (m_movement->m_type == no_movement))
		go_back_to_default_position();*/

	update_incline();
}

void Movement_controller_PS4::update_movement()
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

void Movement_controller_PS4::update_paw_spreading()
{
	if(m_move_apart_pressed)
	{
		if(m_paw_spreading <= (TIBIA_LENGTH + FEMUR_LENGTH))
			m_paw_spreading += SPREADING_STEP;
	}
	else if(m_tighten_pressed)
	{
		if(m_paw_spreading >= TIBIA_ORIGIN_OFFSET)
		m_paw_spreading -= SPREADING_STEP;
	}
}

void Movement_controller_PS4::update_center_height()
{
	if(m_up_pressed)
	{
		if(m_center_height <= 0)
			m_center_height += HEIGHT_STEP;
	}
	else if(m_down_pressed)
	{
		if(m_center_height >= (-TIBIA_LENGTH - FEMUR_LENGTH))
			m_center_height -= HEIGHT_STEP;
	}
}

void Movement_controller_PS4::verify_movement_type_changement()
{
	if((m_change_movement_pressed == true) && (m_change_movement_was_pressed == false))
	{
		if(m_movement_type == linear)
			m_movement_type = circular;
		else if(m_movement_type == circular)
			m_movement_type = linear;
	}

}



