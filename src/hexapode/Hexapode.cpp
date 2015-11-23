//============================================================================
// Name        : Hexapode.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Hexapode.h"
#include "bcm2835.h"
#include "Linear_movement.h"
#include "No_movement.h"
#include "complete_linear_movement.h"

const int SEQUENCE_NUMBER = 3;

Hexapode::Hexapode() : m_i2c(new i2cdev),
					   m_left_side(side_left, m_i2c), m_right_side(side_right, m_i2c),
					   m_movement(NULL), // initialiser à no_movement
					   m_current_sequence_number(0), //remettre -1
					   m_current_step_number(0),
					   m_step_number(1)

{
	bcm2835_gpio_clr(7);
}

Hexapode::~Hexapode()
{
	delete m_i2c;
	delete m_movement;
}

void Hexapode::run()
{
	m_timer.reset();

	move(new No_movement());
	cout << "toggle" <<endl;
	toggle();
	cout << "fin" <<endl;
	//move(new Linear_movement(direction_back, 50, 30));
	move(new complete_linear_movement(135, 40, 20));

	int cpt = 0;
	while(1)
	{
		if(m_timer.elapsed().millis() >= 20.0)
		{
			m_timer.reset();

			if(!update())
				toggle();
				//doris.move({linear, direction_front, 100, 0, 50});
			cpt++;
		}

		/*if(cpt == 310)
			move(new Linear_movement(direction_front, 50, 75));*/
	}
}

void Hexapode::calibrate_servomotors(double x, double y, double z)
{
	Angles angles;
	int time3, time2, time1;

	angles = m_left_side.get_front_paw().move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.get_front_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.get_front_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.get_front_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel0, time3);
	m_left_side.get_module().set_off_time(channel1, time2);
	m_left_side.get_module().set_off_time(channel2, time1);

	angles = m_left_side.get_front_paw().move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.get_middle_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.get_middle_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.get_middle_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel3, time3);
	m_left_side.get_module().set_off_time(channel4, time2);
	m_left_side.get_module().set_off_time(channel5, time1);

	angles = m_left_side.get_front_paw().move(x, y, z);
	time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + m_left_side.get_back_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + m_left_side.get_back_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + m_left_side.get_back_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel6, time3);
	m_left_side.get_module().set_off_time(channel7, time2);
	m_left_side.get_module().set_off_time(channel8, time1);

	angles = m_right_side.get_front_paw().move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.get_front_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.get_front_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.get_front_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel0, time3);
	m_right_side.get_module().set_off_time(channel1, time2);
	m_right_side.get_module().set_off_time(channel2, time1);

	angles = m_right_side.get_front_paw().move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.get_middle_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.get_middle_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.get_middle_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel3, time3);
	m_right_side.get_module().set_off_time(channel4, time2);
	m_right_side.get_module().set_off_time(channel5, time1);

	angles = m_right_side.get_front_paw().move(x, -y, z);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + m_right_side.get_back_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + m_right_side.get_back_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + m_right_side.get_back_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel6, time3);
	m_right_side.get_module().set_off_time(channel7, time2);
	m_right_side.get_module().set_off_time(channel8, time1);
}

void Hexapode::toggle(bool not_change)
{
	if(!not_change)
	{
		m_current_sequence_number++;
		if(m_current_sequence_number >= SEQUENCE_NUMBER)
			m_current_sequence_number = 0;

		m_current_step_number = 0;
	}

	double real_distance_left  = m_left_side.change_sequence_number(m_current_sequence_number, m_current_step_number);
	double real_distance_right = m_right_side.change_sequence_number(m_current_sequence_number, m_current_step_number);
	double min_distance = min(real_distance_left, real_distance_right);
	//cout << " min d " << min_distance << endl;
	if(min_distance != (m_movement->m_distance / 2))
		m_movement->m_corrected_distance = min_distance;
	else
		m_movement->m_corrected_distance = m_movement->m_distance;
	m_movement->compute_variables();
}

void Hexapode::move(Movement *mvt)
{
	/* TODO :
	 * 	compute the distance for each side for circular mvt
	 */
	//toggle();
	if(m_movement != nullptr)
		delete m_movement;
	else
		m_step_number =  mvt->m_step_number;

	m_movement = mvt;
	if(m_step_number != m_movement->m_step_number)
	{
		m_current_step_number = (m_movement->m_step_number / m_step_number) * m_current_step_number;
		m_step_number = m_movement->m_step_number;
	}
	m_left_side.memorize_movement(mvt, m_current_step_number);
	m_right_side.memorize_movement(mvt, m_current_step_number);

	toggle(true);
}

int Hexapode::update()
{
	int result_right, result_left;
	result_right = m_right_side.update(m_current_sequence_number);
	result_left  = m_left_side.update(m_current_sequence_number);
	m_current_step_number ++;
	m_movement->m_current_step_number = m_current_step_number;
	return result_right & result_left;
}

