#pragma once
#include "Vec3.h"
#include "Ray.h"

class HitRecord {
public:
	Vec3 point;        // Point of intersection
	Vec3 normal;      // Normal at the intersection point
	double t;         // Parameter t for the ray equation
	bool frontFace;   // True if the ray hits the front face of the surface
	// Constructor
	HitRecord(const Vec3& p, const Vec3& n, double t_val);
	// Default constructor
	HitRecord() : point(Vec3()), normal(Vec3()), t(0), frontFace(true) {}

	void setFaceNormal(const Ray& ray, const Vec3& outwardNormal);
};