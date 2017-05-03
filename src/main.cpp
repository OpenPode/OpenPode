/*
 * main.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ctime>
#include <signal.h>
#include <string>

#include "hexapode/Hexapode.h"
#include "drivers/broadcom/bcm2835.h"
#include "utility/unix/exec.h"
#include "config.h"

using namespace std;

void block_until_ds4_controller_connexion();

void launch_ds4drv();

int main()
{
	cout << "Hello Blues Brothers" << endl;

#ifndef CALIBRATION
	//launch_ds4drv();

	//block_until_ds4_controller_connexion();
#endif

	cout << "Doris is now running" << endl;

	/*PS4Controller ctrl;

	std::cout << std::endl << "Executing \"ps -a\"" << std::endl;

	while(1)
	{
		ctrl.process_input();
		std::cout << std::boolalpha;
		//std::cout << "circle : " << ctrl.is_key_press(PS4_Key::KEY_CIRCLE) << std::endl;
	}*/

	Hexapode doris;

#ifndef CALIBRATION
	doris.run();
#else
	//while(1)
	//{
		doris.calibrate_servomotors(-44.1f, 70.f, -100.f);
	//}
#endif

	/*********************************************************/

	/*clock_t begin = clock();

	paw1.move(50,120,-50);

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;*/

	/*********************************************************/

	/*Angles angles = doris.m_left_side.m_front_paw.move(-44.1, 70, -100);
	int time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + 345.);
	int time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + 280.);
	int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 360.);
	doris.m_left_side.m_module.set_off_time(channel6, time3);
	doris.m_left_side.m_module.set_off_time(channel7, time2);
	doris.m_left_side.m_module.set_off_time(channel8, time1);
	cout << dec << angles.theta1*(180./3.14159) << endl;
	cout << angles.theta2*(180./3.14159) << endl;
	cout << angles.theta3*(180./3.14159) << endl;
	cout << time1 << endl;
	cout << time2 << endl;
	cout << time3 << endl;*/

	/*Angles angles = doris.m_right_side.m_front_paw.move(-44.1, -70, -100);
	int time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 441.);
	int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
	int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + 355.);
	doris.m_right_side.m_module.set_off_time(channel3, time3);
	doris.m_right_side.m_module.set_off_time(channel4, time2);
	doris.m_right_side.m_module.set_off_time(channel5, time1);
	cout << dec << angles.theta1*(180./3.14159) << endl;
	//cout << angles.theta2*(180./3.14159) << endl;
	//cout << angles.theta3*(180./3.14159) << endl;
	cout << time1 << endl;
	//cout << time2 << endl;
	//cout << time3 << endl;*/

	/*********************************************************/

	/*int i=0;

	while(i<20)
	{

		for(int i=-100 ; i<100 ; i+=2)
		{
			Angles angles = doris.m_right_side.m_front_paw.move(i, -70, -70);
			int time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 280.);
			int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + 365.);
			doris.m_right_side.m_module.set_off_time(channel3, time3);
			doris.m_right_side.m_module.set_off_time(channel4, time2);
			doris.m_right_side.m_module.set_off_time(channel5, time1);
			cout << dec << angles.theta1*(180./3.14159) << endl;
			//cout << angles.theta2*(180./3.14159) << endl;
			//cout << angles.theta3*(180./3.14159) << endl;
			cout << time1 << endl;
			usleep(5000);
		}
		for(int i=100 ; i>-100 ; i-=2)
		{
			Angles angles = doris.m_right_side.m_front_paw.move(i, -70, -70);
			int time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 280.);
			int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + 365.);
			doris.m_right_side.m_module.set_off_time(channel3, time3);
			doris.m_right_side.m_module.set_off_time(channel4, time2);
			doris.m_right_side.m_module.set_off_time(channel5, time1);
			cout << dec << angles.theta1*(180./3.14159) << endl;
			//cout << angles.theta2*(180./3.14159) << endl;
			//cout << angles.theta3*(180./3.14159) << endl;
			cout << time1 << endl;
			usleep(5000);
		}
		i++;
	}*/

	/*********************************************************/

	/*doris.move({linear, direction_, 50, 0, 50});
	int i = 0;
	while(i < 20)
	{
		i++;
		while(doris.update()) usleep(2000);
		doris.move({linear, direction_front, 100, 0, 50});
	}*/

	return 0;
}

void block_until_ds4_controller_connexion()
{
	string mac_address = "";

	cout << "Wait for DS4 controller " << DS4_MAC_ADDR << " connexion"  << endl;

	while(mac_address != DS4_MAC_ADDR)
	{
		try
		{
			mac_address = exec(bluetooth_scan_command).substr(20,17);
		}
		catch(const std::exception &e)
		{
			mac_address = "";
		}
		sleep(2);
	}

	cout << "The DS4 controller " << DS4_MAC_ADDR << " is connected"  << endl;
}

void launch_ds4drv()
{
	fstream pid_file(pid_filename);

	cout << "Check if ds4drv is launched" << endl;

	if(not pid_file.is_open())
	{
		cout << "Launch ds4drv" << endl;
		system(ds4_driver_launch_command);
		sleep(2);
	}
	else
	{
		cout << "Ds4drv already launched" << endl;
	}

	pid_file.close();
}



