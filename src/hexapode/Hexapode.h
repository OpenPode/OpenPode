/*
 * Hexapode.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef HEXAPODE_H_
#define HEXAPODE_H_

#include "i2cdev.h"
#include "Side.h"
#include "timer_t.hpp"
#include "Movement_controller.h"

class Movement;

class Hexapode
{
public:

	Hexapode();
	~Hexapode();

	void run();
	void calibrate_servomotors(double x, double y, double z);

	void move(Movement *mvt);

	int update(double a= 0, double b=0, double c=-50, double paw_spreading=90);

	void determine_real_distance_for_movement();
	void toggle();

public:

	i2cdev *m_i2c;

	util::timer_t m_timer;

	Side m_left_side;
	Side m_right_side;

	Movement *m_movement;
	int m_current_sequence_number;
	int m_current_step_number;
	int m_step_number;

	Movement_controller m_controller;
};

#endif /* HEXAPODE_H_ */
