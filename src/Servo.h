/*
 * Servo.h
 *
 *  Created on: 29 sept. 2015
 *      Author: Julien
 */

#ifndef SERVO_H_
#define SERVO_H_

class Servo
{
public:

	Servo(int id);

	void move(double degrees);

protected:

	int m_id;
};



#endif /* SERVO_H_ */
