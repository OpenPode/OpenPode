/*
 * config.h
 *
 *  Created on: 28 nov. 2015
 *      Author: Julien
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

//#define CALIBRATION
#define ERROR_ACTION
#define HEAD

/**********************************************************************************
 * DS4 Controller
 **********************************************************************************/

// Driver
constexpr auto ds4_driver_launch_command = "ds4drv";
constexpr auto bluetooth_scan_command = "hcitool -i hci0 con";
constexpr auto pid_filename = "/tmp/ds4drv.pid";

// Controller
constexpr auto ps4_controller_mac_addr = "1C:96:5A:D2:D2:74";

//default position
constexpr float DEFAULT_PAW_SPREADING =  80.f;
constexpr float DEFAULT_HEIGHT		  = -50.f;

//paw position
constexpr float DEFAULT_Y 	= DEFAULT_PAW_SPREADING;
constexpr float DEFAULT_Z 	= DEFAULT_HEIGHT;

constexpr float DEFAULT_X_CENTER_FRONT  =  0.f;
constexpr float DEFAULT_X_CENTER_MIDDLE = -20.f;
constexpr float DEFAULT_X_CENTER_BACK   = -44.1f;

//movement
constexpr float MAX_HEIGHT_GET_UP = -30.f;
constexpr float MAX_STEP_NUMBER   = 140.f;
constexpr float MIN_STEP_NUMBER   = 12.f;

constexpr float DEFAULT_DISTANCE = 40.f;

constexpr float SPREADING_STEP = 1.f;
constexpr float HEIGHT_STEP    = 0.5f;

constexpr float NO_MOVEMENT_STEP_DIST = 4.f;

//utilities
constexpr bool SEQUENCE_FINISH 		= true;
constexpr bool SEQUENCE_IN_PROGRESS = false;

//PCA9685 addr
constexpr char PCA9685_BASE_ADDR 	= 0x40;
constexpr char PCA9685_LEFT		 	= 0x02;
constexpr char PCA9685_RIGHT		= 0x01;
constexpr char PCA9685_LEFT_ADDR 	= PCA9685_BASE_ADDR | PCA9685_LEFT;
constexpr char PCA9685_RIGHT_ADDR 	= PCA9685_BASE_ADDR | PCA9685_RIGHT;

#endif /* CONFIG_H_ */
