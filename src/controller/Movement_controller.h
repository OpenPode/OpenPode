/*
 * Movement_controller.h
 *
 *  Created on: 18 avr. 2016
 *      Author: Sarthou
 */

#ifndef CONTROLLER_MOVEMENT_CONTROLLER_H_
#define CONTROLLER_MOVEMENT_CONTROLLER_H_

#include "controller/PS4Controller.h"
#include "movement/Incline_coef.h"
#include "movement/Movement.h"

class Movement;

class Movement_controller_delegate
{

public :

  virtual ~Movement_controller_delegate(){}

  virtual void reinit(float pitch_stick, float roll_stick, float height, float paw_spreading) = 0;

};

class Movement_controller
{
public:

	Movement_controller();
	~Movement_controller();

	void get_new_movement(int current_step_number, int step_number);
	bool is_a_new_movement();
	void run_controller();

	float get_paw_spreading() const { return m_paw_spreading; }
	Incline_coef_t get_incline_coef() const { return m_incline_coef; }
	float get_center_height() const { return m_center_height; }
	float get_pitch_stick() const { return m_incline_pitch_lin_value; }
	float get_roll_stick() const { return m_incline_roll_lin_value; }
	int get_led();

	Movement* get_movement() { return m_movement; }

	void set_new_paw_spreading(float p_paw_spreading);
	void set_new_center_height(float p_center_height);
	void set_new_incline(float p_pitch_stick, float p_roll_stick);
	void go_back_to_default_position();

	//delegate
	void set_delegate(Movement_controller_delegate* p_delegate);

private:

	Movement *m_movement;
	Movement_controller_delegate* m_delegate;

	float m_paw_spreading;
	float m_center_height;

	int m_current_step_number;
	int m_step_number;

	bool new_movement;

	float make_more_linear(int stick_value); //out between [-1 ; 1]
	void make_sticks_more_linear();

private:

	int m_movement_x_value;
	int m_movement_y_value;
	int m_incline_pitch_value;
	int m_incline_roll_value;

	float m_movement_x_lin_value;
	float m_movement_y_lin_value;
	float m_incline_pitch_lin_value;
	float m_incline_roll_lin_value;

	bool m_up_pressed;
	bool m_down_pressed;
	bool m_move_apart_pressed;
	bool m_tighten_pressed;

	bool m_turn_back_default_pressed;
	bool m_turn_back_default_last_state;
	Incline_coef_t m_incline_coef;

	bool m_square_pressed;
	bool m_circle_pressed;

	bool m_change_movement_pressed;
	bool m_change_movement_was_pressed;
	Movement_type m_movement_type;

	void get_control_values();

	void update_movement();
	void update_paw_spreading();
	void update_center_height();
	void update_incline();
	void verify_movement_type_changement();

	PS4Controller m_PS4_controller;
};

#endif /* CONTROLLER_MOVEMENT_CONTROLLER_H_ */
