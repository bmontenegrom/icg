/**
 * @file Ray.cpp
 * @brief Implementación de la clase Ray para el sistema de ray tracing
 * 
 * Esta clase representa un rayo en el espacio 3D y proporciona métodos para
 * calcular puntos a lo largo del rayo.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Ray.h"

/**
 * @brief Constructor por defecto
 */
Ray::Ray() : origin(Vec3()), direction(Vec3()) {}

/**
 * @brief Constructor que inicializa el rayo con un origen y una dirección
 * @param origin Punto de origen del rayo
 * @param direction Vector de dirección del rayo
 */
Ray::Ray(const Vec3& origin, const Vec3& direction) : origin(origin), direction(direction) {}

/**
 * @brief Obtiene el punto de origen del rayo
 * @return Referencia constante al punto de origen
 */
const Vec3& Ray::getOrigin() const {
	return origin;
}

/**
 * @brief Obtiene el vector de dirección del rayo
 * @return Referencia constante al vector de dirección
 */
const Vec3& Ray::getDirection() const {
	return direction;
}

/**
 * @brief Calcula un punto a lo largo del rayo usando el parámetro t
 * @param t Parámetro escalar para la ecuación del rayo P(t) = origin + t * direction
 * @return Punto calculado en el espacio 3D
 */
Vec3 Ray::pointAtParameter(double t) const {
	return origin + t * direction;
}

