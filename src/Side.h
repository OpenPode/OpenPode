/*
 * Side.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef SIDE_H_
#define SIDE_H_

#include "Paw.h"
#include "PCA9685.h"
#include "enums.h"

class i2cdev;

enum Movement_type
{
	linear,
	rotation_left,
	rotation_right,
	no_movement
};

enum Movement_direction
{
	direction_front,
	direction_back
};

struct Movement
{
	Movement_type type;
	Movement_direction direction;
	double distance;
	double angle;
	int step_number;
};

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

	void memorize_movement(Movement mvt);

	int update(int sequence_number, double a=0, double b=-50, double paw_spreading=70);

	double change_sequence_number(int sequence_number);

public:

	void move_paw(Paw &paw, double tab[3]);
	void memorize_current_paw_position();
	double determine_real_distance();
	void determine_paws_position(int sequence_number, double front_height, double back_height, double paw_spreading);
	double reproach_position(double present, double futur);
	void determine_y_paws_position(int sequence_number, double paw_spreading);
	double get_up_paw(double final_height, Paw &paw);
	void determine_z_paws_position(int sequence_number, double a, double b);

	Side_enum m_side;
	Paws_position m_paws_position;

	Paw m_front_paw;
	Paw m_middle_paw;
	Paw m_back_paw;

	int m_side_coef;

	double m_paw_position[3][3];

	static const Channel_t channel_table[3][3];

	PCA9685 m_module;

	Movement m_movement;
	double m_step_distance;
	int m_current_step_number;
	int m_current_sequence_number;

};


#endif /* SIDE_H_ */
