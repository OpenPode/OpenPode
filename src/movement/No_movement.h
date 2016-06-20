/*
 * No_movement.h
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#ifndef NO_MOVEMENT_H_
#define NO_MOVEMENT_H_

#include "Movement.h"

class No_movement : public Movement
{
public:
	No_movement();
	virtual ~No_movement() {}

	virtual float determine_real_distance(Paw &paw);
	virtual void compute_variables();
	virtual float* determine_paw_position(Paw &paw);
	virtual bool is_sequence_finished(Side &side);

protected:

	void determine_x_paws_position(Paw &paw);
	void determine_y_paws_position(Paw &paw);
	void determine_z_paws_position(Paw &paw);
	void determine_z_paws_position_not_get_up(Paw &paw);

private:
	bool test_for_good_position_xy(Paw &paw);
	bool test_for_good_position_z(Paw &paw);

	bool m_z_good_position;
	bool m_xy_good_position;
};

#endif /* NO_MOVEMENT_H_ */
