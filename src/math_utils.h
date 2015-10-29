/*
 * math_utils.h
 *
 *  Created on: 29 oct. 2015
 *      Author: Julien
 */

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

#include <cmath>

namespace utils
{

inline double to_rad(double deg)
{
	return deg * M_PI / 180.0;
}

inline double to_deg(double rad)
{
	return rad * 180.0 / M_PI;
}

}

#endif /* MATH_UTILS_H_ */
