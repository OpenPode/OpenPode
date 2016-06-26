/*
 * led_controller.cpp
 *
 *  Created on: 7 juin. 2016
 *      Author: Guilaume
 */

#include "drivers/led/led_controller.h"
#include <iostream>

Led_controller::Led_controller(RPiGPIOPin p_red_pin, RPiGPIOPin p_green_pin, RPiGPIOPin p_blue_pin, Led_color p_color) : m_blue_pin(p_blue_pin), m_red_pin(p_red_pin), m_green_pin(p_green_pin),
														m_color_standard(p_color), m_color_debug(NONE)
{
	bcm2835_gpio_fsel(p_red_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(p_green_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(p_blue_pin, BCM2835_GPIO_FSEL_OUTP);
	set_color(m_color_standard);
}


Led_controller::~Led_controller()
{

}

void Led_controller::set_debug(Debug_by_color debug)
{
	switch(debug)
	{
	case ok:
	{
		m_color_debug = m_color_standard;
	}
	break;
	case error:
	{
		if(m_color_standard != Led_color::RED)
			m_color_debug = Led_color::RED;
		else
			m_color_debug = Led_color::MAGENTA;
	}
	break;
	case warning:
	{
		if(m_color_standard != Led_color::YELLOW)
			m_color_debug = Led_color::YELLOW;
		else
			m_color_debug = Led_color::MAGENTA;
	}
	break;
	case info:
	{
		if(m_color_standard != Led_color::BLUE)
			m_color_debug = Led_color::BLUE;
		else
			m_color_debug = Led_color::GREEN;
	}
	break;
	}

	set_color(m_color_debug);
}

void Led_controller::set_standard()
{
	set_color(m_color_standard);
}

void Led_controller::change_color(Led_color p_color)
{
	set_color(p_color);
}

void Led_controller::update_color(Led_color p_color)
{
	m_color_standard = p_color;
	set_color(m_color_standard);
}

void Led_controller::increase_color()
{
	int color = int(m_color_standard);
	color ++;
	m_color_standard = (Led_color)color;
	if (m_color_standard > Led_color::WHITE)
		m_color_standard = NONE;

	set_color(m_color_standard);
}

Led_controller Led_controller::operator ++(int)
{
	increase_color();
	return *this;
}

void Led_controller::decrease_color()
{
	int color = int(m_color_standard);
	color --;
	m_color_standard = (Led_color)color;
	if (m_color_standard < Led_color::NONE)
		m_color_standard = WHITE;

	set_color(m_color_standard);
}

Led_controller Led_controller::operator --(int)
{
	decrease_color();
	return *this;
}

void Led_controller::set_color(Led_color color)
{
	if((int)color & (int)Led_color::RED)
		bcm2835_gpio_set(m_red_pin);
	else
		bcm2835_gpio_clr(m_red_pin);

	if((int)color & (int)Led_color::GREEN)
		bcm2835_gpio_set(m_green_pin);
	else
		bcm2835_gpio_clr(m_green_pin);

	if((int)color & (int)Led_color::BLUE)
		bcm2835_gpio_set(m_blue_pin);
	else
		bcm2835_gpio_clr(m_blue_pin);
}
