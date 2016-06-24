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

#include "drivers/pca9685/PCA9685.h"

struct Position_on_hexapode
{
	float x_offset;
	float y_offset;
};

struct Paw_coords
{
	Coords m_prepare_coords;
	Coords m_current_coords;
	Coords m_last_coords;
};

/*struct Servos_of_paw
{
	Servo m_coxa;
	Servo m_femur;
	Servo m_tibia;
	int servos_time_table[3];
};*/

class Error_detection;

class Paw : public Paw_math_model
{
public:

	Paw(Side_enum side, Paw_position_enum paw_position, Error_detection* p_error_detection, float p_x_offset, float p_y_offset, int p_active_sequence_number);

	void prepare_to_move(float position[3]);
	void valid_move();

	Coords get_current_position() const;
	Coords get_last_position() const;
	int get_active_sequence_number() const {return m_active_sequence_number; }
	int get_side_coef() const { return m_side_coef; }

public:

	Side_enum m_side;
	Paw_position_enum m_position;
	int m_side_coef;

	//Servos_of_paw m_servos;
	Servo m_coxa;
	Servo m_femur;
	Servo m_tibia;
	int servos_time_table[3];

	Paw_coords coords;
	Angles m_servo_angles;

	float m_x_center;
	Position_on_hexapode m_position_on_hexapode;
	int m_active_sequence_number;

	Error_detection* m_error_detection;

	void determine_servos_paw_time();
};


#endif /* PAW_H_ */
