/*
c * Side.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef SIDE_H_
#define SIDE_H_

#include "Paw.h"
#include "PCA9685.h"

class i2cdev;
class Movement;

struct Paws_position
{
	Coords front_paw;
	Coords middle_paw;
	Coords back_paw;
};

class Side
{
public:

	Side(Side_enum side, i2cdev *i2c);

	void memorize_movement(Movement *mvt);
	int update(int sequence_number, double a= 0, double b=-50, double paw_spreading=70); // a = 0.07
	double change_sequence_number(int sequence_number);

	Paws_position get_paws_position() const { return m_paws_position; }
	Side_enum get_side_id() const { return m_side; }
	int get_side_coef() const { return m_side_coef; }
	int get_current_sequence_number() const { return m_current_sequence_number; }

	// Debug functions
	Paw& get_front_paw() {return m_front_paw;}
	Paw& get_middle_paw() {return m_middle_paw;}
	Paw& get_back_paw() {return m_back_paw;}
	PCA9685& get_module() {return m_module;}

private:

	void move_paw(Paw &paw, double tab[3]);
	void memorize_current_paw_position();

	Side_enum m_side;
	Paws_position m_paws_position;

	Paw m_front_paw;
	Paw m_middle_paw;
	Paw m_back_paw;

	int m_side_coef;

	static const Channel_t channel_table[3][3];

	PCA9685 m_module;

	Movement *m_movement;
	int m_current_sequence_number;
};


#endif /* SIDE_H_ */
