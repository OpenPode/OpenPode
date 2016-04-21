/*
 * config.h
 *
 *  Created on: 28 nov. 2015
 *      Author: Julien
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>

/**********************************************************************************
 * DS4 Controller
 **********************************************************************************/

// Driver
const std::string ds4_driver_launch_command = "ds4drv";
const std::string bluetooth_scan_command = "hcitool -i hci0 con";
const std::string pid_filename = "/tmp/ds4drv.pid";

// Controller
const std::string ps4_controller_mac_addr = "1C:96:5A:D2:D2:74";

//paw position
const double DEFAULT_Y 	= 80;
const double DEFAULT_Z 	= -70;

const double DEFAULT_X_CENTER_FRONT = 0;
const double DEFAULT_X_CENTER_MIDDLE = -20;
const double DEFAULT_X_CENTER_BACK = -44.1;

//movement
const double MAX_HEIGHT_GET_UP = -30;
const double MAX_STEP_NUMBER = 140.;
const double MIN_STEP_NUMBER = 12.;

const double DEFAULT_DISTANCE = 40.;

const double SPREADING_STEP = 0.5;
const double HEIGHT_STEP = 0.5;

//default position
const double DEFAULT_PAW_SPREADING = 80;
const double DEFAULT_HEIGHT		   = -50;

#endif /* CONFIG_H_ */
