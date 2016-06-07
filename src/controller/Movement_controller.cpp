//============================================================================
// Name        : Movement_controller.cpp
// Author      : Sarthou
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "controller/Movement_controller.h"
#include "i2c/bcm2835.h"
#include "hexapode/hexapode_dimensions.h"
#include "config.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include <cmath>
#include <stdlib.h>

Movement_controller::Movement_controller() : m_movement(nullptr), m_delegate(nullptr),
					   m_paw_spreading(DEFAULT_PAW_SPREADING), m_center_height(DEFAULT_HEIGHT),
					   m_current_step_number(0), m_step_number(0), new_movement(0),
					   m_movement_x_value(0), m_movement_y_value(0),
					   m_incline_pitch_value(0), m_incline_roll_value(0),
					   m_movement_x_lin_value(0), m_movement_y_lin_value(0),
					   m_incline_pitch_lin_value(0), m_incline_roll_lin_value(0),
					   m_up_pressed(false), m_down_pressed(false),
					   m_move_apart_pressed(false), m_tighten_pressed(false),
					   m_turn_back_default_pressed(false), m_turn_back_default_last_state(false)

{
	m_movement = new No_movement();
}

Movement_controller::~Movement_controller()
{
	delete m_movement;
}

float Movement_controller::make_more_linear(int stick_value) //out between [-1 ; 1]
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

void Movement_controller::make_sticks_more_linear()
{
	m_movement_x_lin_value = make_more_linear(m_movement_x_value);
	m_movement_y_lin_value = make_more_linear(m_movement_y_value);
	m_incline_pitch_lin_value = make_more_linear(m_incline_pitch_value);
	m_incline_roll_lin_value = make_more_linear(m_incline_roll_value);
}

void Movement_controller::run_controller()
{
	m_PS4_controller.process_input();
}

void Movement_controller::get_control_values()
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
}

bool Movement_controller::is_a_new_movement()
{
	return new_movement;
}

void Movement_controller::get_new_movement(int current_step_number, int step_number)
{
	new_movement = false;

	m_current_step_number = current_step_number;
	m_step_number = step_number;

	get_control_values();
	make_sticks_more_linear();

	update_movement();

	update_paw_spreading();

	update_center_height();

	if((m_turn_back_default_pressed == true) and (m_turn_back_default_last_state == false) and (m_movement->m_type == no_movement))
		go_back_to_default_position();

	update_incline();
}

void Movement_controller::update_movement()
{
	if((m_movement_x_lin_value == 0) and (m_movement_y_lin_value == 0))
	{
		if(m_movement != nullptr)
			delete m_movement;

		m_movement = new No_movement();
		new_movement = true;
	}
	else if(((m_current_step_number <= 1) && (m_movement->m_type == no_movement)) || (m_movement->m_type != no_movement))
	{
		int step_number = abs(MAX_STEP_NUMBER - sqrt(m_movement_y_lin_value*m_movement_y_lin_value + m_movement_x_lin_value*m_movement_x_lin_value)*MAX_STEP_NUMBER + MIN_STEP_NUMBER);
		if(step_number < 12)
			step_number = 12;

		if(m_movement != nullptr)
			delete m_movement;

		m_movement = new complete_linear_movement(  atan2(m_movement_x_lin_value, m_movement_y_lin_value)*180/M_PI, DEFAULT_DISTANCE, step_number);
		new_movement = true;
	}
}

void Movement_controller::update_paw_spreading()
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

void Movement_controller::update_center_height()
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

void Movement_controller::update_incline()
{
	float incline_coef = std::abs(m_incline_roll_lin_value) + std::abs(m_incline_pitch_lin_value);
	if(incline_coef == 0)
		incline_coef = 1.;

	m_incline_coef.A = ((m_center_height + CENTER_TO_GROUND_OFFSET) / (-HALF_LENGTH)) * m_incline_pitch_lin_value * std::abs(m_incline_pitch_lin_value) / incline_coef;
	m_incline_coef.B = ((m_center_height + CENTER_TO_GROUND_OFFSET) / (-HALF_WIDTH_MAX)) * m_incline_roll_lin_value * std::abs(m_incline_roll_lin_value) / incline_coef;;
	m_incline_coef.C = m_center_height;
}

void Movement_controller::go_back_to_default_position()
{
	m_paw_spreading = DEFAULT_PAW_SPREADING;
	m_center_height = DEFAULT_HEIGHT;
	if(m_delegate != nullptr)
		m_delegate->reinit(m_incline_pitch_lin_value, m_incline_roll_lin_value, m_center_height, m_paw_spreading);


}

void Movement_controller::set_new_paw_spreading(float p_paw_spreading)
{
	m_paw_spreading = p_paw_spreading;
}

void Movement_controller::set_new_center_height(float p_center_height)
{
	m_center_height = p_center_height;
}

void Movement_controller::set_new_incline(float p_pitch_stick, float p_roll_stick)
{
	m_incline_roll_lin_value = p_roll_stick;
	m_incline_pitch_lin_value = p_pitch_stick;
	update_incline();
}

void Movement_controller::set_delegate(Movement_controller_delegate* p_delegate)
{
	m_delegate = p_delegate;
}
