/*
 * Side.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */


#include "Side.h"
#include "hexapode_dimensions.h"
#include "Movement.h"
#include <cmath>

const Channel_t Side::channel_table[3][3] =
{
		{channel0, channel1, channel2},
		{channel3, channel4, channel5},
		{channel6, channel7, channel8}
};

Side::Side(Side_enum side, i2cdev *i2c) : m_side(side),
										  m_front_paw(side, position_front),
										  m_middle_paw(side, position_middle),
										  m_back_paw(side, position_back),
										  m_movement(nullptr)
{
	if(m_side == side_left)
	{
		m_module = PCA9685(i2c, 0x42);
		m_side_coef = +1;
	}
	else
	{
		m_module = PCA9685(i2c, 0x41);
		m_side_coef = -1;
	}
	//represent the initial sequence
	m_current_sequence_number = -1;
}

int Side::update(int sequence_number, double a, double b, double paw_spreading) //ax+b for height
{
	Paw_position paw_position = m_movement->determine_paws_position(*this, sequence_number, a, b, paw_spreading);

	move_paw(m_front_paw, paw_position.front);
	move_paw(m_middle_paw, paw_position.middle);
	move_paw(m_back_paw, paw_position.back);

	// for test purpose
	if(m_movement->m_current_step_number == (m_movement->m_step_number - 1))
		return 0;
	else
		return 1;
}

void Side::move_paw(Paw &paw, double coords[3])
{
	Angles angles = paw.move(coords[coord_x], coords[coord_y], coords[coord_z]);

	int time_tibias = static_cast<int>(- m_side_coef * (angles.theta3*(180./M_PI)+90.) * Servo::resolution + paw.m_tibia.get_offset());
	int time_femur  = static_cast<int>(  m_side_coef *  angles.theta2*(180./M_PI) * Servo::resolution      + paw.m_femur.get_offset());
	int time_coxa   = static_cast<int>(-(angles.theta1*(180./M_PI) - m_side_coef*90.) * Servo::resolution  + paw.m_coxa.get_offset());

	m_module.set_off_time(channel_table[paw.m_position][position_tibia], time_tibias);
	m_module.set_off_time(channel_table[paw.m_position][position_femur], time_femur);
	m_module.set_off_time(channel_table[paw.m_position][position_coxa], time_coxa);
}

void Side::memorize_current_paw_position()
{
	m_paws_position.front_paw = m_front_paw.get_current_position();
	m_paws_position.middle_paw = m_middle_paw.get_current_position();
	m_paws_position.back_paw = m_back_paw.get_current_position();
}

void Side::memorize_movement(Movement *mvt, int p_current_step_number)
{
	m_movement = mvt;
	m_movement->m_current_step_number = p_current_step_number;
}

void Side::change_sequence_number(int sequence_number, int p_current_step_number)
{
	m_current_sequence_number = sequence_number;
	m_movement->m_current_step_number = p_current_step_number;
}

double Side::get_real_distance()
{
	return m_movement->determine_real_distance(*this);
}
