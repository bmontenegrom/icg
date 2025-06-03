#pragma once
#include "Vec3.h"

class Ray {
public:
	Ray();
	Ray(const Vec3& origin, const Vec3& direction);

	~Ray() = default;

	const Vec3& getOrigin() const;
	const Vec3& getDirection() const;
	Vec3 pointAtParameter(double t) const;

private:
	Vec3 origin;  
	Vec3 direction; 
};