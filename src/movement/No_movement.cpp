/*
 * No_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "No_movement.h"
#include "Side.h"
#include "hexapode_dimensions.h"

No_movement::No_movement() : Movement(no_movement, direction_front, 0, 0, 30), m_z_good_position(0), m_xy_good_position(0)
{

}

void No_movement::determine_x_paws_position(Side &side, int sequence_number)
{
	m_paw_position.front[coord_x]  = side.get_paws_position().front_paw.x;
	m_paw_position.middle[coord_x] = side.get_paws_position().middle_paw.x;
	m_paw_position.back[coord_x]   = side.get_paws_position().back_paw.x;

	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if(side.get_side_id() == side_left)
	{
		if(sequence_number == 0)
			m_paw_position.front[coord_x]  = reproach_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = reproach_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(sequence_number == 2)
			m_paw_position.back[coord_x]   = reproach_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
	}
	else
	{
		if(sequence_number == 0)
			m_paw_position.back[coord_x]   = reproach_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = reproach_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
		else if(sequence_number == 2)
			m_paw_position.front[coord_x]  = reproach_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_y_paws_position(Side &side, int sequence_number, double paw_spreading)
{
	m_paw_position.front[coord_y] = side.get_front_paw().m_current_coords.y;
	m_paw_position.middle[coord_y] = side.get_middle_paw().m_current_coords.y;
	m_paw_position.back[coord_y] = side.get_back_paw().m_current_coords.y;

	//remember try to go to the position: reproach_position(present, futur, step_dist);
	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_y]  = reproach_position(side.get_front_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_y]   = reproach_position(side.get_back_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = reproach_position(side.get_back_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_y]  = reproach_position(side.get_front_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_z_paws_position(Side &side, int sequence_number, double a, double b, double c)
{
	m_paw_position.front[coord_z]  = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + c;
	m_paw_position.middle[coord_z] = a*m_paw_position.middle[coord_x] + c;
	m_paw_position.back[coord_z]   = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + c;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = just_get_up_paw(side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_z]   = just_get_up_paw(side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_z] = just_get_up_paw(side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_z] = just_get_up_paw(side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = just_get_up_paw(side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_z]  = just_get_up_paw(side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_z_paws_position_not_get_up(Side &side, int sequence_number, double a, double b, double c)
{
	m_paw_position.front[coord_z]  = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + c;
	m_paw_position.middle[coord_z] = a*m_paw_position.middle[coord_x] + c;
	m_paw_position.back[coord_z]   = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + c;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = just_get_down_paw(a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + c, side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_z]   = just_get_down_paw(a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + c, side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_z] = just_get_down_paw(a*(side.get_middle_paw().m_x_center + m_distance / 2) + c, side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_z] = just_get_down_paw(a*(side.get_middle_paw().m_x_center + m_distance / 2) + c, side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = just_get_down_paw(a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + c, side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_z]  = just_get_down_paw(a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + c,side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
	}
}

bool No_movement::test_for_good_position_z(Side &side,double a, double b, double c, int sequence_number)
{
	//test if one or more paw is not at the correct x,y or z position
	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			return((a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + c) == (side.get_front_paw().m_current_coords.z));
		else
			return((a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + c) == ( side.get_back_paw().m_current_coords.z));
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			return((a*(side.get_middle_paw().m_x_center + m_distance / 2) + c) == ( side.get_middle_paw().m_current_coords.z));
		else
			return((a*(side.get_middle_paw().m_x_center + m_distance / 2) + c) == ( side.get_middle_paw().m_current_coords.z));
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			return((a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + c) == ( side.get_back_paw().m_current_coords.z));
		else
			return((a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + c) == (side.get_front_paw().m_current_coords.z));
	}
	return 0;
}

bool No_movement::test_for_good_position_xy(Side &side, double paw_spreading, int sequence_number)
{
	//test if one or more paw is not at the correct x,y or z position
	if(sequence_number == 0)
		return test_for_get_up_sequence0(side, paw_spreading);
	else if(sequence_number == 1)
		return test_for_get_up_sequence1(side, paw_spreading);
	else if(sequence_number == 2)
		return test_for_get_up_sequence2(side, paw_spreading);
	else
		return 0;
}

bool No_movement::test_for_get_up_sequence0(Side &side, double paw_spreading)
{
	//test if one or more paw is not at the correct x,y or z position
	if(side.get_side_id() == side_left)
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_front_paw().m_current_coords.x != side.get_front_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_back_paw().m_current_coords.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
}

bool No_movement::test_for_get_up_sequence1(Side &side, double paw_spreading)
{
	//test if one or more paw is not at the correct x,y or z position
	if( (side.get_middle_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_middle_paw().m_current_coords.x != side.get_middle_paw().m_x_center))
		return 0;
	else
		return 1;
}

bool No_movement::test_for_get_up_sequence2(Side &side, double paw_spreading)
{
	//test if one or more paw is not at the correct x,y or z position
	if(side.get_side_id() == side_left)
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_back_paw().m_current_coords.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_front_paw().m_current_coords.x != side.get_front_paw().m_x_center))
			return 0;
		else
			return 1;
}

double No_movement::determine_real_distance(Side &side)
{
	return 0;
}

void No_movement::compute_variables()
{
}

Paw_position No_movement::determine_paws_position(Side &side, int sequence_number, double a, double b, double c, double paw_spreading)
{
	m_paw_spreading = paw_spreading;

	m_xy_good_position = test_for_good_position_xy(side, paw_spreading, sequence_number);
	m_z_good_position = test_for_good_position_z(side, a, b, c,  sequence_number);
	//get up paws only if one or more paw is not at the correct x,y or z position
	if(!m_xy_good_position)
	{
		determine_x_paws_position(side, sequence_number);
		determine_y_paws_position(side, sequence_number, paw_spreading);
		determine_z_paws_position(side, sequence_number, a, b, c);
	}
	else
	{
		determine_x_paws_position(side, sequence_number);
		determine_y_paws_position(side, sequence_number, paw_spreading);
		determine_z_paws_position_not_get_up(side, sequence_number, a, b, c);
	}

	return m_paw_position;
}

//permit to create a more realistic movement
bool No_movement::is_sequence_finished(Side &side, int sequence_number)
{
	if(m_current_step_number >= 15)//minimal step number
	{
		if(m_xy_good_position && m_z_good_position)
			return 1;
	}
	return 0; //movement doesn't finished
}


