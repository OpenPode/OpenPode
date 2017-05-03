/*
 * Movement_controller.h
 *
 *  Created on: 18 avr. 2016
 *      Author: Sarthou
 */

#ifndef CONTROLLER_MOVEMENT_CONTROLLER_H_
#define CONTROLLER_MOVEMENT_CONTROLLER_H_

#include "controller/PS4Controller.h"
#include "movement/Incline_coef.h"
#include "movement/Movement.h"
#include "controller/Movement_controller_base.h"
#include "Bluetooth/RfcommListener.h"
#include "Bluetooth/RfcommSocket.h"
#include <array>

class Movement;

class Movement_controller : public Movement_controller_base
{
public:

	Movement_controller();
	~Movement_controller();

	void run_controller();

private:
	uint8_t data_input[10];

	bt::RfcommListener m_listener;
	bt::RfcommSocket m_client;

	void get_control_values();

	void update_paw_spreading();
	void update_center_height();
};

#endif /* CONTROLLER_MOVEMENT_CONTROLLER_H_ */
