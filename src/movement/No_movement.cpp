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

void No_movement::determine_x_paws_position(Side &side)
{
	m_paw_position.front[coord_x]  = side.get_paws_position().front_paw.x;
	m_paw_position.middle[coord_x] = side.get_paws_position().middle_paw.x;
	m_paw_position.back[coord_x]   = side.get_paws_position().back_paw.x;

	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if(side.get_side_id() == side_left)
	{
		if(m_sequence_number == 0)
			m_paw_position.front[coord_x]  = reproach_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(m_sequence_number == 1)
			m_paw_position.middle[coord_x] = reproach_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(m_sequence_number == 2)
			m_paw_position.back[coord_x]   = reproach_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
	}
	else
	{
		if(m_sequence_number == 0)
			m_paw_position.back[coord_x]   = reproach_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(m_sequence_number == 1)
			m_paw_position.middle[coord_x] = reproach_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(m_sequence_number == 2)
			m_paw_position.front[coord_x]  = reproach_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_y_paws_position(Side &side)
{
	m_paw_position.front[coord_y] = side.get_front_paw().m_current_coords.y;
	m_paw_position.middle[coord_y] = side.get_middle_paw().m_current_coords.y;
	m_paw_position.back[coord_y] = side.get_back_paw().m_current_coords.y;

	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_y]  = reproach_position(side.get_front_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
		else
			m_paw_position.back[coord_y]   = reproach_position(side.get_back_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
	}
	else if(m_sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
		else
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = reproach_position(side.get_back_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
		else
			m_paw_position.front[coord_y]  = reproach_position(side.get_front_paw().m_current_coords.y, side.get_side_coef() * m_paw_spreading, m_paw_spreading_step);
	}
}

void No_movement::determine_z_paws_position(Side &side)
{
	compute_z_value_for_standard_paw(side, m_incline_coef);

	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = just_get_up_paw(side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_z]   = just_get_up_paw(side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(m_sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_z] = just_get_up_paw(side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_z] = just_get_up_paw(side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = just_get_up_paw(side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_z]  = just_get_up_paw(side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_z_paws_position_not_get_up(Side &side)
{
	compute_z_value_for_standard_paw(side, m_incline_coef);

	//use standard height paw as final height
	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)

			m_paw_position.front[coord_z] = just_get_down_paw(m_paw_position.front[coord_z], side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_z] = just_get_down_paw(m_paw_position.back[coord_z], side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(m_sequence_number == 1)
	{
		m_paw_position.middle[coord_z] = just_get_down_paw(m_paw_position.middle[coord_z], side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z] = just_get_down_paw(m_paw_position.back[coord_z], side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_z] = just_get_down_paw(m_paw_position.front[coord_z],side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
	}
}

bool No_movement::test_for_good_position_z(Side &side)
{
	//test if one or more paw is not at the correct x,y or z position

	double z_theoretic_front_value = m_incline_coef.A*(side.get_front_paw().m_x_center + HALF_LENGTH) +
									 m_incline_coef.B*(side.get_side_coef()*(m_paw_spreading + HALF_WIDTH_MIN)) +
									 m_incline_coef.C;
	double z_theoretic_middle_value =m_incline_coef.A*(side.get_middle_paw().m_x_center) +
									 m_incline_coef.B*(side.get_side_coef()*(m_paw_spreading + HALF_WIDTH_MAX)) +
									 m_incline_coef.C;
	double z_theoretic_back_value =  m_incline_coef.A*(side.get_back_paw().m_x_center - HALF_LENGTH) +
									 m_incline_coef.B*(side.get_side_coef()*m_paw_spreading + HALF_WIDTH_MIN) +
									 m_incline_coef.C;

	if(m_sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			return(z_theoretic_front_value == (side.get_front_paw().m_current_coords.z));
		else
			return(z_theoretic_back_value == (side.get_back_paw().m_current_coords.z));
	}
	else if(m_sequence_number == 1)
	{
		return(z_theoretic_middle_value == (side.get_middle_paw().m_current_coords.z));
	}
	else if(m_sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			return(z_theoretic_back_value == (side.get_back_paw().m_current_coords.z));
		else
			return(z_theoretic_front_value == (side.get_front_paw().m_current_coords.z));
	}
	return 0;
}

bool No_movement::test_for_good_position_xy(Side &side)
{
	//test if one or more paw is not at the correct x,y or z position
	if(m_sequence_number == 0)
		return test_for_get_up_sequence0(side);
	else if(m_sequence_number == 1)
		return test_for_get_up_sequence1(side);
	else if(m_sequence_number == 2)
		return test_for_get_up_sequence2(side);
	else
		return 0;
}

bool No_movement::test_for_get_up_sequence0(Side &side)
{
	//test if one or more paw is not at the correct x,y or z position
	if(side.get_side_id() == side_left)
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * m_paw_spreading) or
		    (side.get_front_paw().m_current_coords.x != side.get_front_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * m_paw_spreading) or
			(side.get_back_paw().m_current_coords.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
}

bool No_movement::test_for_get_up_sequence1(Side &side)
{
	//test if one or more paw is not at the correct x,y or z position
	if( (side.get_middle_paw().m_current_coords.y  != side.get_side_coef() * m_paw_spreading) or
		(side.get_middle_paw().m_current_coords.x != side.get_middle_paw().m_x_center))
		return 0;
	else
		return 1;
}

bool No_movement::test_for_get_up_sequence2(Side &side)
{
	//test if one or more paw is not at the correct x,y or z position
	if(side.get_side_id() == side_left)
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * m_paw_spreading) or
			(side.get_back_paw().m_current_coords.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * m_paw_spreading) or
			(side.get_front_paw().m_current_coords.x != side.get_front_paw().m_x_center))
			return 0;
		else
			return 1;
}

float No_movement::determine_real_distance(Side &side)
{
	return 0;
}

void No_movement::compute_variables()
{
}

Paw_position No_movement::determine_paws_position(Side &side)
{
	m_xy_good_position = test_for_good_position_xy(side);
	m_z_good_position = test_for_good_position_z(side);
	//get up paws only if one or more paw is not at the correct x,y or z position
	if(!m_xy_good_position)
	{
		determine_x_paws_position(side);
		determine_y_paws_position(side);
		determine_z_paws_position(side);
	}
	else
	{
		determine_x_paws_position(side);
		determine_y_paws_position(side);
		determine_z_paws_position_not_get_up(side);
	}

	return m_paw_position;
}

//permit to create a more realistic movement
bool No_movement::is_sequence_finished(Side &side)
{
	if(m_current_step_number >= 15)//minimal step number
	{
		if(m_xy_good_position && m_z_good_position)
			return 1;
	}
	return 0; //movement doesn't finished
}


