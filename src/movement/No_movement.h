/*
 * No_movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef NO_MOVEMENT_H_
#define NO_MOVEMENT_H_

#include "Movement.h"

#define NO_MOVEMENT_STEP_DIST 2

class No_movement : public Movement
{
public:
	No_movement();
	virtual ~No_movement() {}

	virtual double determine_real_distance(Side &side);
	virtual void compute_variables();
	virtual Paw_position determine_paws_position(Side &side, int sequence_number, double front_height, double back_height, double paw_spreading);

protected:

	void determine_x_paws_position(Side &side, int sequence_number);
	void determine_y_paws_position(Side &side, int sequence_number, double paw_spreading);
	void determine_z_paws_position(Side &side, int sequence_number, double a, double b);
	void determine_z_paws_position_not_get_up(double a, double b);

private:
	bool test_for_get_up(Side &side, double paw_spreading);
};

#endif /* NO_MOVEMENT_H_ */
