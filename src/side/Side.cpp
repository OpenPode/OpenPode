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

int Side::update(int sequence_number, Incline_coef_t p_incline_coef, double paw_spreading) //ax+by+c for height
{
	Paw_position paw_position = m_movement->determine_paws_position(*this, sequence_number, p_incline_coef.A, p_incline_coef.B, p_incline_coef.C, paw_spreading);
	determine_servos_paw_time(m_front_paw, paw_position.front);
	determine_servos_paw_time(m_middle_paw, paw_position.middle);
	determine_servos_paw_time(m_back_paw, paw_position.back);

	move_paw(m_front_paw);
	move_paw(m_middle_paw);
	move_paw(m_back_paw);
	// for test purpose
	if(m_movement->is_sequence_finished(*this, sequence_number))
		return 0;
	else
		return 1;
}

bool Side::determine_servos_paw_time(Paw &paw, double coords[3])
{
	Angles angles = paw.move(coords[coord_x], coords[coord_y], coords[coord_z]);

	servos_time_table[paw.m_position][position_tibia] = (int)(- m_side_coef * (angles.theta3*(180./M_PI)+90.) * Servo::resolution + paw.m_tibia.get_offset());
	servos_time_table[paw.m_position][position_femur] = (int)(  m_side_coef *  angles.theta2*(180./M_PI) * Servo::resolution      + paw.m_femur.get_offset());
	servos_time_table[paw.m_position][position_coxa]  = (int)(-(angles.theta1*(180./M_PI) - m_side_coef*90.) * Servo::resolution  + paw.m_coxa.get_offset());

	return (paw.is_position_available(servos_time_table[paw.m_position][position_tibia],
									  servos_time_table[paw.m_position][position_femur],
									  servos_time_table[paw.m_position][position_coxa]));
}

void Side::move_paw(Paw &paw)
{
	m_module.set_off_time(channel_table[paw.m_position][position_tibia], servos_time_table[paw.m_position][position_tibia]);
	m_module.set_off_time(channel_table[paw.m_position][position_femur], servos_time_table[paw.m_position][position_femur]);
	m_module.set_off_time(channel_table[paw.m_position][position_coxa], servos_time_table[paw.m_position][position_coxa]);
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
