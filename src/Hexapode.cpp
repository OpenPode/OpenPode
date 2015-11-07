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

void Hexapode::toggle()
{
	if(m_current_sequence_number)
		m_current_sequence_number = 0;
	else
		m_current_sequence_number = 1;

	m_left_side.change_sequence_number(m_current_sequence_number);
	m_right_side.change_sequence_number(m_current_sequence_number);
}

void Hexapode::move(Movement mvt)
{
	/* TODO :
	 * 	compute the distance for each side for circular mvt
	 */
	//toggle();
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

