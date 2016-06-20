/*
 * Movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_

#include "Incline_coef.h"
#include "hexapode/hexapode_dimensions.h"

constexpr int NO_MOVEMENT_STEP_DIST = 2;

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

class Movement
{
public:
	Movement(Movement_type type, Movement_direction direction, float distance, float angle, int step_number);
	virtual ~Movement() {}

	void compute_z_value_for_standard_paw(Paw &paw, Incline_coef_t p_incline_coef);
	void memorize_parameters(int sequence_number, Incline_coef_t p_incline_coef, float paw_spreading);
	void raz_current_step_number();
	void update_current_step_number(int p_current_step_number);
	void update_sequence_number(int p_sequence_number);
	void increase_current_step_number();

	virtual float determine_real_distance(Side &side) = 0;
	virtual void compute_variables() = 0;
	virtual float* determine_paw_position(Paw &paw) = 0;
	virtual bool is_sequence_finished(Side &side) = 0;

	float reproach_position(float present, float futur, float p_step_distance);
	float get_up_paw(float final_height, Paw &paw, float p_step_distance);
	float just_get_up_paw(Paw &paw, float p_step_distance);
	float just_get_down_paw(float final_height, Paw &paw, float p_step_distance);
	float goto_position(float present, float futur, float nb_step);

	void set_paw_spreading_step(int step = NO_MOVEMENT_STEP_DIST);
	void set_number_of_sequence(int p_nb_of_sequence);

	Movement_type m_type;
	Movement_direction m_direction;
	float m_distance;
	float m_corrected_distance;

	int m_sequence_number;
	int m_number_of_sequence;
	int m_step_number;
	int m_current_step_number;

	float m_step_distance_x;
	float m_step_distance_y;
	float m_step_distance_z;

	float m_angle;
	int m_paw_spreading;
	int m_paw_spreading_step;
	Incline_coef_t m_incline_coef;

	float m_paw_position[3];

	int find_solution;
};

#endif /* MOVEMENT_H_ */
