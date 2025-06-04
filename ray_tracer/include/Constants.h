#pragma once
#include <limits>
#include <cstdlib>



const double PI = 3.14159265358979323846;
const int WIDTH = 800;
const int HEIGHT = 600;

const double infinity = std::numeric_limits<double>::infinity();


//utility functions

inline double degToRad(double degrees) {
	return degrees * (PI / 180.0);
}

inline double random_double() {
	return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	return min + (max - min) * random_double();
}