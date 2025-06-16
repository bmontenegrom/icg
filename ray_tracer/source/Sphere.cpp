/**
 * @file Sphere.cpp
 * @brief Implementación de la clase Sphere para el sistema de ray tracing
 * 
 * Este archivo implementa la geometría de una esfera y proporciona métodos para
 * calcular intersecciones de rayos con la esfera. La implementación incluye:
 * - Cálculo eficiente de intersecciones rayo-esfera
 * - Manejo de normales y materiales
 * - Soporte para sombras y reflexiones
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

/**
 * @brief Verifica si un rayo intersecta con la esfera usando una versión optimizada
 * 
 * Este método implementa una versión eficiente del cálculo de intersección rayo-esfera:
 * 1. Calcula los coeficientes de la ecuación cuadrática
 * 2. Verifica el discriminante para determinar si hay intersección
 * 3. Encuentra el punto de intersección más cercano dentro del intervalo válido
 * 4. Calcula la normal y actualiza el registro de intersección
 * 
 * La optimización incluye:
 * - Reducción de operaciones matemáticas
 * - Evita calcular la raíz cuadrada innecesariamente
 * - Manejo eficiente de casos límite
 * 
 * @param ray El rayo a verificar
 * @param ray_t El intervalo de parámetros válidos del rayo
 * @param rec Registro que almacena la información de la intersección
 * @return true si hay intersección dentro del intervalo válido, false en caso contrario
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
	rec.material_ptr = material_ptr; // Asignar el material al hit record
	
	// --- Cálculo de coordenadas de textura (u,v) ---
	Vec3 p_local = unitVector(rec.point - center); // Punto en la superficie normalizado

	float u = 0.5f + atan2(p_local.getZ(), p_local.getX()) / (2.0f * PI);
	float v = 0.5f - asin(p_local.getY()) / PI;

	rec.u = u;
	rec.v = v;

	return true;
}

/**
 * @brief Asigna un material a la esfera
 * 
 * Este método permite cambiar el material de la esfera en tiempo de ejecución.
 * El material determina cómo la esfera interactúa con la luz, afectando:
 * - Reflexión de la luz
 * - Color y textura
 * - Propiedades ópticas como transparencia y refracción
 * 
 * @param material Puntero compartido al material a asignar
 */
void Sphere::setMaterial(std::shared_ptr<Material> material) {
	material_ptr = material;
}