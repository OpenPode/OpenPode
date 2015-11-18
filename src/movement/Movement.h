/*
 * Movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

class Side;
class Paw;

enum Movement_type
{
	linear,
	rotation_left,
	rotation_right,
	no_movement
};

enum Movement_direction
{
	direction_front = 1,
	direction_back  = -1
};

struct Paw_position
{
	double front[3];
	double middle[3];
	double back[3];
};

class Movement
{
public:
	Movement(Movement_type type, Movement_direction direction, double distance, double angle, int step_number);
	virtual ~Movement() {}

	double reproach_position(double present, double futur, double p_step_distance);
	double get_up_paw(double final_height, Paw &paw, double p_step_distance);
	double goto_position(double present, double futur, double nb_step);

	virtual double determine_real_distance(Side &side) = 0;

	virtual Paw_position determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading) = 0;

	Movement_type m_type;
	Movement_direction m_direction;
	double m_distance;
	double m_angle;
	int m_step_number;
	double m_step_distance_x;
	double m_step_distance_z;

	double m_corrected_distance;

	int m_current_step_number;

	Paw_position m_paw_position;
};

#endif /* MOVEMENT_H_ */
