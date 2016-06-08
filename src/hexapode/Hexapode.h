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

public:

	i2cdev *m_i2c;

	util::timer_t<float> m_timer;

	Side m_left_side;
	Side m_right_side;

	Movement *m_movement;
	int m_current_sequence_number;
	int m_current_step_number;
	int m_step_number;

	Movement_controller m_controller;
	Error_detection m_error_detection;
	Error_actions m_error_actions;
	led_controller m_led_right;
	led_controller m_led_left;
	int last_led;

	void init();
	void determine_movement();
	void set_parameters_on_movement();
	void standard_action();
	void error_action();
	void manage_led(char error_code = 0x00);
};

#endif /* HEXAPODE_H_ */
