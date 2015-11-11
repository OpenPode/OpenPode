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

class Hexapode
{
public:

	Hexapode();
	~Hexapode();

	void run();
	void calibrate_servomotors(double x, double y, double z);

	void move(Movement mvt);

	int update();

	void toggle();

public:

	i2cdev *m_i2c;

	util::timer_t m_timer;

	Side m_left_side;
	Side m_right_side;

	Movement m_mvt;
	int m_current_sequence_number;
};

#endif /* HEXAPODE_H_ */
