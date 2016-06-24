/*
 * hexapode_description.h
 *
 *  Created on: 24 juin 2016
 *      Author: Sarthou
 */

#ifndef HEXAPODE_DESCRIPTION_H_
#define HEXAPODE_DESCRIPTION_H_

/*
 * define what you want to execute
 */
//#define CALIBRATION
#define ERROR_ACTION
#define HEAD


/*				   HEXAPODE
 *
 * 					  X
 * 					  |
 * 					  |
 * 			   Y______Z
 *
 *       LEFT 		 __		RIGHT
 *         			/  \
 *       		   /_||_\
 *     			 __________
 *   		    /		   \
 *   _____|____/			\_____|____		FRONT
 * 			  /			     \
 * 			 |	   1     2	  |
 * _____|____|	   1	 2	  |_____|____ 	MIDDLE
 * 			 |	   1	 2	  |
 *  		  \			     /
 *   _____|____\			/_____|____		BACK
 *    			\__________/
 *
 *		1 : PCA9685 left module
 *		2 : PCA9685 right module
 *
 * 					  PAW
 * 					   5
 * 					  / \
 *                   /   \
 * _______________  /     \
 * |			  |/       \
 * |	  3		  /         \
 * |	  3		4 |          \
 * |______3_______|
 * 		  3
 *
 *		3 : coxa servo
 *		4 : femur servo
 *		5 : tibia servo
 */

/*
 * Servo calibration : value of PCA9685 module to have paws on calibration position
 * _______________
 * |			  |
 * |	  3		  |
 * |	  3		4 ______________5
 * |______3_______|				|
 * 		  3						|
 * 		  						|
 * 		  						|
 * 		  						|
 */
static const int offset_table[2][3][3] =
{
		{// left
				{390, 280, 260}, // front
				{395, 345, 240}, // middle
				{340, 320, 350}  // back
		},
		{// right
				{380, 370, 380}, // front
				{290, 340, 355}, // middle
				{315, 345, 305}  // back
		}     // tib  fem  cox
};

/*
 * Table of active sequence of each paw : first sequence number is 0. More you have sequence, slower the hexapode will be
 */
constexpr int sequence_of_paws[2][3] =
{
		{0		, 1		,2		}, //right
		{2		, 1		,0		}  //left
	   //front	, middle, back
};

/*
 * PCA9685 module addresse : represent the addresse that you weld on the module for each side
 */
constexpr char PCA9685_LEFT		 	= 0x02;
constexpr char PCA9685_RIGHT		= 0x01;

/*
 * PS4 Controller
 */
constexpr auto ps4_controller_mac_addr = "1C:96:5A:D2:D2:74";

/*
 * Represent the center of rotation of paw for coxa servo
 */
constexpr float DEFAULT_X_CENTER_FRONT  =  0.f;
constexpr float DEFAULT_X_CENTER_MIDDLE = -20.f;
constexpr float DEFAULT_X_CENTER_BACK   = -44.1f;

/*
 * Default characteristic when you switch on the hexapode
 */
constexpr float DEFAULT_PAW_SPREADING =  80.f;
constexpr float DEFAULT_HEIGHT		  = -50.f;

/*
 * Hexapode dimension
 */
constexpr float HALF_WIDTH_MIN 			= 65.0f;
constexpr float HALF_WIDTH_MAX 			= 75.0f;
constexpr float HALF_LENGTH 			= 110.0f;

constexpr float TIBIA_LENGTH 			= 100.0f;
constexpr float FEMUR_LENGTH 			= 70.0f;
constexpr float TIBIA_ORIGIN_OFFSET 	= 44.1f;

constexpr float CENTER_TO_GROUND_OFFSET = 40.0f;


#endif /* HEXAPODE_DESCRIPTION_H_ */
