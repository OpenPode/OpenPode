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
constexpr char ERROR 		=0x01;
constexpr char SIDE  		=0x06;
constexpr char SIDE_LEFT 	=0x04;
constexpr char SIDE_RIGHT 	=0x02;
constexpr char ERROR_TYPE  	=0x18;
constexpr char MECA_LIMIT	=0x08;
constexpr char MODEL		=0x10;
constexpr char SEQUENCE		=0x60;
constexpr char IN_SEQ		=0x20;
constexpr char STANDARD		=0x40;

//mask for error_location
constexpr char FRONT 		=0x01;
constexpr char MIDDLE 		=0x02;
constexpr char BACK 		=0x04;
//shift for error_location
constexpr char RIGHT_SHIFT	=0;
constexpr char LEFT_SHIFT	=4;

//mask for error_paw_X_X
constexpr char TIBIA 		=0x01;
constexpr char FEMUR 		=0x02;
constexpr char COXA  		=0x04;
//shift for error_paw_X_X
constexpr char MECA_LIMIT_SHIFT   =0;
constexpr char MODEL_LIMIT_SHIFT  =4;

class Error_detection
{
public:

	Error_detection();
	~Error_detection();

	void reset();
	void set_sequence_number(int p_sequence_number);

	void set_error();
	void set_paw(Paw &p_paw);

	bool is_on_error();

//protected:

	char error_code;
	char error_location;
	char error_paw_R_F;
	char error_paw_R_M;
	char error_paw_R_B;
	char error_paw_L_F;
	char error_paw_L_M;
	char error_paw_L_B;
	int m_sequence_number;

	void set_error_location(Paw &p_paw);
	void set_error_model_type();
	void set_error_mecanical_type();
	void set_sequence(Paw &p_paw);

	void test_machenical_stop_limit(Paw &p_paw, char* p_error_paw);
	void test_model_limit(Paw &p_paw, char* p_error_paw);
	char* select_paw_code(Paw &p_paw);

};

#endif /* ERROR_MANAGEMENT_ERROR_DETECTION_H_ */
