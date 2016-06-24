/*
 * Side.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */


#include "side/Side.h"
#include "hexapode/hexapode_dimensions.h"
#include "movement/Movement.h"
#include <cmath>

const Channel_t Side::channel_table[3][3] =
{
		{channel0, channel1, channel2},
		{channel3, channel4, channel5},
		{channel6, channel7, channel8}
};

Side::Side(Side_enum side, i2cdev *i2c, Error_detection* p_error_detection, const int p_sequence_of_paws[3]) : m_side(side),
										  m_front_paw(side, position_front, p_error_detection, HALF_LENGTH, HALF_WIDTH_MIN, p_sequence_of_paws[0]),
										  m_middle_paw(side, position_middle, p_error_detection, 0.f, HALF_WIDTH_MAX, p_sequence_of_paws[1]),
										  m_back_paw(side, position_back, p_error_detection, -HALF_LENGTH, HALF_WIDTH_MIN, p_sequence_of_paws[2]),
										  m_movement(nullptr), m_error_detection(p_error_detection)
{
	if(m_side == side_left)
	{
		m_module = PCA9685(i2c, PCA9685_LEFT_ADDR);
		m_side_coef = +1;
	}
	else
	{
		m_module = PCA9685(i2c, PCA9685_RIGHT_ADDR);
		m_side_coef = -1;
	}
}

void Side::prepare_update()
{
	prepare_one_paw(m_front_paw);
	prepare_one_paw(m_middle_paw);
	prepare_one_paw(m_back_paw);
}

void Side::prepare_one_paw(Paw &paw)
{
#ifdef ERROR_ACTION
	char* error_code;
	int nb_of_test = 0;
	do
	{
#endif

		paw.prepare_to_move(m_movement->determine_paw_position(paw));

#ifdef ERROR_ACTION
		error_code = m_error_detection->select_paw_code(paw);
		if((*error_code) & IN_SEQUENCE)
		{
			nb_of_test++;
			m_movement->set_nb_of_solve(nb_of_test);
		}
	}
	while(((*error_code) & IN_SEQUENCE) && (nb_of_test <= 3));

	if((*error_code) & IN_SEQUENCE)
	{
		m_movement->reset_nb_of_solve();
		paw.prepare_to_move(m_movement->determine_paw_position(paw));
	}
	else if(nb_of_test != 0)
		m_movement->reset_nb_of_solve();
#endif
}

int Side::update()
{
	move_paw(m_front_paw);
	move_paw(m_middle_paw);
	move_paw(m_back_paw);
	// for test purpose
	if(m_movement->is_sequence_finished(m_front_paw) &&
	   m_movement->is_sequence_finished(m_middle_paw) &&
	   m_movement->is_sequence_finished(m_back_paw))
		return SEQUENCE_FINISH;
	else
		return SEQUENCE_IN_PROGRESS;
}

void Side::move_paw(Paw &paw)
{
	paw.valid_move();
	m_module.set_off_time(channel_table[paw.m_position][position_tibia], paw.servos_time_table[position_tibia]);
	m_module.set_off_time(channel_table[paw.m_position][position_femur], paw.servos_time_table[position_femur]);
	m_module.set_off_time(channel_table[paw.m_position][position_coxa],  paw.servos_time_table[position_coxa]);
}

void Side::memorize_movement(Movement *mvt)
{
	m_movement = mvt;
}

double Side::get_real_distance()
{
	float real_dist = std::min(m_movement->determine_real_distance(m_front_paw), m_movement->determine_real_distance(m_middle_paw));
	real_dist = std::min(real_dist, m_movement->determine_real_distance(m_back_paw));
	return real_dist;
}

int Side::get_max_sequence_number()
{
	int seq_front = m_front_paw.get_active_sequence_number();
	int seq_middle = m_middle_paw.get_active_sequence_number();
	int seq_back = m_back_paw.get_active_sequence_number();

	seq_middle = max(seq_front, seq_middle);
	seq_back = max(seq_middle, seq_back);
	return(seq_back);
}
