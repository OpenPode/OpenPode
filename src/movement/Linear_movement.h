/*
 * Linear_movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef LINEAR_MOVEMENT_H_
#define LINEAR_MOVEMENT_H_

#include "Movement.h"

class Paw;

class Linear_movement : public Movement
{
public:
	Linear_movement(Movement_direction direction, double distance, int step_number);
	virtual ~Linear_movement() {}

	void determine_x_paws_position(Side &side, int sequence_number);
	void determine_y_paws_position(Side &side, int sequence_number, double paw_spreading);
	void determine_z_paws_position(Side &side, int sequence_number, double front_height, double back_height);

	virtual double determine_real_distance(Side &side);
	virtual void compute_variables();
	virtual Paw_position determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading);
	bool is_sequence_finished(Side &side, int sequence_number);
};

#endif /* LINEAR_MOVEMENT_H_ */
