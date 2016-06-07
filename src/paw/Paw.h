/*
 * Paw.h
 *
 *  Created on: 28 sept. 2015
 *      Author: Julien
 */

#ifndef PAW_H_
#define PAW_H_

#include "paw/Paw_math_model.h"
#include "servo/Servo.h"

#include "PCAmodule/PCA9685.h"

class Error_detection;

class Paw : public Paw_math_model
{
public:

	Paw(Side_enum side, Paw_position_enum paw_position, Error_detection* p_error_detection);

	void prepare_to_move(float position[3]);
	void valid_move();

	Coords get_current_position() const;

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

	float m_x_center;

	Error_detection* m_error_detection;

	void determine_servos_paw_time();
};


#endif /* PAW_H_ */
