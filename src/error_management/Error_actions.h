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
	stop_sequence,
	cancel_incline,
	reduce_incline,
	find_stable_parameters_direction,
	find_stable_parameters,
	test_one_shot,
	save_parameters
};

class Error_actions : public Movement_controller_delegate
{
public :

	Error_actions(Movement_controller* p_movement_controller);
	~Error_actions();

	void valid_parameters();
	void purpose_new_parameters(double x_stick_incline, double y_stick_incline, double height, double paw_spreading);

	void resolve_error(Movement_type p_movement_type);

	bool have_finished_to_corrected_error() const { return finished_to_corrected_error; }

protected:

	Movement_controller* m_movement_controller;
	parameters_t m_precedent_parameters;
	parameters_t m_purpose_parameters;
	parameters_t m_new_parameters;
	parameters_t m_saved_parameters;

	bool finished_to_corrected_error;

	void action_no_movement_no_changement();
	void action_no_movement_changement();

	void reinit(double pitch_stick, double roll_stick, double height, double paw_spreading);

};

#endif /* ERROR_MANAGEMENT_ERROR_ACTIONS_H_ */
