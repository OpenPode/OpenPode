/*
 * Vector.h
 *
 *  Created on: 27 juin 2016
 *      Author: julien
 */

#ifndef SRC_UTILITY_VECTOR_H_
#define SRC_UTILITY_VECTOR_H_

template<typename T>
struct Vector2D
{
	T x;
	T y;
};

template<typename T>
struct Vector3D
{
	T x;
	T y;
	T z;
};

using Vector2f = Vector2D<float>;
using Vector3f = Vector3D<float>;

using Vector2i = Vector2D<int>;
using Vector3i = Vector3D<int>;

#endif /* SRC_UTILITY_VECTOR_H_ */
