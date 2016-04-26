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
	char* error_paw = select_paw_code(p_paw);

	test_machenical_stop_limit(p_paw, error_paw);
	test_model_limit(p_paw, error_paw);

	if(error_paw != 0)//error on the paw
		set_sequence(p_paw);
}

void Error_detection::set_error_location(Paw &p_paw)
{
	char shift;

	if(p_paw.m_side == side_left)
	{
		error_code |= SIDE_LEFT;
		shift = LEFT_SHIFT;
	}
	else if(p_paw.m_side == side_right)
	{
		error_code |= SIDE_RIGHT;
		shift = RIGHT_SHIFT;
	}

	if(p_paw.m_position == position_front)
		error_location |= (FRONT << shift);
	else if(p_paw.m_position == position_middle)
		error_location |= (MIDDLE << shift);
	else if(p_paw.m_position == position_back)
		error_location |= (BACK << shift);

	set_error();
}

void Error_detection::test_machenical_stop_limit(Paw &p_paw, char* p_error_paw)
{
	if(!p_paw.m_tibia.is_value_in_the_range(p_paw.servos_time_table[position_tibia]))
	{
		*p_error_paw |= (TIBIA << MECA_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_mecanical_type();
	}
	if(!p_paw.m_femur.is_value_in_the_range(p_paw.servos_time_table[position_femur]))
	{
		*p_error_paw |= (FEMUR << MECA_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_mecanical_type();
	}
	if(!p_paw.m_coxa.is_value_in_the_range(p_paw.servos_time_table[position_coxa]))
	{
		*p_error_paw |= (COXA << MECA_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_mecanical_type();
	}
}

void Error_detection::set_sequence(Paw &p_paw)
{
	if(p_paw.m_side == side_left)
	{
		if((m_sequence_number == 0) && (p_paw.m_position == position_front))
			error_code |= IN_SEQ;
		else if((m_sequence_number == 1) && (p_paw.m_position == position_middle))
			error_code |= IN_SEQ;
		else if((m_sequence_number == 2) && (p_paw.m_position == position_back))
			error_code |= IN_SEQ;
		else
			error_code |= STANDARD;
	}
	else
	{
		if((m_sequence_number == 0) && (p_paw.m_position == position_back))
			error_code |= IN_SEQ;
		else if((m_sequence_number == 1) && (p_paw.m_position == position_middle))
			error_code |= IN_SEQ;
		else if((m_sequence_number == 2) && (p_paw.m_position == position_front))
			error_code |= IN_SEQ;
		else
			error_code |= STANDARD;
	}
}

void Error_detection::test_model_limit(Paw &p_paw, char* p_error_paw)
{
	if(p_paw.m_servo_angles.theta3 != p_paw.m_servo_angles.theta3)//tibia
	{
		*p_error_paw |= (TIBIA << MODEL_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_model_type();
	}
	if(p_paw.m_servo_angles.theta2 != p_paw.m_servo_angles.theta2)//femur
	{
		*p_error_paw |= (FEMUR << MODEL_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_model_type();
	}
	if(p_paw.m_servo_angles.theta1 != p_paw.m_servo_angles.theta1)//coxa
	{
		*p_error_paw |= (COXA << MODEL_LIMIT_SHIFT);
		set_error_location(p_paw);
		set_error_model_type();
	}
}

char* Error_detection::select_paw_code(Paw &p_paw)
{
	if(p_paw.m_side == side_left)
	{
		if(p_paw.m_position == position_front)
			return &error_paw_L_F;
		else if(p_paw.m_position == position_middle)
			return &error_paw_L_M;
		else if(p_paw.m_position == position_back)
			return &error_paw_L_B;
	}
	else if(p_paw.m_side == side_right)
	{
		if(p_paw.m_position == position_front)
			return &error_paw_R_F;
		else if(p_paw.m_position == position_middle)
			return &error_paw_R_M;
		else if(p_paw.m_position == position_back)
			return &error_paw_R_B;
	}
	return nullptr;
}

bool Error_detection::is_on_error()
{
	return (error_code&ERROR);
}
