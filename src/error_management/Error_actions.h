/*
 * Error_actions.h
 *
 *  Created on: 26 avr. 2016
 *      Author: Sarthou
 */

#ifndef ERROR_MANAGEMENT_ERROR_ACTIONS_H_
#define ERROR_MANAGEMENT_ERROR_ACTIONS_H_

#include "Movement.h"
#include "Movement_controller.h"
#include "config.h"

const double DEFAULT_STEP = 10;

struct sticks_values_t
{
	double pitch;
	double roll;
	bool operator==(const sticks_values_t &c)
	{
		return ((int)(c.pitch * 1000) == (int)(pitch * 1000))
			&& ((int)(c.roll * 1000) == (int)(roll * 1000));
	}
};

struct parameters_t
{
	sticks_values_t incline_values;
	double paw_spreading;
	double center_height;
	bool operator==(const parameters_t &c)
	{
		return (incline_values == c.incline_values) &&
	           ((int)(c.paw_spreading * 1000) == (int)(paw_spreading * 1000)) &&
			   ((int)(c.center_height * 1000) == (int)(center_height * 1000));
	}
};



enum step_of_solution
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
	void purpose_new_parameters(double x_stick_incline, double y_stick_incline, double height, double paw_spreading);

	void resolve_error(Movement_type p_movement_type, bool on_error);
	void set_parameters_on_movement_controller();

	bool have_finished_to_corrected_error() const { return m_finished_to_corrected_error; }
	bool is_resolving() const { return m_resolving; }

protected:

	Movement_controller* m_movement_controller;
	parameters_t m_precedent_parameters;
	parameters_t m_purpose_parameters;
	parameters_t m_new_parameters;
	parameters_t m_saved_parameters;

	bool m_finished_to_corrected_error;
	bool m_resolving;

	step_of_solution m_current_step;

	bool m_on_error;
	int find_solution;

	double paw_speading_direction;
	double height_direction;

	//no_movement
	void action_no_movement_no_changement();
	void action_no_movement_reduce_incline();
	void action_no_movement_find_paw_spreadind_direction();
	void action_no_movement_find_height_direction();
	void action_no_movement_get_closer_stable_parameters();
	void action_no_movement_changement();
	void action_no_movement_get_closer_stop_sequence();

	void reinit(double pitch_stick, double roll_stick, double height, double paw_spreading);
	void set_end_of_solving();

	//solving algo
	double dichotomie(bool condition, double last_new, double purpose);
	void find_direction(int &cpt, double &direction, double &new_parameters, int step = DEFAULT_STEP);
};

#endif /* ERROR_MANAGEMENT_ERROR_ACTIONS_H_ */
