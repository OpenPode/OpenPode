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
#include "utility/Vector.h"
#include "drivers/pca9685/PCA9685.h"

/*struct Position_on_hexapode // deprecated, use Vector2f instead
{
	float x_offset;
	float y_offset;
};*/

struct Paw_coords
{
	Vector3f m_prepare_coords;
	Vector3f m_current_coords;
	Vector3f m_last_coords;
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

	Paw_position_enum get_position() const { return m_position; }
	Side_enum get_side() const { return m_side; }

	Vector2f get_position_offset() const { return m_position_offset; }
	Vector3f get_current_coords() const { return coords.m_current_coords; }
	Vector3f get_last_position() const { return coords.m_last_coords; }

	float get_x_center() const { return m_x_center; }

	int get_servo_time(Servo_position_enum servo_position) const { return servos_time_table[servo_position]; }
	int get_active_sequence_number() const {return m_active_sequence_number; }
	int get_side_coef() const { return m_side_coef; }

	Angles get_servo_angle() const { return m_servo_angles; }

	const Servo& get_tibia() const { return m_tibia; }
	const Servo& get_femur() const { return m_femur; }
	const Servo& get_coxa () const { return m_coxa;  }

	void calibrate(int time[3]);

private:

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
	Vector2f m_position_offset;
	int m_active_sequence_number;

	Error_detection* m_error_detection;

	void determine_servos_paw_time();
};


#endif /* PAW_H_ */
