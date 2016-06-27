/*
 * Error_detection.cpp
 *
 *  Created on: 25 avr. 2016
 *      Author: Sarthou
 */

#include "Error_detection.h"

Error_detection::Error_detection() : error_code(0), error_location(0),
									 error_paw_R_F(0), error_paw_R_M(0), error_paw_R_B(0),
									 error_paw_L_F(0), error_paw_L_M(0), error_paw_L_B(0),
									 m_sequence_number(0)
{

}

Error_detection::~Error_detection()
{

}

void Error_detection::reset()
{
	error_code = 0;
	error_location = 0;
	error_paw_R_F = 0;
	error_paw_R_M = 0;
	error_paw_R_B = 0;
	error_paw_L_F = 0;
	error_paw_L_M = 0;
	error_paw_L_B = 0;
}

void Error_detection::set_sequence_number(int p_sequence_number)
{
	m_sequence_number = p_sequence_number;
}

void Error_detection::set_error()
{
	error_code |= ERROR;
}

void Error_detection::test_error()
{
	if(error_paw_L_B | error_paw_L_M | error_paw_L_F | error_paw_R_B | error_paw_R_M | error_paw_R_F)
	{
		set_error();
		if((error_paw_L_B&IN_SEQUENCE) | (error_paw_L_M&IN_SEQUENCE) | (error_paw_L_F&IN_SEQUENCE) |
		   (error_paw_R_B&IN_SEQUENCE) | (error_paw_R_M&IN_SEQUENCE) | (error_paw_R_F&IN_SEQUENCE))
			error_code |= IN_SEQ;

		if(error_paw_L_F)
			error_location |= (FRONT << LEFT_SHIFT);
		else if(error_paw_L_M)
			error_location |= (MIDDLE << LEFT_SHIFT);
		else if(error_paw_L_B)
			error_location |= (BACK << LEFT_SHIFT);
		else if(error_paw_R_F)
			error_location |= (FRONT << RIGHT_SHIFT);
		else if(error_paw_R_M)
			error_location |= (MIDDLE << RIGHT_SHIFT);
		else if(error_paw_R_B)
			error_location |= (BACK << RIGHT_SHIFT);

		if(error_paw_L_B | error_paw_L_M | error_paw_L_F)
			error_code |= SIDE_LEFT;

		if(error_paw_R_B | error_paw_R_M | error_paw_R_F)
			error_code |= SIDE_RIGHT;
	}
	else
		reset();
}

void Error_detection::set_error_model_type()
{
	error_code |= MODEL;
}
void Error_detection::set_error_mecanical_type()
{
	error_code |= MECA_LIMIT;
}

void Error_detection::set_paw(Paw &p_paw)
{
	uint8_t* error_paw = select_paw_code(p_paw);
	*error_paw = 0;
	test_machenical_stop_limit(p_paw, error_paw);
	test_model_limit(p_paw, error_paw);
}

void Error_detection::test_machenical_stop_limit(Paw &p_paw, uint8_t *p_error_paw)
{
	if(!p_paw.get_tibia().is_value_in_the_range(p_paw.get_servo_time(position_tibia)))
	{
		*p_error_paw |= (TIBIA << MECA_LIMIT_SHIFT);
		set_error_mecanical_type();
		set_sequence(p_paw);
	}
	if(!p_paw.get_femur().is_value_in_the_range(p_paw.get_servo_time(position_femur)))
	{
		*p_error_paw |= (FEMUR << MECA_LIMIT_SHIFT);
		set_error_mecanical_type();
		set_sequence(p_paw);
	}
	if(!p_paw.get_coxa().is_value_in_the_range(p_paw.get_servo_time(position_coxa)))
	{
		*p_error_paw |= (COXA << MECA_LIMIT_SHIFT);
		set_error_mecanical_type();
		set_sequence(p_paw);
	}
}

void Error_detection::set_sequence(Paw &p_paw)
{
	if(m_sequence_number == p_paw.get_active_sequence_number())
	{
		uint8_t* error_paw = select_paw_code(p_paw);
		*error_paw |= IN_SEQUENCE;
		error_code |= IN_SEQ;
	}
	else
		error_code |= STANDARD;
}

void Error_detection::test_model_limit(Paw &p_paw, uint8_t *p_error_paw)
{
	if(p_paw.get_servo_angle().theta3 != p_paw.get_servo_angle().theta3)//tibia
	{
		*p_error_paw |= (TIBIA << MODEL_LIMIT_SHIFT);
		set_error_model_type();
		set_sequence(p_paw);
	}
	if(p_paw.get_servo_angle().theta2 != p_paw.get_servo_angle().theta2)//femur
	{
		*p_error_paw |= (FEMUR << MODEL_LIMIT_SHIFT);
		set_error_model_type();
		set_sequence(p_paw);
	}
	if(p_paw.get_servo_angle().theta1 != p_paw.get_servo_angle().theta1)//coxa
	{
		*p_error_paw |= (COXA << MODEL_LIMIT_SHIFT);
		set_error_model_type();
		set_sequence(p_paw);
	}
}

uint8_t* Error_detection::select_paw_code(Paw &p_paw)
{
	if(p_paw.get_side() == side_left)
	{
		if(p_paw.get_position() == position_front)
			return &error_paw_L_F;
		else if(p_paw.get_position() == position_middle)
			return &error_paw_L_M;
		else if(p_paw.get_position() == position_back)
			return &error_paw_L_B;
	}
	else if(p_paw.get_side() == side_right)
	{
		if(p_paw.get_position() == position_front)
			return &error_paw_R_F;
		else if(p_paw.get_position() == position_middle)
			return &error_paw_R_M;
		else if(p_paw.get_position() == position_back)
			return &error_paw_R_B;
	}
	return NULL;
}

bool Error_detection::is_on_error()
{
	return error_code & ERROR;
}

bool Error_detection::is_on_sequence()
{
	return error_code & IN_SEQ;
}
