/*
 * Servo.cpp
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#include "Servo.h"

Servo::Servo(int id) : m_id(id)
{

}

void Servo::move(double degrees)
{
	/*transmit degrees to the corresponding motor on I2C bus (with m_id)*/
}


