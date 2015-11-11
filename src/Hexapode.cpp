//============================================================================
// Name        : Hexapode.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Hexapode.h"
#include "bcm2835.h"

Hexapode::Hexapode() : m_i2c(new i2cdev),
					   m_left_side(side_left, m_i2c), m_right_side(side_right, m_i2c),
					   m_current_sequence_number(0) //remettre -1
{
	bcm2835_gpio_clr(7);
}

Hexapode::~Hexapode()
{
	delete m_i2c;
}

void Hexapode::run()
{
	m_timer.reset();
	toggle();

	move({linear, direction_back, 50, 0, 12});

	while(1)
	{
		if(m_timer.elapsed().millis() >= 20.0)
		{
			m_timer.reset();

			if(!update())
				toggle();
				//doris.move({linear, direction_front, 100, 0, 50});

		}
	}
}

void Hexapode::calibrate_servomotors(double x, double y, double z)
{
	Angles angles;
	int time3, time2, time1;

	angles = m_left_side.m_front_paw.move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.m_front_paw.m_servo3.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.m_front_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.m_front_paw.m_servo1.get_offset());
	m_left_side.m_module.set_off_time(channel0, time3);
	m_left_side.m_module.set_off_time(channel1, time2);
	m_left_side.m_module.set_off_time(channel2, time1);

	angles = m_left_side.m_front_paw.move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.m_middle_paw.m_servo3.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.m_middle_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.m_middle_paw.m_servo1.get_offset());
	m_left_side.m_module.set_off_time(channel3, time3);
	m_left_side.m_module.set_off_time(channel4, time2);
	m_left_side.m_module.set_off_time(channel5, time1);

	angles = m_left_side.m_front_paw.move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.m_back_paw.m_servo3.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.m_back_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.m_back_paw.m_servo1.get_offset());
	m_left_side.m_module.set_off_time(channel6, time3);
	m_left_side.m_module.set_off_time(channel7, time2);
	m_left_side.m_module.set_off_time(channel8, time1);

	angles = m_right_side.m_front_paw.move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.m_front_paw.m_servo3.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.m_front_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.m_front_paw.m_servo1.get_offset());
	m_right_side.m_module.set_off_time(channel0, time3);
	m_right_side.m_module.set_off_time(channel1, time2);
	m_right_side.m_module.set_off_time(channel2, time1);

	angles = m_right_side.m_front_paw.move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.m_middle_paw.m_servo3.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.m_middle_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.m_middle_paw.m_servo1.get_offset());
	m_right_side.m_module.set_off_time(channel3, time3);
	m_right_side.m_module.set_off_time(channel4, time2);
	m_right_side.m_module.set_off_time(channel5, time1);

	angles = m_right_side.m_front_paw.move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.m_back_paw.m_servo3.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.m_back_paw.m_servo2.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.m_back_paw.m_servo1.get_offset());
	m_right_side.m_module.set_off_time(channel6, time3);
	m_right_side.m_module.set_off_time(channel7, time2);
	m_right_side.m_module.set_off_time(channel8, time1);
}

void Hexapode::toggle()
{
	if(m_current_sequence_number == 0)
		m_current_sequence_number = 1;
	else if(m_current_sequence_number == 1)
		m_current_sequence_number = 2;
	else
		m_current_sequence_number = 0;

	double real_distance_left  = m_left_side.change_sequence_number(m_current_sequence_number);
	double real_distance_right = m_right_side.change_sequence_number(m_current_sequence_number);
	double min_distance = min(real_distance_left, real_distance_right);
	if(min_distance != (m_mvt.distance / 2))
	{
		m_left_side.m_movement.corrected_distance = min_distance;
		m_right_side.m_movement.corrected_distance = min_distance;
	}
	else
	{
		m_left_side.m_movement.corrected_distance = m_mvt.distance;
		m_right_side.m_movement.corrected_distance = m_mvt.distance;
	}
}

void Hexapode::move(Movement mvt)
{
	/* TODO :
	 * 	compute the distance for each side for circular mvt
	 */
	//toggle();
	m_mvt = mvt;
	m_left_side.memorize_movement(mvt);
	m_right_side.memorize_movement(mvt);
}

int Hexapode::update()
{
	int result_right, result_left;
	result_right = m_right_side.update(m_current_sequence_number);
	result_left  = m_left_side.update(m_current_sequence_number);
	return result_right & result_left;
}

