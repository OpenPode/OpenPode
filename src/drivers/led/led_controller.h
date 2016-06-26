/*
 * led_controller.h
 *
 *  Created on: 7 juin. 2016
 *      Author: Guillaume
 */

#ifndef LED_CONTROLLER_H_
#define LED_CONTROLLER_H_

#include <cstdint>
#include "drivers/broadcom/bcm2835.h"

enum Led_color
{
	NONE    = 0x00,
	RED     = 0x01,
	GREEN   = 0x02,
	BLUE    = 0x04,
	YELLOW  = RED   | GREEN,
	MAGENTA = RED   | BLUE,
	CYAN    = GREEN | BLUE,
	WHITE   = RED   | GREEN | BLUE
};

enum Debug_by_color
{
	ok,
	error,
	warning,
	info
};

class Led_controller
{
public:

	Led_controller(RPiGPIOPin p_red_pin, RPiGPIOPin p_green_pin, RPiGPIOPin p_blue_pin, Led_color p_color = NONE);
	~Led_controller();

	void set_debug(Debug_by_color debug);
	void set_standard();
	void change_color(Led_color p_color);
	void update_color(Led_color p_color);
	void increase_color();
	void decrease_color();

	Led_controller operator ++(int);
	Led_controller operator --(int);

protected:
	RPiGPIOPin m_blue_pin;
	RPiGPIOPin m_red_pin;
	RPiGPIOPin m_green_pin;

	Led_color m_color_standard;
	Led_color m_color_debug;

	void set_color(Led_color color);
};

#endif /* LED_CONTROLLER */
