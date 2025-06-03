#include "Ray.h"


Ray::Ray() : origin(Vec3()), direction(Vec3()) {}

Ray::Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

const Vec3& Ray::getOrigin() const {
	return origin;
}

const Vec3& Ray::getDirection() const {
	return direction;
}

Vec3 Ray::pointAtParameter(double t) const {
	return origin + t * direction;
}

