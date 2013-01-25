//============================================================================
// Name        : Navigation.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "MathTypes/Vector2.h"
#include "Compute.h"
#include <math.h>
using namespace std;

int main() {
	Vector2 a = Vector2(0,0);
	Vector2 b = Vector2(1,1);
	Compute compute;
	cout << compute.ComputeDistance(a,b)<< endl; // prints !!!Hello World!!!
	cout << atan(9/5)<< endl; // prints !!!Hello World!!!
	return 0;
}
