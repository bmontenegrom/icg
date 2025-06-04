#pragma once
#include "Constants.h"

class Interval {
public:
	
	Interval(double min_val, double max_val);
	
	Interval();
	
	double getMin() const;
	double getMax() const;
	
	void setMin(double min_val);
	void setMax(double max_val);	

	double size() const;
	bool contains(double value) const;
	bool surrounds(double value) const;

	
private:
	double min;
	double max;

};

