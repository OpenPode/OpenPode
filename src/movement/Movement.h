/*
 * Movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Incline_coef.h"
#include "hexapode_dimensions.h"

class Side;
class Paw;

enum Movement_type
{
	linear,
	complete_linear,
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

	void compute_z_value_for_standard_paw(Side &side, Incline_coef_t p_incline_coef);
	void memorize_parameters(int sequence_number, Incline_coef_t p_incline_coef, double paw_spreading);
	void raz_current_step_number();
	void update_current_step_number(int p_current_step_number);
	void update_sequence_number(int p_sequence_number);
	void increase_current_step_number();

	virtual double determine_real_distance(Side &side) = 0;
	virtual void compute_variables() = 0;
	virtual Paw_position determine_paws_position(Side &side) = 0;
	virtual bool is_sequence_finished(Side &side) = 0;

	double reproach_position(double present, double futur, double p_step_distance);
	double get_up_paw(double final_height, Paw &paw, double p_step_distance);
	double just_get_up_paw(Paw &paw, double p_step_distance);
	double just_get_down_paw(double final_height, Paw &paw, double p_step_distance);
	double goto_position(double present, double futur, double nb_step);

	Movement_type m_type;
	Movement_direction m_direction;
	double m_distance;
	double m_corrected_distance;

	int m_sequence_number;
	int m_step_number;
	int m_current_step_number;

	double m_step_distance_x;
	double m_step_distance_y;
	double m_step_distance_z;

	double m_angle;
	int m_paw_spreading;
	Incline_coef_t m_incline_coef;

	Paw_position m_paw_position;
};

#endif /* MOVEMENT_H_ */
