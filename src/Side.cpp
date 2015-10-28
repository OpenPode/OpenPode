/*
 * Side.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */


#include "Side.h"

Side::Side(Side_enum side, i2cdev *i2c) : m_side(side),
										  m_paw1(side, position_front),
										  m_paw2(side, position_middle),
										  m_paw3(side, position_back)
{
	if(m_side == side_left)
		m_module = PCA9685(i2c, 0x42);
	else
		m_module = PCA9685(i2c, 0x41);

}

