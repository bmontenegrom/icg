#include "HitRecord.h"

HitRecord::HitRecord(const Vec3& p, const Vec3& n, double t_val): point(p), normal(n), t(t_val), frontFace(true) {}

void HitRecord::setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
	frontFace = dotProduct(ray.getDirection(), outwardNormal) < 0;
	if (!frontFace) {
		normal = -outwardNormal; // Flip the normal if the ray hits the back face
	}
	else {
		normal = outwardNormal; // Use the outward normal for front face
	}
}