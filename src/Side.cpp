/*
 * Side.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */


#include "Side.h"
#include "hexapode_dimensions.h"
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
										  m_step_distance_x(0),
										  m_step_distance_z(0),
										  m_current_step_number(0)
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

	m_current_sequence_number = -1;
}

void Side::memorize_movement(Movement mvt)
{
	m_movement = mvt;

	switch(mvt.type)
	{
	case linear:
		m_step_distance_z = mvt.distance / 2.0 / mvt.step_number;
		m_step_distance_x = mvt.corrected_distance / 2.0 / mvt.step_number;
		break;
	case rotation_left:
		break;
	case rotation_right:
		break;
	case no_movement:
		break;
	}
}

int Side::update(int sequence_number, double a, double b, double paw_spreading) //ax+b for height
{
	m_current_step_number++;

	m_step_distance_z = m_movement.distance / 2.0 / m_movement.step_number;
	m_step_distance_x = m_movement.corrected_distance / 2.0 / m_movement.step_number;

	determine_paws_position(sequence_number, a, b, paw_spreading);

	move_paw(m_front_paw, m_paw_position[position_front]);
	move_paw(m_middle_paw, m_paw_position[position_middle]);
	move_paw(m_back_paw, m_paw_position[position_back]);

	// for test purpose
	if(m_current_step_number == m_movement.step_number)
		return 0;
	else
		return 1;
}

double Side::reproach_position(double present, double futur)
{
	if((present - futur) <= -m_step_distance_z)
	{
		return(present + m_step_distance_z);
	}
	else if((present - futur) >= m_step_distance_z)
	{
		return(present - m_step_distance_z);
	}
	else
	{
		return futur;
	}
}

double Side::goto_position(double present, double futur, double nb_step)
{
	double step_distance = abs(present - futur) / nb_step;

	if(futur > present)
	{
		return(present + step_distance);
	}
	else if(futur < present)
	{
		return(present - step_distance);
	}
	else
	{
		return futur;
	}
}

void Side::determine_y_paws_position(int sequence_number, double paw_spreading)
{
	m_paw_position[position_front][coord_y] = m_front_paw.m_current_coords.y;
	m_paw_position[position_middle][coord_y] = m_middle_paw.m_current_coords.y;
	m_paw_position[position_back][coord_y] = m_back_paw.m_current_coords.y;

	if(sequence_number == 0)
	{
		if(m_side == side_left)
			m_paw_position[position_front][coord_y]  = reproach_position(m_front_paw.m_current_coords.y, m_side_coef * paw_spreading);
		else
			m_paw_position[position_back][coord_y]   = reproach_position(m_back_paw.m_current_coords.y, m_side_coef * paw_spreading);
	}
	else if(sequence_number == 1)
	{
		if(m_side == side_left)
			m_paw_position[position_middle][coord_y] = reproach_position(m_middle_paw.m_current_coords.y, m_side_coef * (paw_spreading - 0));
		else
			m_paw_position[position_middle][coord_y] = reproach_position(m_middle_paw.m_current_coords.y, m_side_coef * (paw_spreading - 0));
	}
	else
	{
		if(m_side == side_left)
			m_paw_position[position_back][coord_y]   = reproach_position(m_back_paw.m_current_coords.y, m_side_coef * paw_spreading);
		else
			m_paw_position[position_front][coord_y]  = reproach_position(m_front_paw.m_current_coords.y, m_side_coef * paw_spreading);
	}
}

double Side::get_up_paw(double final_height, Paw &paw)
{
	double z;

	if(m_current_step_number <= (m_movement.step_number / 2))
	{
		if(paw.m_current_coords.z <= -30)
			z = paw.m_current_coords.z + m_step_distance_z*2;
		else
			z = paw.m_current_coords.z;
	}
	else
	{
		double step_distance_get_up = abs((final_height - paw.m_current_coords.z) / (m_movement.step_number - m_current_step_number));
		z = paw.m_current_coords.z - step_distance_get_up;
	}

	return z;
}

void Side::determine_z_paws_position(int sequence_number, double a, double b)
{
	if(sequence_number == 0)
	{
		if(m_side == side_left)
		{
			m_paw_position[position_front][coord_z]  = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2 + HALF_LENGTH) + b, m_front_paw);
			m_paw_position[position_middle][coord_z] = a*m_paw_position[position_middle][coord_x] + b;
			m_paw_position[position_back][coord_z]   = a*(m_paw_position[position_back][coord_x] - HALF_LENGTH) + b;
		}
		else
		{
			m_paw_position[position_front][coord_z]  = a*(m_paw_position[position_front][coord_x] + HALF_LENGTH) + b;
			m_paw_position[position_middle][coord_z] = a*m_paw_position[position_middle][coord_x] + b;
			m_paw_position[position_back][coord_z]   = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2 - HALF_LENGTH) + b, m_back_paw);
		}
	}
	else if(sequence_number == 1)
	{
		if(m_side == side_left)
		{
			m_paw_position[position_front][coord_z]  = a*(m_paw_position[position_front][coord_x] + HALF_LENGTH) + b;
			m_paw_position[position_middle][coord_z] = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2) + b, m_middle_paw);
			m_paw_position[position_back][coord_z]   = a*(m_paw_position[position_back][coord_x] - HALF_LENGTH) + b;
		}
		else
		{
			m_paw_position[position_front][coord_z]  = a*(m_paw_position[position_front][coord_x] + HALF_LENGTH) + b;
			m_paw_position[position_middle][coord_z] = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2) + b, m_middle_paw);
			m_paw_position[position_back][coord_z]   = a*(m_paw_position[position_back][coord_x] - HALF_LENGTH) + b;
		}
	}
	else
	{
		if(m_side == side_left)
		{
			m_paw_position[position_front][coord_z]  = a*(m_paw_position[position_front][coord_x] + HALF_LENGTH) + b;
			m_paw_position[position_middle][coord_z] = a*m_paw_position[position_middle][coord_x] + b;
			m_paw_position[position_back][coord_z]   = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2 - HALF_LENGTH) + b, m_back_paw);
		}
		else
		{
			m_paw_position[position_front][coord_z]  = get_up_paw(a*(m_front_paw.m_x_center + m_movement.distance / 2 + HALF_LENGTH) + b, m_front_paw);
			m_paw_position[position_middle][coord_z] = a*m_paw_position[position_middle][coord_x] + b;
			m_paw_position[position_back][coord_z]   = a*(m_paw_position[position_back][coord_x] - HALF_LENGTH) + b;
		}
	}
}

