#pragma once


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

private:
	double r, g, b;
};