/*
 * Error_detection.h
 *
 *  Created on: 25 avr. 2016
 *      Author: Sarthou
 */

#ifndef ERROR_DETECTION_ERROR_DETECTION_H_
#define ERROR_DETECTION_ERROR_DETECTION_H_

#include "Side.h"

//mask for error_code
#define ERROR 		0x01;
#define SIDE  		0x06;
#define SIDE_LEFT 	0x04;
#define SIDE_RIGHT 	0x02;
#define ERROR_TYPE  0x18;
#define MECA_LIMIT	0x08;
#define MODEL		0x10;
#define SEQUENCE	0x60;
#define IN_SEQ		0x20;
#define STANDARD	0x40;

//mask for error_location
#define FRONT 		0x01;
#define MIDDLE 		0x02;
#define BACK 		0x04;
//shift for error_location
#define RIGHT_SHIFT	0;
#define LEFT_SHIFT	4;

//mask for error_paw_X_X
#define TIBIA		0x01;
#define	FEMUR		0x02;
#define	COXA		0x04;
//shift for error_paw_X_X
#define MECA_LIMIT_SHIFT 	0;
#define MODEL_LIMIT_SHIFT 	4;

class Error_detection
{
public:

	Error_detection();
	~Error_detection();

	void reset();

	void set_error();
	void set_paw(Paw p_paw);

protected:

	char error_code;
	char error_location;
	char error_paw_R_F;
	char error_paw_R_M;
	char error_paw_R_B;
	char error_paw_L_F;
	char error_paw_L_M;
	char error_paw_L_B;

};

#endif /* ERROR_DETECTION_ERROR_DETECTION_H_ */
