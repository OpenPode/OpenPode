/*
 * main.cpp
 *
 *  Created on: 28 oct. 2015
 *      Author: Julien
 */

#include <iostream>
#include <unistd.h>
#include <ctime>

#include "Paw.h"
#include "bcm2835.h"

using namespace std;

int main()
{

	//cout << "Hello World" << endl;

	//Paw paw1(Side_t::right);
	/*clock_t begin = clock();

	paw1.move(50,120,-50);

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;*/

	/*bcm2835_gpio_clr(7);

	paw1.m_module.set_prescale(121);

	paw1.m_module.set_on_time_all_channel(0);*/

	/*paw1.m_module.set_off_time(channel2, 410);
	paw1.m_module.set_off_time(channel7, 200);
	paw1.m_module.set_off_time(channel6, 441-205);*/


	/*Angles angles = paw1.move(44.1,70, -100);
	int time3 = static_cast<int>((angles.theta3*(180./3.14159)+90)*1.97 + 280.);
	int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
	int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 380.);
	paw1.m_module.set_off_time(channel6, time3);
	paw1.m_module.set_off_time(channel7, time2);
	paw1.m_module.set_off_time(channel8, time1);
	cout << dec << angles.theta1*(180./3.14159) << endl;
	cout << angles.theta2*(180./3.14159) << endl;
	cout << angles.theta3*(180./3.14159) << endl;
	cout << time1 << endl;
	cout << time2 << endl;
	cout << time3 << endl;*/

	/*int i=0;

	while(i<5)
	{

		for(int i=-150 ; i<150 ; i+=300)
		{
			Angles angles = paw1.move(i, 100, -50);
			int time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 280.);
			int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 380.);
			//paw1.m_module.set_off_time(channel0, time3);
			//paw1.m_module.set_off_time(channel4, time2);
			paw1.m_module.set_off_time(channel2, time1);
			cout << time1 << endl;
			cout << time2 << endl;
			cout << time3 << endl;
			usleep(200000);
		}
		for(int i=150 ; i>-150 ; i-=300)
		{
			Angles angles = paw1.move(i, 100, -50);
			int time3 = static_cast<int>( (angles.theta3*(180./3.14159)+90)*1.97 + 280.);
			int time2 = static_cast<int>( -angles.theta2*(180./3.14159)*1.97     + 340.);
			int time1 = static_cast<int>(-(angles.theta1*(180./3.14159)-90)*1.97 + 380.);
			//paw1.m_module.set_off_time(channel2, time3);
			//paw1.m_module.set_off_time(channel4, time2);
			paw1.m_module.set_off_time(channel2, time1);
			cout << time1 << endl;
			cout << time2 << endl;
			cout << time3 << endl;
			usleep(200000);
		}
		i++;
	}

	cout << "hello\n";*/

	return 0;
}



