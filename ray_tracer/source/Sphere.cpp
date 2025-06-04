#include "Sphere.h"

Sphere::Sphere(const Vec3& center, double radius) : center(center), radius(radius) {}

//version más eficiente para la esfera
bool Sphere::hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const {
	Vec3 oc = center - ray.getOrigin();
	double a = ray.getDirection().lengthSquared();
	double h = dotProduct(ray.getDirection(), oc);
	double c = oc.lengthSquared() - radius * radius;
	double discriminant = h * h - a * c;
	if (discriminant < 0) {
		return false; // No intersection
	}

	double sqrtDiscriminant = std::sqrt(discriminant);

	double root = (h - sqrtDiscriminant) / a;
	if (root <= tMin || tMax <= root) {
		root = (h + sqrtDiscriminant) / a;
		if (root <= tMin || tMax <= root) {
			return false; // No valid intersection
		}
	}

	rec.t = root;
	rec.point = ray.pointAtParameter(rec.t);
	Vec3 normal = (rec.point - center) / radius; // Normal at the intersection point
	rec.setFaceNormal(ray, normal); //tambien setea el valor de la normal
	
	
	return true;

}