#include "Vec3.h"

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(double x, double y, double z) : x(x), y(y), z(z){}

double Vec3::getX() const
{
	return this->x;
}

double Vec3::getY() const
{
	return this->y;
}

double Vec3::getZ() const
{
	return this->z;
}

void Vec3::setX(double x)
{
	this->x = x;
}

void Vec3::setY(double y)
{
	this->y = y;
}

void Vec3::setZ(double z)
{
	this->z = z;
}


Vec3 Vec3::operator-() const
{
	return Vec3(-x, -y, -z);
}

Vec3& Vec3::operator+=(const Vec3& other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3& Vec3::operator*=(double t)
{
	this->x *= t;
	this->y *= t;
	this->z *= t;
	return *this;
}

Vec3& Vec3::operator/=(double t)
{
	this->x /= t;
	this->y /= t;
	this->z /= t;
	return *this;
}

double Vec3::length() const
{
	return std::sqrt(lengthSquared());
}

double Vec3::lengthSquared() const
{
	return x * x + y * y + z * z;
}

std::ostream& operator<<(std::ostream& os, const Vec3& vec)
{
	os << "Vec3(" << vec.getX() << ", " << vec.getY() << ", " << vec.getZ() << ")";
	return os;
}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

Vec3 operator-(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}

Vec3 operator*(const Vec3& a, const Vec3& b)
{
	return Vec3(a.getX() * b.getX(), a.getY() * b.getY(), a.getZ() * b.getZ());
}

Vec3 operator*(const Vec3& a, double t)
{
	return Vec3(a.getX() * t, a.getY() * t, a.getZ() * t);
}

Vec3 operator*(double t, const Vec3& a)
{
	return Vec3(a.getX() * t, a.getY() * t, a.getZ() * t);
}

Vec3 operator/(const Vec3& a, double t)
{
	return Vec3(a.getX() / t, a.getY() / t, a.getZ() / t);
}

double dotProduct(const Vec3& a, const Vec3& b)
{
	return a.getX() * b.getX() + a.getY() * b.getY() + a.getZ() * b.getZ();
}

Vec3 crossProduct(const Vec3& a, const Vec3& b)
{
	return Vec3(
		a.getY() * b.getZ() - a.getZ() * b.getY(),
		a.getZ() * b.getX() - a.getX() * b.getZ(),
		a.getX() * b.getY() - a.getY() * b.getX()
	);
}