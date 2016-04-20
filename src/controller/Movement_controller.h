/*
 * Movement_controller.h
 *
 *  Created on: 18 avr. 2016
 *      Author: Sarthou
 */

#ifndef CONTROLLER_MOVEMENT_CONTROLLER_H_
#define CONTROLLER_MOVEMENT_CONTROLLER_H_

#include "PS4Controller.h"

class Movement;

class Movement_controller
{
public:

	Movement_controller();
	~Movement_controller();

	bool get_new_movement(int current_step_number, int step_number);
	void run_controller();

public:

	Movement *m_movement;

	double m_paw_spreading;
	double m_center_height;

	int m_current_step_number;
	int m_step_number;

	float m_A_coef_incline;
	float m_B_coef_incline;

	double get_paw_spreading() const { return m_paw_spreading; }
	double get_center_height() const { return m_center_height; }
	double get_A_coef_incline() const { return m_A_coef_incline; }
	double get_B_coef_incline() const { return m_B_coef_incline; }

	double make_more_linear(int stick_value); //out between [-1 ; 1]
	void make_sticks_more_linear();

private:

	int m_movement_x_value;
	int m_movement_y_value;
	int m_incline_value;
	double m_movement_x_lin_value;
	double m_movement_y_lin_value;
	double m_incline_lin_value;
	bool m_up_pressed;
	bool m_down_pressed;
	bool m_move_apart_pressed;
	bool m_tighten_pressed;

	void get_control_values();

	PS4Controller m_PS4_controller;
};

#endif /* CONTROLLER_MOVEMENT_CONTROLLER_H_ */
