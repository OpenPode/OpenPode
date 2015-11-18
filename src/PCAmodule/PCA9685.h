/*
 * PCA9685.h
 *
 *  Created on: 26 oct. 2015
 *      Author: Julien
 */

#ifndef PCA9685_H_
#define PCA9685_H_

#include "i2cdev.h"

enum Channel_t
{
	channel0  	= 0x06,
	channel1  	= 0x0A,
	channel2  	= 0x0E,
	channel3  	= 0x12,
	channel4  	= 0x16,
	channel5  	= 0x1A,
	channel6  	= 0x1E,
	channel7  	= 0x22,
	channel8  	= 0x26,
	channel9  	= 0x2A,
	channel10 	= 0x2E,
	channel11 	= 0x32,
	channel12 	= 0x36,
	channel13 	= 0x3A,
	channel14 	= 0x3E,
	channel15 	= 0x42,
	allChannel 	= 0xFA
};

class PCA9685
{
public:

	PCA9685();
	PCA9685(i2cdev *i2c, uint8_t address);
	~PCA9685();

	void set_on_time_all_channel(int on_time);
	void set_off_time_all_channel(int off_time);
	void set_on_time(Channel_t channel, int on_time);
	void set_off_time(Channel_t channel, int off_time);
	void set_prescale(int prescale);
	uint8_t get_prescale();

protected:

	i2cdev *m_i2c;
	uint8_t m_address;
};



#endif /* PCA9685_H_ */