void Side::determine_paws_position(int sequence_number, double a, double b, double paw_spreading) //ax+b for the height
{
	m_paw_position[position_front][coord_x]  = m_paws_position.front_paw.x  - m_movement.direction * m_step_distance_x * m_current_step_number;
	m_paw_position[position_middle][coord_x] = m_paws_position.middle_paw.x - m_movement.direction * m_step_distance_x * m_current_step_number;
	m_paw_position[position_back][coord_x]   = m_paws_position.back_paw.x   - m_movement.direction * m_step_distance_x * m_current_step_number;

	if(m_side == side_left)
	{
		if(sequence_number == 0)
			m_paw_position[position_front][coord_x]  = goto_position(m_front_paw.m_current_coords.x, m_front_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position[position_middle][coord_x] = goto_position(m_middle_paw.m_current_coords.x, m_middle_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
		else
			m_paw_position[position_back][coord_x]   = goto_position(m_back_paw.m_current_coords.x, m_back_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
	}
	else
	{
		if(sequence_number == 0)
			m_paw_position[position_back][coord_x]   = goto_position(m_back_paw.m_current_coords.x, m_back_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position[position_middle][coord_x] = goto_position(m_middle_paw.m_current_coords.x, m_middle_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
		else
			m_paw_position[position_front][coord_x]  = goto_position(m_front_paw.m_current_coords.x, m_front_paw.m_x_center + m_movement.direction * m_movement.distance / 2, m_movement.step_number - m_current_step_number);
	}
	determine_y_paws_position(sequence_number, paw_spreading);
	determine_z_paws_position(sequence_number, a, b);
}

void Side::move_paw(Paw &paw, double coords[3])
{
	Angles angles = paw.move(coords[coord_x], coords[coord_y], coords[coord_z]);

	int time_tibias = static_cast<int>(- m_side_coef * (angles.theta3*(180./M_PI)+90.) * Servo::resolution + paw.m_servo3.get_offset());
	int time_femur  = static_cast<int>(  m_side_coef *  angles.theta2*(180./M_PI) * Servo::resolution      + paw.m_servo2.get_offset());
	int time_coxa   = static_cast<int>(-(angles.theta1*(180./M_PI) - m_side_coef*90.) * Servo::resolution  + paw.m_servo1.get_offset());

	if(paw.m_position == position_middle)
	{
		cout << "tib : " << - m_side_coef * (angles.theta3*(180./M_PI)+90.) * Servo::resolution + paw.m_servo3.get_offset() << endl;
		cout << "fem : " << m_side_coef *  angles.theta2*(180./M_PI) * Servo::resolution      + paw.m_servo2.get_offset() << endl;
		cout << "cox : " << -(angles.theta1*(180./M_PI) - m_side_coef*90.) * Servo::resolution  + paw.m_servo1.get_offset() << endl;
	}

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

double Side::change_sequence_number(int sequence_number)
{
	m_current_sequence_number = sequence_number;
	m_current_step_number = 0;
	memorize_current_paw_position();
	return determine_real_distance();
}

double Side::determine_real_distance()
{
	double real_distance[3];
	real_distance[position_front]  = (m_paws_position.front_paw.x - m_movement.direction * (m_front_paw.m_x_center - m_movement.distance / 2));
	real_distance[position_middle] = (m_paws_position.middle_paw.x - m_movement.direction * (m_middle_paw.m_x_center - m_movement.distance / 2));
	real_distance[position_back]   = (m_paws_position.back_paw.x - m_movement.direction * (m_back_paw.m_x_center - m_movement.distance / 2));

	if(m_side == side_left)
	{
		if(m_current_sequence_number == 0)
			real_distance[position_front]  = m_movement.direction * m_movement.distance;
		else if(m_current_sequence_number == 1)
			real_distance[position_middle] = m_movement.direction * m_movement.distance;
		else
			real_distance[position_back]   = m_movement.direction * m_movement.distance;
	}
	else
	{
		if(m_current_sequence_number == 0)
			real_distance[position_back]   = m_movement.direction * m_movement.distance;
		else if(m_current_sequence_number == 1)
			real_distance[position_middle] = m_movement.direction * m_movement.distance;
		else
			real_distance[position_front]  = m_movement.direction * m_movement.distance;
	}

	if(m_movement.direction * real_distance[position_front] < 0)
		real_distance[position_front] = 0;
	if(m_movement.direction * real_distance[position_middle] < 0)
		real_distance[position_middle] = 0;
	if(m_movement.direction * real_distance[position_back] < 0)
		real_distance[position_back] = 0;

	real_distance[position_middle] = min(abs(real_distance[position_front]), abs(real_distance[position_middle]));
	return (min(abs(real_distance[position_middle]), abs(real_distance[position_back])));
}

