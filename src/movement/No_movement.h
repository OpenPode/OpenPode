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
	virtual Paw_position determine_paws_position(Side &side, int sequence_number, Incline_coef_t p_incline_coef, double paw_spreading);
	virtual bool is_sequence_finished(Side &side);

protected:

	void determine_x_paws_position(Side &side);
	void determine_y_paws_position(Side &side);
	void determine_z_paws_position(Side &side);
	void determine_z_paws_position_not_get_up(Side &side);

private:
	bool test_for_get_up_sequence0(Side &side);
	bool test_for_get_up_sequence1(Side &side);
	bool test_for_get_up_sequence2(Side &side);
	bool test_for_good_position_xy(Side &side);
	bool test_for_good_position_z(Side &side);

	bool m_z_good_position;
	bool m_xy_good_position;
};

#endif /* NO_MOVEMENT_H_ */
