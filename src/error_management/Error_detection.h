/*
 * Error_detection.h
 *
 *  Created on: 25 avr. 2016
 *      Author: Sarthou
 */

#ifndef ERROR_MANAGEMENT_ERROR_DETECTION_H_
#define ERROR_MANAGEMENT_ERROR_DETECTION_H_

#include "paw/Paw.h"

//mask for error_code
constexpr uint8_t ERROR 		=0x01;
constexpr uint8_t SIDE  		=0x06;
constexpr uint8_t SIDE_LEFT 	=0x04;
constexpr uint8_t SIDE_RIGHT 	=0x02;
constexpr uint8_t ERROR_TYPE  	=0x18;
constexpr uint8_t MECA_LIMIT	=0x08;
constexpr uint8_t MODEL		=0x10;
constexpr uint8_t SEQUENCE		=0x60;
constexpr uint8_t IN_SEQ		=0x20;
constexpr uint8_t STANDARD		=0x40;

//mask for error_location
constexpr uint8_t FRONT 		=0x01;
constexpr uint8_t MIDDLE 		=0x02;
constexpr uint8_t BACK 		=0x04;
//shift for error_location
constexpr uint8_t RIGHT_SHIFT	=0;
constexpr uint8_t LEFT_SHIFT	=4;

//mask for error_paw_X_X
constexpr uint8_t TIBIA 		=0x01;
constexpr uint8_t FEMUR 		=0x02;
constexpr uint8_t COXA  		=0x04;
constexpr uint8_t IN_SEQUENCE	=0x08;
//shift for error_paw_X_X
constexpr uint8_t MECA_LIMIT_SHIFT   =0;
constexpr uint8_t MODEL_LIMIT_SHIFT  =4;


class Error_detection
{
public:

	Error_detection();
	~Error_detection();

	void reset();
	void set_sequence_number(int p_sequence_number);

	void set_error();
	void test_error();
	void set_paw(Paw &p_paw);

	bool is_on_error();
	bool is_on_sequence();

//protected:

	uint8_t error_code;
	uint8_t error_location;
	uint8_t error_paw_R_F;
	uint8_t error_paw_R_M;
	uint8_t error_paw_R_B;
	uint8_t error_paw_L_F;
	uint8_t error_paw_L_M;
	uint8_t error_paw_L_B;
	int m_sequence_number;

	void set_error_model_type();
	void set_error_mecanical_type();
	void set_sequence(Paw &p_paw);

	void test_machenical_stop_limit(Paw &p_paw, uint8_t *p_error_paw);
	void test_model_limit(Paw &p_paw, uint8_t *p_error_paw);
	uint8_t* select_paw_code(Paw &p_paw);

};

#endif /* ERROR_MANAGEMENT_ERROR_DETECTION_H_ */
