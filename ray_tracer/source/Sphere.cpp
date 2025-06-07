/**
 * @file Sphere.cpp
 * @brief Implementación de la clase Sphere para el sistema de ray tracing
 * 
 * Esta clase implementa la geometría de una esfera y proporciona métodos para
 * calcular intersecciones de rayos con la esfera.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Sphere.h"

/**
 * @brief Constructor que inicializa la esfera con un centro y un radio
 * @param center Centro de la esfera
 * @param radius Radio de la esfera
 */
Sphere::Sphere(const Vec3& center, double radius) : center(center), radius(radius) {}

//version ms eficiente para la esfera
/**
 * @brief Verifica si un rayo intersecta con la esfera
 * @param ray El rayo a verificar
 * @param ray_t El intervalo de parámetros del rayo
 * @param rec Registro de intersección
 * @return true si hay intersección, false en caso contrario
 */
bool Sphere::hit(const Ray& ray, Interval ray_t, HitRecord& rec) const {
	Vec3 oc = center - ray.getOrigin();
	double a = ray.getDirection().lengthSquared();
	double h = dotProduct(ray.getDirection(), oc);
	double c = oc.lengthSquared() - radius * radius;
	double discriminant = h * h - a * c;
	if (discriminant < 0) {
		return false; // No hay intersección
	}

	double sqrtDiscriminant = std::sqrt(discriminant);

	double root = (h - sqrtDiscriminant) / a;
	if (!ray_t.surrounds(root)) {
		root = (h + sqrtDiscriminant) / a;
		if (!ray_t.surrounds(root)) {
			return false; // No hay intersección
		}
	}

	rec.t = root;
	rec.point = ray.pointAtParameter(rec.t);
	Vec3 normal = (rec.point - center) / radius; // Normal en el punto de intersección
	rec.setFaceNormal(ray, normal); //tambien setea el valor de la normal	
	
	
	return true;

}