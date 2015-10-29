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

using namespace std;

int main()
{

	cout << "Hello World" << endl;

	Hexapode doris;

	/*********************************************************/

	/*clock_t begin = clock();

	paw1.move(50,120,-50);

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;*/

	/*********************************************************/

	/*Angles angles = doris.m_left_side.m_front_paw.move(0, 70, -70);
	int time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + 350.);
	int time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + 295.);
	int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 295.);
	doris.m_left_side.m_module.set_off_time(channel3, time3);
	doris.m_left_side.m_module.set_off_time(channel4, time2);
	doris.m_left_side.m_module.set_off_time(channel5, time1);
	cout << dec << angles.theta1*(180./3.14159) << endl;
	cout << angles.theta2*(180./3.14159) << endl;
	cout << angles.theta3*(180./3.14159) << endl;
	cout << time1 << endl;
	cout << time2 << endl;
	cout << time3 << endl;

	angles = doris.m_right_side.m_front_paw.move(-44.1, -70, -70);
	time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 280.);
	time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
	time1 = static_cast<int>(-(angles.theta1*(180./3.14159)+90)*1.97 + 370.);
	doris.m_right_side.m_module.set_off_time(channel3, time3);
	doris.m_right_side.m_module.set_off_time(channel4, time2);
	doris.m_right_side.m_module.set_off_time(channel5, time1);
	cout << dec << angles.theta1*(180./3.14159) << endl;
	cout << angles.theta2*(180./3.14159) << endl;
	cout << angles.theta3*(180./3.14159) << endl;
	cout << time1 << endl;
	cout << time2 << endl;
	cout << time3 << endl;*/

	/*********************************************************/

	/*int i=0;

	while(i<5)
	{

		for(int i=-100 ; i<100 ; i+=2)
		{
			Angles angles = doris.m_left_side.m_front_paw.move(i, 70, -70);
			int time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + 350.);
			int time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + 295.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 295.);
			doris.m_left_side.m_module.set_off_time(channel3, time3);
			doris.m_left_side.m_module.set_off_time(channel4, time2);
			doris.m_left_side.m_module.set_off_time(channel5, time1);
			usleep(5000);
		}
		for(int i=100 ; i>-100 ; i-=2)
		{
			Angles angles = doris.m_left_side.m_front_paw.move(i, 70, -70);
			int time3 = static_cast<int>( -(angles.theta3*(180./3.14159)+90)*1.97 + 350.);
			int time2 = static_cast<int>( angles.theta2*(180./3.14159)*1.97     + 295.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 295.);
			doris.m_left_side.m_module.set_off_time(channel3, time3);
			doris.m_left_side.m_module.set_off_time(channel4, time2);
			doris.m_left_side.m_module.set_off_time(channel5, time1);
			usleep(5000);
		}
		i++;
	}*/

	/*********************************************************/

	doris.move({linear, direction_front, 60, 0, 50});
	int i = 0;
	while(i < 20)
	{
		i++;
		while(doris.update()) usleep(5000);
		doris.move({linear, direction_front, -120, 0, 50});
		while(doris.update()) usleep(5000);
		doris.move({linear, direction_front, 120, 0, 50});
	}

	cout << "bye\n";

	return 0;
}



