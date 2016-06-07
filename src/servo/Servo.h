/*
 * Servo.h
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#ifndef SERVO_H_
#define SERVO_H_

enum Side_enum
{
	side_left,
	side_right
};

enum Paw_position_enum
{
	position_front,
	position_middle,
	position_back
};

enum Servo_position_enum
{
	position_tibia,
	position_femur,
	position_coxa
};

enum Coord_enum
{
	coord_x,
	coord_y,
	coord_z
};

class Servo
{
public:

	Servo(Side_enum side, Paw_position_enum paw_position, Servo_position_enum servo_position);

	static const int min_ratio = 205;
	static const int max_ratio = 441;
	static constexpr float excursion_deg = 120.0f;
	static constexpr float resolution = float(max_ratio-min_ratio)/excursion_deg;

	int get_offset() const;
	bool is_value_in_the_range(int value);

public:

	Side_enum m_side;
	Paw_position_enum m_paw_position;
	Servo_position_enum m_position;

	int m_offset;
};



#endif /* SERVO_H_ */
