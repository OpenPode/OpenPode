//============================================================================
// Name        : Hexapode.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "hexapode/Hexapode.h"
#include "drivers/broadcom/bcm2835.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include "utility/math_utils.h"
#include <cmath>

Hexapode::Hexapode() : m_i2c(new i2cdev),
					   m_left_side(side_left, m_i2c, &m_error_detection, sequence_of_paws[1]),
					   m_right_side(side_right, m_i2c, &m_error_detection, sequence_of_paws[0]),
					   m_movement(nullptr), // init to no_movement
					   m_current_sequence_number(0), m_sequence_number(0),
					   m_current_step_number(0), m_step_number(1),
					   m_error_actions(&m_controller)
#ifdef HEAD
					   ,m_led_right(RPI_BPLUS_GPIO_J8_40,RPI_BPLUS_GPIO_J8_38,RPI_BPLUS_GPIO_J8_36, BLUE),
					   m_led_left(RPI_BPLUS_GPIO_J8_15,RPI_BPLUS_GPIO_J8_13,RPI_BPLUS_GPIO_J8_11, BLUE),
					   last_led(0)
#endif
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

			determine_movement();

			prepare_update();

#ifdef ERROR_ACTION
			if(must_do_error_action())//in progress
				error_action();
			else
#endif
				standard_action();
			//std::cout << m_timer.elapsed().millis() << std::endl;

#ifdef HEAD
			manage_led(m_error_detection.error_code);
#endif
		}
		usleep(500);
	}
}

bool Hexapode::must_do_error_action()
{
	if(m_error_detection.is_on_error())
	{
		if((m_movement->m_type == no_movement) && m_error_detection.is_on_sequence())
			return true;
		else if(m_movement->m_type != no_movement)
			return false;
		else
			return false;
	}
	else
		return false;
}

void Hexapode::init()
{
	m_controller.go_back_to_default_position();
	define_nb_sequence();
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
	if(update())
	{
		toggle(); //if sequence is finished
	}
}

void Hexapode::error_action()
{
	m_movement->in_correction = true;
	m_movement->set_paw_spreading_step(180);
	do
	{
		m_error_actions.resolve_error(m_movement->m_type, m_error_detection.is_on_error());
		m_error_actions.set_parameters_on_movement_controller();
		m_movement->memorize_parameters(m_current_sequence_number, m_controller.get_incline_coef(), m_controller.get_paw_spreading());
		set_parameters_on_movement();
		prepare_update();
	} while(m_error_actions.is_resolving());
	m_movement->in_correction = false;
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

	if(min_distance > (m_movement->m_distance / (float)(m_sequence_number-1)))
		m_movement->m_corrected_distance = min_distance*(float)(m_sequence_number-1);
	else
		m_movement->m_corrected_distance = m_movement->m_distance;
}

void Hexapode::update_sequence_number()
{
	m_current_sequence_number++;
	if(m_current_sequence_number >= m_sequence_number)
		m_current_sequence_number = 0;

	m_current_step_number = 0;
	m_movement->raz_current_step_number();
	m_movement->update_sequence_number(m_current_sequence_number);
	m_error_detection.set_sequence_number(m_current_sequence_number);
}

void Hexapode::toggle()
{
	update_sequence_number();

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
	m_movement->set_number_of_sequence(m_sequence_number);

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
	m_error_detection.test_error();
}

int Hexapode::update()
{
	bool sequence_end_right, sequence_end_left;
	sequence_end_right = m_right_side.update();
	sequence_end_left  = m_left_side.update();

	m_current_step_number ++;
	m_movement->increase_current_step_number();

	return (sequence_end_right && sequence_end_left); //if both sequence are finished
}

#ifdef HEAD
void Hexapode::manage_led(char error_code)
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
	else
	{
		m_led_right.set_standard();
		m_led_left.set_standard();
	}

#ifdef ERROR_ACTION
	if(error_code&SIDE_LEFT)
		m_led_left.set_debug(error);

	if(error_code&SIDE_RIGHT)
		m_led_right.set_debug(error);
#endif

	last_led = led;
}
#endif

void Hexapode::define_nb_sequence()
{
	int left_seq_number = m_left_side.get_max_sequence_number();
	int right_seq_number = m_right_side.get_max_sequence_number();
	m_sequence_number = max(left_seq_number, right_seq_number) + 1;
	std::cout << m_sequence_number << " sequences" << std::endl;
	if(m_sequence_number < 2)
	{
		std::cout << "not enough sequences" << std::endl;
		exit(-1);
	}
	else if(m_sequence_number == 2)
		std::cout << "two sequences can be dangerous for motors" << std::endl;
}

//for calibration
void Hexapode::calibrate_servomotors(float x, float y, float z)
{
	int time[3];
	float position[3] = {x,y,z};

	m_left_side.get_front_paw().prepare_to_move(position);
	calibrate_one_paw(m_left_side.get_front_paw(), time);
	m_left_side.get_module().set_off_time(channel0, time[2]);
	m_left_side.get_module().set_off_time(channel1, time[1]);
	m_left_side.get_module().set_off_time(channel2, time[0]);

	m_left_side.get_middle_paw().prepare_to_move(position);
	calibrate_one_paw(m_left_side.get_middle_paw(), time);
	m_left_side.get_module().set_off_time(channel3, time[2]);
	m_left_side.get_module().set_off_time(channel4, time[1]);
	m_left_side.get_module().set_off_time(channel5, time[0]);

	m_left_side.get_back_paw().prepare_to_move(position);
	calibrate_one_paw(m_left_side.get_back_paw(), time);
	m_left_side.get_module().set_off_time(channel6, time[2]);
	m_left_side.get_module().set_off_time(channel7, time[1]);
	m_left_side.get_module().set_off_time(channel8, time[0]);

	position[1] = -y;

	m_right_side.get_front_paw().prepare_to_move(position);
	calibrate_one_paw(m_right_side.get_front_paw(), time);
	m_right_side.get_module().set_off_time(channel0, time[2]);
	m_right_side.get_module().set_off_time(channel1, time[1]);
	m_right_side.get_module().set_off_time(channel2, time[0]);

	m_right_side.get_middle_paw().prepare_to_move(position);
	calibrate_one_paw(m_right_side.get_middle_paw(), time);
	m_right_side.get_module().set_off_time(channel3, time[2]);
	m_right_side.get_module().set_off_time(channel4, time[1]);
	m_right_side.get_module().set_off_time(channel5, time[0]);

	m_right_side.get_back_paw().prepare_to_move(position);
	calibrate_one_paw(m_right_side.get_back_paw(), time);
	m_right_side.get_module().set_off_time(channel6, time[2]);
	m_right_side.get_module().set_off_time(channel7, time[1]);
	m_right_side.get_module().set_off_time(channel8, time[0]);
}

void Hexapode::calibrate_one_paw(Paw &paw, int time[3])
{
	Angles angles;

	angles.theta3 = paw.m_servo_angles.theta3;
	angles.theta2 = paw.m_servo_angles.theta2;
	angles.theta1 = paw.m_servo_angles.theta1;
	time[2] = static_cast<int>(- paw.m_side_coef * (util::to_deg(angles.theta3)+90.f) * Servo::resolution + paw.m_tibia.get_offset());
	time[1] = static_cast<int>(  paw.m_side_coef * (util::to_deg(angles.theta2)) * Servo::resolution      + paw.m_femur.get_offset());
	time[0] = static_cast<int>(-(util::to_deg(angles.theta1) - paw.m_side_coef*90.f) * Servo::resolution  + paw.m_coxa.get_offset());
}
