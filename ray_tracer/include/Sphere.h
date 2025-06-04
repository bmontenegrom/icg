#pragma once

#include "Entity.h"
#include "Vec3.h"

class Sphere : public Entity {
public:
	Sphere(const Vec3& center, double radius);
	~Sphere() = default;
	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
private:
	Vec3 center;  // Center of the sphere
	double radius; // Radius of the sphere
};