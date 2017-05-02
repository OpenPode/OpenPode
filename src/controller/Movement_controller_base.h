/*
 * Movement_controller_base.h
 *
 *  Created on: 2 mai 2017
 *      Author: guill
 */

#ifndef SRC_CONTROLLER_MOVEMENT_CONTROLLER_BASE_H_
#define SRC_CONTROLLER_MOVEMENT_CONTROLLER_BASE_H_

#include "movement/Incline_coef.h"
#include "movement/Movement.h"

class Movement;

class Movement_controller_delegate
{

public :

  virtual ~Movement_controller_delegate(){}

  virtual void reinit(float pitch_stick, float roll_stick, float height, float paw_spreading) = 0;

};

class Movement_controller_base
{
public:

	Movement_controller_base();
	virtual ~Movement_controller_base();

	void get_new_movement(int current_step_number, int step_number);
	virtual void custom_get_new_movement() {};
	virtual void run_controller() = 0;

	float get_paw_spreading() const { return m_paw_spreading; }
	Incline_coef_t get_incline_coef() const { return m_incline_coef; }
	float get_center_height() const { return m_center_height; }
	float get_pitch_stick() const { return m_incline_pitch_lin_value; }
	float get_roll_stick() const { return m_incline_roll_lin_value; }
	Movement* get_movement() { return m_movement; }
	bool is_a_new_movement();

	void set_new_paw_spreading(float p_paw_spreading);
	void set_new_center_height(float p_center_height);
	void set_new_incline(float p_pitch_stick, float p_roll_stick);
	void go_back_to_default_position();

	//delegate
	void set_delegate(Movement_controller_delegate* p_delegate);

protected:

	Movement *m_movement;
	Movement_controller_delegate* m_delegate;

	int m_current_step_number;
	int m_step_number;

	bool new_movement;

	float m_paw_spreading;
	float m_center_height;
	float m_movement_x_lin_value;
	float m_movement_y_lin_value;
	float m_incline_pitch_lin_value;
	float m_incline_roll_lin_value;
	Incline_coef_t m_incline_coef;

	Movement_type m_movement_type;

	virtual void get_control_values() = 0;

	void update_movement();
	virtual void update_paw_spreading() {};
	virtual void update_center_height() {};
	virtual void update_incline();
};

#endif /* SRC_CONTROLLER_MOVEMENT_CONTROLLER_BASE_H_ */
