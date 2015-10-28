/*
 * PCA9685.cpp
 *
 *  Created on: 26 oct. 2015
 *      Author: Julien
 */

#include "PCA9685.h"

PCA9685::PCA9685() : m_i2c(nullptr), m_address(0)
{

}

PCA9685::PCA9685(i2cdev *i2c, uint8_t address) : m_i2c(i2c), m_address(address)
{

}

PCA9685::~PCA9685()
{

}

void PCA9685::set_on_time_all_channel(int on_time)
{
	m_i2c->write_byte_reg(m_address, Channel_t::allChannel  , uint8_t(on_time&0xFF));
	m_i2c->write_byte_reg(m_address, Channel_t::allChannel+1, uint8_t((on_time>>8)&0x0F));
}

void PCA9685::set_off_time_all_channel(int off_time)
{
	m_i2c->write_byte_reg(m_address, Channel_t::allChannel+2, uint8_t(off_time&0xFF));
	m_i2c->write_byte_reg(m_address, Channel_t::allChannel+3, uint8_t((off_time>>8)&0x0F));
}

void PCA9685::set_on_time(Channel_t channel, int on_time)
{
	m_i2c->write_byte_reg(m_address, channel  , uint8_t(on_time&0xFF));
	m_i2c->write_byte_reg(m_address, channel+1, uint8_t((on_time>>8)&0x0F));
}

void PCA9685::set_off_time(Channel_t channel, int off_time)
{
	m_i2c->write_byte_reg(m_address, channel+2, uint8_t(off_time&0xFF));
	m_i2c->write_byte_reg(m_address, channel+3, uint8_t((off_time>>8)&0x0F));
}

void PCA9685::set_prescale(int prescale)
{
	m_i2c->write_byte_reg(m_address, 0, 0x10);
	m_i2c->write_byte_reg(m_address, 0xFE , prescale);
	m_i2c->write_byte_reg(m_address, 0, 0);
}

uint8_t PCA9685::get_prescale()
{
	uint8_t prescale;
	m_i2c->read_byte_reg(m_address, 0xFE , &prescale);
	return prescale;
}

