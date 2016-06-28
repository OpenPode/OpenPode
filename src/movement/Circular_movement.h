/*
 * Circular_movement.h
 *
 *  Created on: 28 juin 2016
 *      Author: Sarthou
 */

#ifndef MOVEMENT_CIRCULAR_MOVEMENT_H_
#define MOVEMENT_CIRCULAR_MOVEMENT_H_

#include "Movement.h"

enum Rotation_side_enum
{
	left_rotation,
	right_rotation
};

class Paw;

class Circular_movement : public Movement
{
public:
	Circular_movement(float radius, Movement_direction direction, Rotation_side_enum side, float distance, int step_number);
	virtual ~Circular_movement() {}

	void determine_x_paws_position(Paw &paw);
	void determine_y_paws_position(Paw &paw);
	void determine_z_paws_position(Paw &paw);

	virtual float determine_real_distance(Paw &paw);
	virtual void compute_variables(Paw & paw);
	virtual float* determine_paw_position(Paw &paw);
	virtual bool is_sequence_finished(Paw &paw);

private:
	Rotation_side_enum m_rotation_side;
	float m_internal_radius;
	float m_internal_angle;
};

#endif /* MOVEMENT_CIRCULAR_MOVEMENT_H_ */
