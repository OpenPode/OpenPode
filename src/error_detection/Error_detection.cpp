/*
 * Error_detection.cpp
 *
 *  Created on: 25 avr. 2016
 *      Author: Sarthou
 */

#include "Error_detection.h"

Error_detection::Error_detection() : error_code(0), error_location(0),
									 error_paw_R_F(0), error_paw_R_M(0), error_paw_R_B(0),
									 error_paw_L_F(0), error_paw_L_M(0), error_paw_L_B(0)
{

}

Error_detection::~Error_detection()
{

}

void Error_detection::reset()
{
	error_code = 0;
	error_location = 0;
	error_paw_R_F = 0;
	error_paw_R_M = 0;
	error_paw_R_B = 0;
	error_paw_L_F = 0;
	error_paw_L_M = 0;
	error_paw_L_B = 0;
}

void Error_detection::set_error()
{
	error_code |= ERROR;
}

void Error_detection::set_paw(Paw p_paw)
{

}
