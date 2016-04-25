/*
 * Paw.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_H_
#define PAW_H_

#include "Paw_math_model.h"
#include "Servo.h"

#include "PCA9685.h"

class Paw : public Paw_math_model
{
public:

	Paw(Side_enum side, Paw_position_enum paw_position);

	void prepare_to_move(double position[3]);
	void valid_move();

	Coords get_current_position() const;

	bool test_machenical_stop_limit(int tibias_value, int femur_value, int coxa_value);
	bool test_model_limit(int tibias_angle, int femur_angle, int coxa_angle);
	bool is_position_available(int tibias_value, int femur_value, int coxa_value);

public:

	Side_enum m_side;
	Paw_position_enum m_position;
	int m_side_coef;

	Servo m_coxa;
	Servo m_femur;
	Servo m_tibia;
	int servos_time_table[3];

	Coords m_prepare_coords;
	Coords m_current_coords;
	Angles m_servo_angles;

	double m_x_center;

	void determine_servos_paw_time();
};


#endif /* PAW_H_ */
