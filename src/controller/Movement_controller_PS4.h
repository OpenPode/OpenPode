/*
 * Movement_controller_PS4.h
 *
 *  Created on: 2 mai 2017
 *      Author: guill
 */

#ifndef SRC_CONTROLLER_MOVEMENT_CONTROLLER_PS4_H_
#define SRC_CONTROLLER_MOVEMENT_CONTROLLER_PS4_H_

#include "controller/PS4Controller.h"
#include "movement/Incline_coef.h"
#include "movement/Movement.h"
#include "controller/Movement_controller_base.h"

class Movement;

class Movement_controller_PS4 : public Movement_controller_base
{
public:

	Movement_controller_PS4();
	~Movement_controller_PS4();

	void get_new_movement(int current_step_number, int step_number);
	void run_controller();

	int get_led();

private:
	float make_more_linear(int stick_value); //out between [-1 ; 1]
	void make_sticks_more_linear();

	int m_movement_x_value;
	int m_movement_y_value;
	int m_incline_pitch_value;
	int m_incline_roll_value;

	bool m_up_pressed;
	bool m_down_pressed;
	bool m_move_apart_pressed;
	bool m_tighten_pressed;

	bool m_turn_back_default_pressed;
	bool m_turn_back_default_last_state;

	bool m_square_pressed;
	bool m_circle_pressed;

	bool m_change_movement_pressed;
	bool m_change_movement_was_pressed;

	void get_control_values();

	void update_movement();
	void update_paw_spreading();
	void update_center_height();
	void verify_movement_type_changement();

	PS4Controller m_PS4_controller;
};



#endif /* SRC_CONTROLLER_MOVEMENT_CONTROLLER_PS4_H_ */
