/*
 * Hexapode.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef HEXAPODE_H_
#define HEXAPODE_H_

#include "drivers/i2c/i2cdev.h"
#include "side/Side.h"
#include "utility/time/timer_t.hpp"
#include "controller/Movement_controller.h"
#include "error_management/Error_detection.h"
#include "error_management/Error_actions.h"
#include "drivers/led/led_controller.h"
#include "config.h"

class Movement;

class Hexapode
{
public:

	Hexapode();
	~Hexapode();

	void run();
	void calibrate_servomotors(float x, float y, float z);

	void move(Movement *mvt);

	void prepare_update();
	int update();

	void determine_real_distance_for_movement();
	void toggle();
	void update_sequence_number();
	void define_nb_sequence();

private:

	i2cdev *m_i2c;

	util::timer_t<float> m_timer;

	Side m_left_side;
	Side m_right_side;

	Movement *m_movement;
	int m_current_sequence_number;
	int m_sequence_number;
	int m_current_step_number;
	int m_step_number;

	Movement_controller m_controller;
	Error_detection m_error_detection;
	Error_actions m_error_actions;
#ifdef HEAD
	Led_controller m_led_right;
	Led_controller m_led_left;
	int last_led;
	void manage_led(uint8_t error_code = 0x00);
#endif

	void init();
	void determine_movement();
	void set_parameters_on_movement();
	bool must_do_error_action();
	void standard_action();
	void error_action();
};

#endif /* HEXAPODE_H_ */
