/*
 * Movement.cpp
 *
 *  Created on: 18 nov. 2015
 *      Author: Julien
 */

#include "Movement.h"
#include "Side.h"
#include <cmath>

Movement::Movement(Movement_type type, Movement_direction direction, double distance, double angle, int step_number) :
	m_type(type), m_direction(direction), m_distance(distance), m_angle(angle), m_step_number(step_number),
	m_step_distance_x(0), m_step_distance_z(0), m_corrected_distance(0), m_current_step_number(0)
{

}

double Movement::reproach_position(double present, double futur, double p_step_distance)
{
	if((present - futur) <= - p_step_distance)
	{
		return(present + p_step_distance);
	}
	else if((present - futur) >= p_step_distance)
	{
		return(present - p_step_distance);
	}
	else
	{
		return futur;
	}
}

double Movement::get_up_paw(double final_height, Paw &paw, double p_step_distance)
{
	double z;

	if(m_current_step_number <= (m_step_number / 2))
	{
		if(paw.m_current_coords.z <= -30)
			z = paw.m_current_coords.z + p_step_distance*2.5;
		else
			z = paw.m_current_coords.z;
	}
	else
	{
		double step_distance_get_up = abs((final_height - paw.m_current_coords.z) / (m_step_number - m_current_step_number));
		z = paw.m_current_coords.z - step_distance_get_up;
	}

	return z;
}

double Movement::goto_position(double present, double futur, double nb_step)
{
	double step_distance = abs(present - futur) / nb_step;
	//cout << "step_dist " << step_distance << " nb_step " << nb_step << endl;
	if(futur > present)
	{
		return(present + step_distance);
	}
	else if(futur < present)
	{
		return(present - step_distance);
	}
	else
	{
		return futur;
	}
}
