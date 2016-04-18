/*
 * No_movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "No_movement.h"
#include "Side.h"
#include "hexapode_dimensions.h"

No_movement::No_movement() : Movement(no_movement, direction_front, 0, 0, 30)
{

}

void No_movement::determine_x_paws_position(Side &side, int sequence_number)
{
	m_paw_position.front[coord_x]  = side.get_paws_position().front_paw.x;
	m_paw_position.middle[coord_x] = side.get_paws_position().middle_paw.x;
	m_paw_position.back[coord_x]   = side.get_paws_position().back_paw.x;

	//remember must go to the position : goto_position(present, futur, nb_step);
	if(side.get_side_id() == side_left)
	{
		if(sequence_number == 0)
			m_paw_position.front[coord_x]  = goto_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, m_step_number - m_current_step_number);
		else if(sequence_number == 2)
			m_paw_position.back[coord_x]   = goto_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, m_step_number - m_current_step_number);
	}
	else
	{
		if(sequence_number == 0)
			m_paw_position.back[coord_x]   = goto_position(side.get_back_paw().m_current_coords.x, side.get_back_paw().m_x_center, m_step_number - m_current_step_number);
		else if(sequence_number == 1)
			m_paw_position.middle[coord_x] = goto_position(side.get_middle_paw().m_current_coords.x, side.get_middle_paw().m_x_center, m_step_number - m_current_step_number);
		else if(sequence_number == 2)
			m_paw_position.front[coord_x]  = goto_position(side.get_front_paw().m_current_coords.x, side.get_front_paw().m_x_center, m_step_number - m_current_step_number);
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
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading - 0), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_y] = reproach_position(side.get_middle_paw().m_current_coords.y, side.get_side_coef() * (paw_spreading - 0), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_y]   = reproach_position(side.get_back_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_y]  = reproach_position(side.get_front_paw().m_current_coords.y, side.get_side_coef() * paw_spreading, NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_z_paws_position(Side &side, int sequence_number, double a, double b)
{
	m_paw_position.front[coord_z]  = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + b;
	m_paw_position.middle[coord_z] = a*m_paw_position.middle[coord_x] + b;
	m_paw_position.back[coord_z]   = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + b;

	if(sequence_number == 0)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.front[coord_z]  = get_up_paw(a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + b, side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.back[coord_z]   = get_up_paw(a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + b, side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 1)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.middle[coord_z] = get_up_paw(a*(side.get_middle_paw().m_x_center + m_distance / 2) + b, side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.middle[coord_z] = get_up_paw(a*(side.get_middle_paw().m_x_center + m_distance / 2) + b, side.get_middle_paw(), NO_MOVEMENT_STEP_DIST);
	}
	else if(sequence_number == 2)
	{
		if(side.get_side_id() == side_left)
			m_paw_position.back[coord_z]   = get_up_paw(a*(side.get_back_paw().m_x_center + m_distance / 2 - HALF_LENGTH) + b, side.get_back_paw(), NO_MOVEMENT_STEP_DIST);
		else
			m_paw_position.front[coord_z]  = get_up_paw(a*(side.get_front_paw().m_x_center + m_distance / 2 + HALF_LENGTH) + b, side.get_front_paw(), NO_MOVEMENT_STEP_DIST);
	}
}

void No_movement::determine_z_paws_position_not_get_up(double a, double b)
{
	m_paw_position.front[coord_z]  = a*(m_paw_position.front[coord_x] + HALF_LENGTH) + b;
	m_paw_position.middle[coord_z] = a*m_paw_position.middle[coord_x] + b;
	m_paw_position.back[coord_z]   = a*(m_paw_position.back[coord_x] - HALF_LENGTH) + b;
}

bool No_movement::test_for_get_up(Side &side, double paw_spreading, int sequence_number)
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
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_paws_position().front_paw.x != side.get_front_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_paws_position().back_paw.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
}

bool No_movement::test_for_get_up_sequence1(Side &side, double paw_spreading)
{
	//test if one or more paw is not at the correct x,y or z position
	if( (side.get_middle_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_paws_position().middle_paw.x != side.get_middle_paw().m_x_center))
		return 0;
	else
		return 1;
}

bool No_movement::test_for_get_up_sequence2(Side &side, double paw_spreading)
{
	//test if one or more paw is not at the correct x,y or z position
	if(side.get_side_id() == side_left)
		if( (side.get_back_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_paws_position().back_paw.x != side.get_back_paw().m_x_center))
			return 0;
		else
			return 1;
	else
		if( (side.get_front_paw().m_current_coords.y  != side.get_side_coef() * paw_spreading)  || (side.get_paws_position().front_paw.x != side.get_front_paw().m_x_center))
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

Paw_position No_movement::determine_paws_position(Side &side, int sequence_number, double a, double b, double paw_spreading)
{
	//get up paws only if one or more paw is not at the correct x,y or z position
	if(!test_for_get_up(side, paw_spreading, sequence_number))
	{
		determine_x_paws_position(side, sequence_number);
		determine_y_paws_position(side, sequence_number, paw_spreading);
		determine_z_paws_position(side, sequence_number, a, b);
	}
	else
	{
		determine_x_paws_position(side, sequence_number);
		determine_y_paws_position(side, sequence_number, paw_spreading);
		determine_z_paws_position_not_get_up(a, b);
	}

	return m_paw_position;
}


