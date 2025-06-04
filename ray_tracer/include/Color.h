#pragma once
#include <iostream>

class Color {
public:
	Color();
	Color(double r, double g, double b);
	~Color() = default;
	double getR() const;
	double getG() const;
	double getB() const;
	void setR(double r);
	void setG(double g);
	void setB(double b);

	int getRbyte() const;
	int getGbyte() const;
	int getBbyte() const;


	Color& operator+=(const Color& other);

private:
	double r, g, b;
};

std::ostream& operator<<(std::ostream& os, const Color& color);

Color operator+(const Color& a, const Color& b);
Color operator-(const Color& a, const Color& b);
Color operator*(const Color& a, double t);
Color operator*(double t, const Color& a);
