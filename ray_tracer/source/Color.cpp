#include "Color.h"

Color::Color() : r(0), g(0), b(0) {}

Color::Color(double r, double g, double b) : r(r), g(g), b(b) {}

double Color::getR() const {
	return r;
}

double Color::getG() const {
	return g;
}

double Color::getB() const {
	return b;
}

void Color::setR(double r) {
	this->r = r;
}

void Color::setG(double g) {
	this->g = g;
}

void Color::setB(double b) {
	this->b = b;
}

int Color::getRbyte() const {
	return static_cast<int>(255.999 * r);
}

int Color::getGbyte() const {
	return static_cast<int>(255.999 * g);
}

int Color::getBbyte() const {
	return static_cast<int>(255.999 * b);
}

