/*
 * No_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "movement/No_movement.h"
#include "side/Side.h"

No_movement::No_movement() : Movement(no_movement, direction_front, 0.f, 0.f, 30), m_z_good_position(0), m_xy_good_position(0)
{

}

void No_movement::determine_x_paws_position(Paw &paw)
{
	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if(paw.get_active_sequence_number() == m_sequence_number)
		m_paw_position[coord_x] = reproach_position(paw.get_current_coords().x, paw.get_x_center(), NO_MOVEMENT_STEP_DIST);
	else
		m_paw_position[coord_x]  = paw.get_last_position().x;
}

void No_movement::determine_y_paws_position(Paw &paw)
{
	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if((paw.get_active_sequence_number() == m_sequence_number) || in_correction)
		m_paw_position[coord_y]  = reproach_position(paw.get_current_coords().y, paw.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
	else
		m_paw_position[coord_y] = paw.get_last_position().y;
}

void No_movement::determine_z_paws_position(Paw &paw)
{
	compute_z_value_for_standard_paw(paw, m_incline_coef);

	if(paw.get_active_sequence_number() == m_sequence_number)
		m_paw_position[coord_z]  = just_get_up_paw(paw, NO_MOVEMENT_STEP_DIST, m_paw_position[coord_z]);
}

void No_movement::determine_z_paws_position_not_get_up(Paw &paw)
{
	compute_z_value_for_standard_paw(paw, m_incline_coef);
	//use standard height paw as final height
	if(paw.get_active_sequence_number() == m_sequence_number)
		m_paw_position[coord_z] = just_get_down_paw(m_paw_position[coord_z], paw, NO_MOVEMENT_STEP_DIST);
}

bool No_movement::test_for_good_position_z(Paw &paw)
{
	//test if one or more paw is not at the correct x,y or z position
	float z_theoretic_value = m_incline_coef.A*(paw.get_x_center() + paw.get_position_offset().x) +
							   m_incline_coef.B*(paw.get_side_coef()*(m_paw_spreading + paw.get_position_offset().y)) +
							   m_incline_coef.C;

	return(z_theoretic_value == (paw.get_current_coords().z));
}

bool No_movement::test_for_good_position_xy(Paw &paw)
{
	//test if one or more paw is not at the correct x,y or z position
	if(paw.get_active_sequence_number() == m_sequence_number)
	{
		if( (paw.get_current_coords().y  != paw.get_side_coef() * m_paw_spreading) or
			(paw.get_current_coords().x != paw.get_x_center()))
			return 0;
	}
	return 1;
}

float No_movement::determine_real_distance(Paw &paw)
{
	return 0;
}

void No_movement::compute_variables(Paw & paw)
{
}

float* No_movement::determine_paw_position(Paw &paw)
{
	compute_variables(paw);
	m_xy_good_position = test_for_good_position_xy(paw);
	m_z_good_position = test_for_good_position_z(paw);
	//get up paws only if one or more paw is not at the correct x,y or z position
	determine_x_paws_position(paw);
	determine_y_paws_position(paw);
	if(!m_xy_good_position)
		determine_z_paws_position(paw);
	else
		determine_z_paws_position_not_get_up(paw);

	return m_paw_position;
}

//permit to create a more realistic movement
bool No_movement::is_sequence_finished(Paw &paw)
{
	if(m_current_step_number >= 15)//minimal step number
	{
		m_xy_good_position = test_for_good_position_xy(paw);
		m_z_good_position = test_for_good_position_z(paw);
		if(m_xy_good_position && m_z_good_position)
			return SEQUENCE_FINISH;
	}
	return SEQUENCE_IN_PROGRESS; //movement doesn't finished
}


