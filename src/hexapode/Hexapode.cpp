//============================================================================
// Name        : Hexapode.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "hexapode/Hexapode.h"
#include "i2c/bcm2835.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include <cmath>

constexpr int SEQUENCE_NUMBER = 3;

Hexapode::Hexapode() : m_i2c(new i2cdev),
					   m_left_side(side_left, m_i2c, &m_error_detection), m_right_side(side_right, m_i2c, &m_error_detection),
					   m_movement(nullptr), // init to no_movement
					   m_current_sequence_number(1), //remettre -1
					   m_current_step_number(0),
					   m_step_number(1),
					   m_error_actions(&m_controller),
					   m_led_right(RPI_BPLUS_GPIO_J8_40,RPI_BPLUS_GPIO_J8_38,RPI_BPLUS_GPIO_J8_36, BLUE),
					   m_led_left(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_13,RPI_BPLUS_GPIO_J8_11, BLUE),
					   last_led(0)
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
	init();

	while(1)
	{
		m_controller.run_controller();
		if(m_timer.elapsed().millis() >= 20.0f)
		{
			m_timer.reset();

			//if(m_error_actions.have_finished_to_corrected_error())
				determine_movement();

			prepare_update();

			if(!m_error_detection.is_on_error())//in progress
			{
				standard_action();
			}
			else
			{
				error_action();
			}
			//std::cout << m_timer.elapsed().millis() << std::endl;

			manage_led();
		}
	}
}

void Hexapode::init()
{
	m_controller.go_back_to_default_position();
	move(m_controller.get_movement());
	toggle();
	m_timer.reset();
}

void Hexapode::determine_movement()
{
	m_controller.get_new_movement(m_current_step_number, m_step_number);

	//distibute parameters
	m_error_actions.purpose_new_parameters(m_controller.get_pitch_stick(), m_controller.get_roll_stick(),
										   m_controller.get_center_height(), m_controller.get_paw_spreading());
	set_parameters_on_movement();

	if(m_controller.is_a_new_movement())
	{
		move(m_controller.get_movement());
	}
}

void Hexapode::set_parameters_on_movement()
{
	m_movement->memorize_parameters(m_current_sequence_number, m_controller.get_incline_coef(), m_controller.get_paw_spreading());
}

void Hexapode::standard_action()
{
	m_error_actions.valid_parameters_no_error();
	if(!update())
	{
		toggle(); //if sequence is finished
	}
}

void Hexapode::error_action()
{
	m_movement->set_paw_spreading_step(180);
	do
	{
		m_error_actions.resolve_error(m_movement->m_type, m_error_detection.is_on_error());
		m_error_actions.set_parameters_on_movement_controller();
		m_movement->memorize_parameters(m_current_sequence_number, m_controller.get_incline_coef(), m_controller.get_paw_spreading());
		set_parameters_on_movement();
		prepare_update();
	} while(m_error_actions.is_resolving());
	m_movement->set_paw_spreading_step();
	set_parameters_on_movement();
	prepare_update();
	//std::cout << "error code " << (int)(m_error_detection.error_code) << std::endl;
	m_error_actions.valid_parameters();
	update();
}

void Hexapode::determine_real_distance_for_movement()
{
	float real_distance_left  = m_left_side.get_real_distance();
	float real_distance_right = m_right_side.get_real_distance();
	float min_distance = min(real_distance_left, real_distance_right);

	if(min_distance > (m_movement->m_distance / 2.f))
		m_movement->m_corrected_distance = min_distance*2.f;
	else
		m_movement->m_corrected_distance = m_movement->m_distance;
}

void Hexapode::update_sequence_number()
{
	m_current_sequence_number++;
	if(m_current_sequence_number >= SEQUENCE_NUMBER)
		m_current_sequence_number = 0;

	m_current_step_number = 0;
	m_movement->raz_current_step_number();
	m_movement->update_sequence_number(m_current_sequence_number);
	m_error_detection.set_sequence_number(m_current_sequence_number);
}

void Hexapode::toggle()
{
	update_sequence_number();

	m_left_side.memorize_current_paw_position();
	m_right_side.memorize_current_paw_position();

	determine_real_distance_for_movement();
	m_movement->compute_variables();

}

