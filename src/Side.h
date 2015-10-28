/*
 * Side.h
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#ifndef SIDE_H_
#define SIDE_H_

#include "Paw.h"
#include "PCA9685.h"
#include "enums.h"

class i2cdev;

class Side
{
public:

	Side(Side_enum side, i2cdev *i2c);

protected:

	Side_enum m_side;

	Paw m_paw1;
	Paw m_paw2;
	Paw m_paw3;

	PCA9685 m_module;
};


#endif /* SIDE_H_ */
