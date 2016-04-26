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
	double x_stick;
	double y_stick;
};

struct parameters_t
{
	sticks_values_t incline_values;
	double paw_spreading;
	double center_height;
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

	bool finished_to_corrected_error;

	void reinit(double x_stick_incline, double y_stick_incline, double height, double paw_spreading);

};

#endif /* ERROR_MANAGEMENT_ERROR_ACTIONS_H_ */
