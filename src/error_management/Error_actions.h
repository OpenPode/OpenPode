/*
 * Error_actions.h
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#ifndef ERROR_MANAGEMENT_ERROR_ACTIONS_H_
#define ERROR_MANAGEMENT_ERROR_ACTIONS_H_

#include "movement/Movement.h"
#include "controller/Movement_controller.h"
#include "config.h"

constexpr float DEFAULT_STEP = 10.f;

struct sticks_values_t
{
	float pitch;
	float roll;
	bool operator==(const sticks_values_t &c)
	{
		return ((int)(c.pitch * 1000) == (int)(pitch * 1000))
			&& ((int)(c.roll * 1000) == (int)(roll * 1000));
	}
};

struct Dichotomie_t
{
	float A;
	float B;
	float C;
};

struct Parameters_t
{
	sticks_values_t incline_values;
	float paw_spreading;
	float center_height;
	bool operator==(const Parameters_t &c)
	{
		return (incline_values == c.incline_values) &&
	           ((int)(c.paw_spreading * 1000) == (int)(paw_spreading * 1000)) &&
			   ((int)(c.center_height * 1000) == (int)(center_height * 1000));
	}
};

enum Resolution_step
{
	wait,
	stop_sequence,
	cancel_incline,
	reduce_incline,
	find_paw_spreading_stable_direction,
	find_height_stable_direction,
	get_closer_stable_parameters,
	save_parameters
};

class Error_actions : public Movement_controller_delegate
{
public :

	Error_actions(Movement_controller* p_movement_controller);
	~Error_actions();

	void valid_parameters();
	void valid_parameters_no_error();
	void purpose_new_parameters(float x_stick_incline, float y_stick_incline, float height, float paw_spreading);

	void resolve_error(Movement_type p_movement_type, bool on_error);
	void set_parameters_on_movement_controller();

	bool have_finished_to_corrected_error() const { return m_finished_to_corrected_error; }
	bool is_resolving() const { return m_resolving; }

protected:

	Movement_controller* m_movement_controller;
	Parameters_t m_precedent_parameters;
	Parameters_t m_purpose_parameters;
	Parameters_t m_new_parameters;
	Parameters_t m_saved_parameters;

	bool m_finished_to_corrected_error;
	bool m_resolving;

	Resolution_step m_current_step;

	bool m_on_error;
	int find_solution;

	float paw_speading_direction;
	float height_direction;
	Dichotomie_t dichotomie_pitch;
	Dichotomie_t dichotomie_roll;

	//no_movement
	void action_no_movement_no_changement();
	void action_no_movement_reduce_incline();
	void action_no_movement_find_paw_spreadind_direction();
	void action_no_movement_find_height_direction();
	void action_no_movement_get_closer_stable_parameters();
	void action_no_movement_changement();
	void action_no_movement_get_closer_stop_sequence();

	void reinit(float pitch_stick, float roll_stick, float height, float paw_spreading);
	void set_end_of_solving();

	//solving algo
	float dichotomie(bool condition, Dichotomie_t dicho);
	void find_direction(int &cpt, float &direction, float &new_parameters, int step = DEFAULT_STEP);

	void no_action();
};

#endif /* ERROR_MANAGEMENT_ERROR_ACTIONS_H_ */
