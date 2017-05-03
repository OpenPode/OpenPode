//============================================================================
// Name        : Movement_controller.cpp
// Author      : Sarthou
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "controller/Movement_controller.h"
#include "drivers/broadcom/bcm2835.h"
#include "config.h"
#include "movement/Linear_movement.h"
#include "movement/No_movement.h"
#include "movement/complete_linear_movement.h"
#include "movement/Circular_movement.h"
#include <cmath>
#include <cstdlib>
#include <cstring>

#define MOVEMENT_DATA 	0x01
	#define DIR_FRONT	0x01
	#define DIR_BACK	0x02
	#define DIR_LEFT	0x03
	#define DIR_RIGHT	0x04
	#define SPEED 0.5
#define SPREADING_DATA 	0x02
#define INCLINE_DATA 	0x03

Movement_controller::Movement_controller()
{
	/*
	 * Wait for incoming connection
	 */
	m_listener.listen(bt::BtAddress::Any, 1);
	int connected = 0;
	while(connected < 1)
	{
		 if(m_listener.accept(m_client) == bt::Status::Done)
		 {
			 connected++;
			 std::cout << "New connection received from " << m_client.getRemoteAddress().toString() << std::endl;
		 }
	}
}

Movement_controller::~Movement_controller()
{

}

void Movement_controller::run_controller()
{
	size_t recv = 0;
	std::cout << "a "<< std::endl;
	m_client.receive(data_input, sizeof(data_input), recv);
	std::cout << "recv bytes = " << recv << std::endl;
}

void Movement_controller::get_control_values()
{
	switch(data_input[0])
	{
	case MOVEMENT_DATA:
		switch(data_input[1])
		{
		case DIR_FRONT:
			m_movement_x_lin_value = SPEED;
			m_movement_y_lin_value = 0;
			break;
		case DIR_BACK:
			m_movement_x_lin_value = -SPEED;
			m_movement_y_lin_value = 0;
			break;
		case DIR_LEFT:
			m_movement_x_lin_value = 0;
			m_movement_y_lin_value = SPEED;
			break;
		case DIR_RIGHT:
			m_movement_x_lin_value = 0;
			m_movement_y_lin_value = -SPEED;
			break;
		default:
			m_movement_x_lin_value = 0;
			m_movement_y_lin_value = 0;
		}
		break;
	case SPREADING_DATA:
		update_paw_spreading(); //data_input[1]
		update_center_height(); // data_input[2]
		 break;
	case INCLINE_DATA:
		m_incline_pitch_lin_value = ((int8_t)data_input[1])/100.;
		m_incline_roll_lin_value = ((int8_t)data_input[2])/100.;
		break;
	default:
		break;
	}
}

void Movement_controller::update_paw_spreading()
{
	m_paw_spreading = TIBIA_ORIGIN_OFFSET + (data_input[1]/255.f)*(TIBIA_LENGTH + FEMUR_LENGTH - TIBIA_ORIGIN_OFFSET);
	if(m_paw_spreading > (TIBIA_LENGTH + FEMUR_LENGTH))
		m_paw_spreading = TIBIA_LENGTH + FEMUR_LENGTH;
	else if(m_paw_spreading < TIBIA_ORIGIN_OFFSET)
		m_paw_spreading = TIBIA_ORIGIN_OFFSET;
}

void Movement_controller::update_center_height()
{
	m_center_height = (data_input[2]/255.f)*(TIBIA_LENGTH + FEMUR_LENGTH);
	if(m_center_height > (TIBIA_LENGTH + FEMUR_LENGTH))
		m_center_height = TIBIA_LENGTH + FEMUR_LENGTH;
	else if(m_center_height < 0)
		m_center_height = 0;

	m_center_height = -m_center_height;
}
