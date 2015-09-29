//============================================================================
// Name        : Hexapode.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <unistd.h>
#include <ctime>

#include "Paw.h"

using namespace std;

int main()
{

	cout << "Hello World" << endl;

	Paw paw1(Side::left);

	clock_t begin = clock();

	paw1.move(10.1,25.6,3.2);

	clock_t end = clock();

	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

	cout << elapsed_secs << endl;

	return 0;
}
