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
	Linear_movement(Movement_direction direction, float distance, int step_number);
	virtual ~Linear_movement() {}

	void determine_x_paws_position(Paw &paw);
	void determine_y_paws_position(Paw &paw);
	void determine_z_paws_position(Paw &paw);

	virtual float determine_real_distance(Paw &paw);
	virtual void compute_variables();
	virtual float* determine_paw_position(Paw &paw);
	bool is_sequence_finished(Paw &paw);
};

#endif /* LINEAR_MOVEMENT_H_ */
