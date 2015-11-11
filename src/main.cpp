/*
 * main.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#include <iostream>
#include <unistd.h>
#include <ctime>

#include "Hexapode.h"
#include "bcm2835.h"
#include "timer_t.hpp"

using namespace std;

int main()
{

	cout << "Hello World" << endl;

	Hexapode doris;

	util::timer_t timer;

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

	timer.reset();
	doris.toggle();

	doris.move({linear, direction_back, 50, 0, 120});

	while(1)
	{
		if(timer.elapsed().millis() >= 20.0)
		{
			timer.reset();

			if(!doris.update())
				doris.toggle();
				//doris.move({linear, direction_front, 100, 0, 50});

		}
	}

	/*doris.move({linear, direction_front, 50, 0, 50});
	int i = 0;
	while(i < 20)
	{
		i++;
		while(doris.update()) usleep(2000);
		doris.move({linear, direction_front, 100, 0, 50});
	}*/

	cout << "bye\n";

	return 0;
}



