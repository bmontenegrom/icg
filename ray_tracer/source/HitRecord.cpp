/**
 * @file HitRecord.cpp
 * @brief Implementación de la clase HitRecord para el sistema de ray tracing
 * 
 * Esta clase almacena información sobre la intersección de un rayo con una superficie,
 * incluyendo el punto de intersección, la normal y otros datos relevantes.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "HitRecord.h"

/**
 * @brief Constructor que inicializa un registro de intersección
 * @param p Punto de intersección
 * @param n Vector normal en el punto de intersección
 * @param t_val Parámetro t del rayo en el punto de intersección
 */
HitRecord::HitRecord(const Vec3& p, const Vec3& n, double t_val): point(p), normal(n), t(t_val), frontFace(true) {}

/**
 * @brief Configura la normal de la cara según la dirección del rayo
 * 
 * Este método determina si el rayo golpea la cara frontal o trasera de la superficie
 * y ajusta la normal en consecuencia. La normal siempre apunta contra la dirección
 * del rayo incidente.
 * 
 * @param ray Rayo que intersecta la superficie
 * @param outwardNormal Normal exterior de la superficie en el punto de intersección
 */
void HitRecord::setFaceNormal(const Ray& ray, const Vec3& outwardNormal) {
	frontFace = dotProduct(ray.getDirection(), outwardNormal) < 0;
	if (!frontFace) {
		normal = -outwardNormal; // Flip the normal if the ray hits the back face
	}
	else {
		normal = outwardNormal; // Use the outward normal for front face
	}
}