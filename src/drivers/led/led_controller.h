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

enum color_t
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

class led_controller
{
public:

	led_controller(RPiGPIOPin p_red_pin, RPiGPIOPin p_green_pin, RPiGPIOPin p_blue_pin, color_t p_color = NONE);
	~led_controller();

	void change_color(color_t p_color);
	void update_color(color_t p_color);
	void increase_color();
	void decrease_color();

	led_controller operator ++(int);
	led_controller operator --(int);

protected:
	RPiGPIOPin m_blue_pin;
	RPiGPIOPin m_red_pin;
	RPiGPIOPin m_green_pin;

	color_t m_color_standard;

	void set_color(color_t color);
};

#endif /* LED_CONTROLLER */
