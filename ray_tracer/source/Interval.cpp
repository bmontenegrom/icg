#include "Interval.h"

Interval::Interval(double min_val, double max_val) : min(min_val), max(max_val) {}

Interval::Interval() : min(infinity), max(-infinity) {}

double Interval::getMin() const {
	return min;
}

double Interval::getMax() const {
	return max;
}

void Interval::setMin(double min_val) {
	min = min_val;
}

void Interval::setMax(double max_val) {
	max = max_val;
}

double Interval::size() const {
	return max - min;
}

bool Interval::contains(double value) const {
	return value >= min && value <= max;
}

bool Interval::surrounds(double value) const {
	return value > min && value < max;
}
