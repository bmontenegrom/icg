#pragma once
#include <iostream>
class Vec3 {
public:
	Vec3();
	Vec3(double x, double y, double z);
	~Vec3() = default;
	double getX() const;
	double getY() const;
	double getZ() const;
	void setX(double x);
	void setY(double y);
	void setZ(double z);

	Vec3 operator-() const;
	Vec3& operator+=(const Vec3& other);
	Vec3& operator*=(double t);
	Vec3& operator/=(double t);

	double length() const;
	double lengthSquared() const;

private:
	double x, y, z;


};


std::ostream& operator<<(std::ostream& os, const Vec3& vec);

Vec3 operator+(const Vec3& a, const Vec3& b);
Vec3 operator-(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, const Vec3& b);
Vec3 operator*(const Vec3& a, double t);
Vec3 operator*(double t, const Vec3& a);
Vec3 operator/(const Vec3& a, double t);

double dotProduct(const Vec3& a, const Vec3& b);
Vec3 crossProduct(const Vec3& a, const Vec3& b);

Vec3 unitVector(const Vec3& v);