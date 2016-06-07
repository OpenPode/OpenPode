/*
 * math_utils.h
 *
 *  Created on: 29 oct. 2015
 *      Author: Julien
 */

#ifndef MATH_UTILS_H_
#define MATH_UTILS_H_

#include <cmath>

namespace util
{

constexpr float PI = float(M_PI);

namespace
{

constexpr float to_rad_coef = PI/180.0f;
constexpr float to_deg_coef = 180.0f/PI;

}

inline float to_rad(float deg)
{
	return deg * to_rad_coef;
}

inline float to_deg(float rad)
{
	return rad * to_deg_coef;
}

}

#endif /* MATH_UTILS_H_ */