void Hexapode::move(Movement *mvt)
{
	/* TODO :
	 * 	compute the distance for each side for circular mvt
	 */
	if(m_movement == nullptr)//for init
		m_step_number =  mvt->m_step_number;

	m_movement = mvt;

	if(m_step_number != m_movement->m_step_number)//if the previous isn't the same of the new one
	{
		//create an approximation of the current step number
		m_current_step_number = m_movement->m_step_number * m_current_step_number / m_step_number;
		m_step_number = m_movement->m_step_number;
	}
	m_left_side.memorize_movement(mvt);
	m_right_side.memorize_movement(mvt);
	m_movement->update_current_step_number(m_current_step_number);

	determine_real_distance_for_movement();
	m_movement->compute_variables();

}

void Hexapode::prepare_update()
{
	m_error_detection.reset();
	m_right_side.prepare_update();
	m_left_side.prepare_update();
}

int Hexapode::update()
{
	int sequence_end_right, sequence_end_left;
	sequence_end_right = m_right_side.update();
	sequence_end_left  = m_left_side.update();

	m_current_step_number ++;
	m_movement->increase_current_step_number();

	return sequence_end_right & sequence_end_left; //if both sequence are finished
}

void Hexapode::manage_led()
{
	int led = m_controller.get_led();
	if((led == 2) && (last_led == 0))
	{
		m_led_right--;
		m_led_left--;
	}
	else if ((led == 1) && (last_led == 0))
	{
		m_led_right++;
		m_led_left++;
	}

	last_led = led;
}

//for calibration
void Hexapode::calibrate_servomotors(float x, float y, float z)
{
	Angles angles;
	int time3, time2, time1;
	float position[3] = {x,y,z};

	m_left_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( -(angles.theta3*(180./M_PI)+90)*1.97 + m_left_side.get_front_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./M_PI)*1.97     + m_left_side.get_front_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_left_side.get_front_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel0, time3);
	m_left_side.get_module().set_off_time(channel1, time2);
	m_left_side.get_module().set_off_time(channel2, time1);

	m_left_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( -(angles.theta3*(180./M_PI)+90)*1.97 + m_left_side.get_middle_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./M_PI)*1.97     + m_left_side.get_middle_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_left_side.get_middle_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel3, time3);
	m_left_side.get_module().set_off_time(channel4, time2);
	m_left_side.get_module().set_off_time(channel5, time1);

	m_left_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( -(angles.theta3*(180./M_PI)+90)*1.97 + m_left_side.get_back_paw().m_tibia.get_offset());
	time2 = static_cast<int>( angles.theta2*(180./M_PI)*1.97     + m_left_side.get_back_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_left_side.get_back_paw().m_coxa.get_offset());
	m_left_side.get_module().set_off_time(channel6, time3);
	m_left_side.get_module().set_off_time(channel7, time2);
	m_left_side.get_module().set_off_time(channel8, time1);

	position[1] = -y;

	m_right_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( (angles.theta3*(180./M_PI)+90)*1.97 + m_right_side.get_front_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./M_PI)*1.97     + m_right_side.get_front_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_right_side.get_front_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel0, time3);
	m_right_side.get_module().set_off_time(channel1, time2);
	m_right_side.get_module().set_off_time(channel2, time1);

	m_right_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( (angles.theta3*(180./M_PI)+90)*1.97 + m_right_side.get_middle_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./M_PI)*1.97     + m_right_side.get_middle_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_right_side.get_middle_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel3, time3);
	m_right_side.get_module().set_off_time(channel4, time2);
	m_right_side.get_module().set_off_time(channel5, time1);

	m_right_side.get_front_paw().prepare_to_move(position);
	angles.theta3 = m_left_side.get_front_paw().m_servo_angles.theta3;
	angles.theta2 = m_left_side.get_front_paw().m_servo_angles.theta2;
	angles.theta1 = m_left_side.get_front_paw().m_servo_angles.theta1;
	time3 = static_cast<int>( (angles.theta3*(180./M_PI)+90)*1.97 + m_right_side.get_back_paw().m_tibia.get_offset());
	time2 = static_cast<int>( -angles.theta2*(180./M_PI)*1.97     + m_right_side.get_back_paw().m_femur.get_offset());
	time1 = static_cast<int>(-(angles.theta1*(180./M_PI)-90)*1.97 + m_right_side.get_back_paw().m_coxa.get_offset());
	m_right_side.get_module().set_off_time(channel6, time3);
	m_right_side.get_module().set_off_time(channel7, time2);
	m_right_side.get_module().set_off_time(channel8, time1);
}

