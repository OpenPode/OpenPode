/*
c * Side.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef SIDE_H_
#define SIDE_H_

#include "paw/Paw.h"
#include "drivers/pca9685/PCA9685.h"

class i2cdev;
class Movement;

struct Paws_position
{
	Coords front_paw;
	Coords middle_paw;
	Coords back_paw;
};

class Side
{
public:

	Side(Side_enum side, i2cdev *i2c, Error_detection* p_error_detection, const int p_sequence_of_paws[3]);

	void memorize_movement(Movement *mvt);
	void prepare_update();
	int update();
	double get_real_distance();
	void memorize_current_paw_position();

	Paws_position get_paws_position() const { return m_paws_position; }
	Side_enum get_side_id() const { return m_side; }
	int get_side_coef() const { return m_side_coef; }
	bool is_paws_positions_available();
	int get_max_sequence_number();

	// Debug functions
	Paw& get_front_paw() {return m_front_paw;}
	Paw& get_middle_paw() {return m_middle_paw;}
	Paw& get_back_paw() {return m_back_paw;}
	PCA9685& get_module() {return m_module;}

private:

	void move_paw(Paw &paw);

	//TODO: Change side_enum place
	Side_enum m_side;
	//represent the xyz coordinates of each paw
	Paws_position m_paws_position;

	Paw m_front_paw;
	Paw m_middle_paw;
	Paw m_back_paw;

	//side_coef is used to correct formula depending of the side
	int m_side_coef;

	static const Channel_t channel_table[3][3];
	int servos_time_table[3][3];

	PCA9685 m_module;

	Movement *m_movement;
};


#endif /* SIDE_H_ */
