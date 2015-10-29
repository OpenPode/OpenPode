/*
 * Side.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */


#include "Side.h"

Side::Side(Side_enum side, i2cdev *i2c) : m_side(side),
										  m_front_paw(side, position_front),
										  m_middle_paw(side, position_middle),
										  m_back_paw(side, position_back)
{
	if(m_side == side_left)
		m_module = PCA9685(i2c, 0x42);
	else
		m_module = PCA9685(i2c, 0x41);

	m_current_sequence_number = -1;
}

void Side::memorize_movement(Movement mvt)
{
	m_movement = mvt;

	switch(mvt.type)
	{
	case linear:
		m_step_distance = mvt.distance / 2.0 / mvt.step_number;
		break;
	case rotation_left:
		break;
	case rotation_right:
		break;
	case no_movement:
		break;
	}
}

int Side::update(int sequence_number, double front_height, double back_height, double paw_spreading)
{
	m_current_step_number++;

	double x_front_paw, y_front_paw, z_front_paw;
	double x_middle_paw, y_middle_paw, z_middle_paw;
	double x_back_paw, y_back_paw, z_back_paw;

	if(sequence_number == 0)
	{
		if(m_side == Side_enum::side_left)
		{
			x_front_paw = m_paws_position.front_paw.x + m_step_distance * m_current_step_number;
			x_middle_paw =  m_paws_position.middle_paw.x - m_step_distance * m_current_step_number;
			x_back_paw =  m_paws_position.back_paw.x + m_step_distance * m_current_step_number;

			y_front_paw = paw_spreading;
			y_middle_paw = paw_spreading - 10.;
			y_back_paw = paw_spreading;

			z_front_paw = -front_height;
			z_middle_paw = -front_height;
			z_back_paw = -back_height;
		}
		else
		{
			x_front_paw = m_paws_position.front_paw.x - m_step_distance * m_current_step_number;
			x_middle_paw =  m_paws_position.middle_paw.x + m_step_distance * m_current_step_number;
			x_back_paw =  m_paws_position.back_paw.x - m_step_distance * m_current_step_number;

			y_front_paw = -paw_spreading;
			y_middle_paw = -(paw_spreading + 10.);
			y_back_paw = -paw_spreading;

			z_front_paw = -front_height;
			z_middle_paw = -front_height;
			z_back_paw = -back_height;
		}
	}
	else
	{
		if(m_side == Side_enum::side_left)
		{
			x_front_paw = m_paws_position.front_paw.x - m_step_distance * m_current_step_number;
			x_middle_paw =  m_paws_position.middle_paw.x + m_step_distance * m_current_step_number;
			x_back_paw =  m_paws_position.back_paw.x - m_step_distance * m_current_step_number;

			y_front_paw = paw_spreading;
			y_middle_paw = paw_spreading - 10.;
			y_back_paw = paw_spreading;

			z_front_paw = -front_height;
			z_middle_paw = -front_height;
			z_back_paw = -back_height;
		}
		else
		{
			x_front_paw = m_paws_position.front_paw.x + m_step_distance * m_current_step_number;
			x_middle_paw =  m_paws_position.middle_paw.x - m_step_distance * m_current_step_number;
			x_back_paw =  m_paws_position.back_paw.x + m_step_distance * m_current_step_number;

			y_front_paw = -paw_spreading;
			y_middle_paw = -(paw_spreading + 10.);
			y_back_paw = -paw_spreading;

			z_front_paw = -front_height;
			z_middle_paw = -front_height;
			z_back_paw = -back_height;
		}
	}

	Angles angles = m_front_paw.move(x_front_paw, y_front_paw, z_front_paw);
	int time3_front = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_front_paw.m_servo3.get_offset());
	int time2_front = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_front_paw.m_servo2.get_offset());
	int time1_front = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_front_paw.m_servo1.get_offset());

	angles = m_middle_paw.move(x_middle_paw, y_middle_paw, z_middle_paw);
	int time3_middle = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_middle_paw.m_servo3.get_offset());
	int time2_middle = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_middle_paw.m_servo2.get_offset());
	int time1_middle = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_middle_paw.m_servo1.get_offset());

	angles = m_back_paw.move(x_back_paw, y_back_paw, z_back_paw);
	int time3_back = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_back_paw.m_servo3.get_offset());
	int time2_back = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_back_paw.m_servo2.get_offset());
	int time1_back = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_back_paw.m_servo1.get_offset());

	m_module.set_off_time(channel0, time3_front);
	m_module.set_off_time(channel1, time2_front);
	m_module.set_off_time(channel2, time1_front);
	m_module.set_off_time(channel3, time3_middle);
	m_module.set_off_time(channel4, time2_middle);
	m_module.set_off_time(channel5, time1_middle);
	m_module.set_off_time(channel6, time3_back);
	m_module.set_off_time(channel7, time2_back);
	m_module.set_off_time(channel8, time1_back);

	if(m_current_step_number == m_movement.step_number)
		return 0;
	else
		return 1;
}

void Side::memorize_current_paw_position()
{
	m_paws_position.front_paw = m_front_paw.get_current_position();
	m_paws_position.middle_paw = m_middle_paw.get_current_position();
	m_paws_position.back_paw = m_back_paw.get_current_position();
}

double Side::change_sequence_number(int sequence_number)
{
	m_current_sequence_number = sequence_number;
	m_current_step_number = 0;
	memorize_current_paw_position();
	return determine_real_distance();
}

double Side::determine_real_distance()
{
	//double real_distance[3];



	return m_movement.distance;
}

