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


#endif /* CONFIG_H_ */
