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

class Hexapode
{
public:

	Hexapode();
	~Hexapode();

	void move(Movement mvt);

	int update();

public:

	i2cdev *m_i2c;

	Side m_left_side;
	Side m_right_side;

	int m_current_sequence_number;
};

#endif /* HEXAPODE_H_ */
