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

	virtual double reproach_position(double present, double futur);
	virtual double get_up_paw(double final_height, Paw &paw);
	virtual double goto_position(double present, double futur, double nb_step);

	virtual void determine_x_paws_position(Side &side, int sequence_number);
	virtual void determine_y_paws_position(Side &side, int sequence_number, double paw_spreading);
	virtual void determine_z_paws_position(Side &side, int sequence_number, double front_height, double back_height);

	virtual double determine_real_distance(Side &side);

	virtual Paw_position determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading);
};

#endif /* LINEAR_MOVEMENT_H_ */
