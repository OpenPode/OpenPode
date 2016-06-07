/*
 * led_controller.cpp
 *
 *  Created on: 7 juin. 2016
 *      Author: Guilaume
 */

#include "led/led_controller.h"
#include <iostream>

led_controller::led_controller(RPiGPIOPin p_red_pin, RPiGPIOPin p_green_pin, RPiGPIOPin p_blue_pin, color_t p_color) : m_blue_pin(p_blue_pin), m_red_pin(p_red_pin), m_green_pin(p_green_pin), m_color(p_color)
{
	bcm2835_gpio_fsel(p_red_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(p_green_pin, BCM2835_GPIO_FSEL_OUTP);
	bcm2835_gpio_fsel(p_blue_pin, BCM2835_GPIO_FSEL_OUTP);
	set_color();
}


led_controller::~led_controller()
{

}

void led_controller::change_color(color_t p_color)
{
	m_color = p_color;
	set_color();
}

void led_controller::increase_color()
{
	int color = int(m_color);
	color ++;
	m_color = (color_t)color;
	if (m_color > color_t::WHITE)
		m_color = NONE;

	set_color();
}

led_controller led_controller::operator ++(int)
{
	increase_color();
	return *this;
}

void led_controller::decrease_color()
{
	int color = int(m_color);
	color --;
	m_color = (color_t)color;
	if (m_color < color_t::NONE)
		m_color = WHITE;

	set_color();
}

led_controller led_controller::operator --(int)
{
	decrease_color();
	return *this;
}

void led_controller::set_color()
{
	if((int)m_color & (int)color_t::RED)
		bcm2835_gpio_set(m_red_pin);
	else
		bcm2835_gpio_clr(m_red_pin);

	if((int)m_color & (int)color_t::GREEN)
		bcm2835_gpio_set(m_green_pin);
	else
		bcm2835_gpio_clr(m_green_pin);

	if((int)m_color & (int)color_t::BLUE)
		bcm2835_gpio_set(m_blue_pin);
	else
		bcm2835_gpio_clr(m_blue_pin);

	//bcm2835_gpio_fsel(pin, mode)
}
