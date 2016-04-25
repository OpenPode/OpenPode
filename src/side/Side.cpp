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
}

int Side::update()
{
	Paw_position paw_position = m_movement->determine_paws_position(*this);
	m_front_paw.prepare_to_move(paw_position.front);
	m_middle_paw.prepare_to_move(paw_position.middle);
	m_back_paw.prepare_to_move(paw_position.back);

	move_paw(m_front_paw);
	move_paw(m_middle_paw);
	move_paw(m_back_paw);
	// for test purpose
	if(m_movement->is_sequence_finished(*this))
		return 0;
	else
		return 1;
}

void Side::move_paw(Paw &paw)
{
	paw.valid_move();
	m_module.set_off_time(channel_table[paw.m_position][position_tibia], paw.servos_time_table[position_tibia]);
	m_module.set_off_time(channel_table[paw.m_position][position_femur], paw.servos_time_table[position_femur]);
	m_module.set_off_time(channel_table[paw.m_position][position_coxa],  paw.servos_time_table[position_coxa]);
}

void Side::memorize_current_paw_position()
{
	m_paws_position.front_paw = m_front_paw.get_current_position();
	m_paws_position.middle_paw = m_middle_paw.get_current_position();
	m_paws_position.back_paw = m_back_paw.get_current_position();
}

void Side::memorize_movement(Movement *mvt)
{
	m_movement = mvt;
}

double Side::get_real_distance()
{
	return m_movement->determine_real_distance(*this);
}
