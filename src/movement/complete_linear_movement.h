/*
 * complete_linear_movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: guillaume
 */

#ifndef COMPLETE_LINEAR_MOVEMENT_H_
#define COMPLETE_LINEAR_MOVEMENT_H_

#include "Movement.h"

class Paw;

class complete_linear_movement : public Movement
{
public:
	complete_linear_movement(int angle, double distance, int step_number);
	virtual ~complete_linear_movement() {}

	void determine_x_paws_position(Side &side, int sequence_number);
	void determine_y_paws_position(Side &side, int sequence_number, double paw_spreading);
	void determine_z_paws_position(Side &side, int sequence_number, double front_height, double back_height);

	virtual double determine_real_distance(Side &side);
	virtual void compute_variables();
	virtual Paw_position determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading);

	int m_paw_spreading;
};

#endif /* COMPLETE_LINEAR_MOVEMENT_H_ */
