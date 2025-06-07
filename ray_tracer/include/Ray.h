#pragma once
#include "Vec3.h"

/**
 * @brief Representa un rayo en el espacio 3D para ray tracing
 * 
 * Un rayo se define matemáticamente como P(t) = origin + t * direction,
 * donde t es un parámetro escalar. Esta clase encapsula la representación
 * de rayos utilizados en el algoritmo de ray tracing.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */
class Ray {
public:
	/**
	 * @brief Constructor por defecto
	 */
	Ray();
	
	/**
	 * @brief Constructor parametrizado
	 * @param origin Punto de origen del rayo
	 * @param direction Vector de dirección del rayo (no necesita estar normalizado)
	 */
	Ray(const Vec3& origin, const Vec3& direction);

	/**
	 * @brief Destructor por defecto
	 */
	~Ray() = default;

	/**
	 * @brief Obtiene el punto de origen del rayo
	 * @return Referencia constante al punto de origen
	 */
	const Vec3& getOrigin() const;
	
	/**
	 * @brief Obtiene el vector de dirección del rayo
	 * @return Referencia constante al vector de dirección
	 */
	const Vec3& getDirection() const;
	
	/**
	 * @brief Calcula un punto a lo largo del rayo usando el parámetro t
	 * @param t Parámetro escalar para la ecuación del rayo P(t) = origin + t * direction
	 * @return Punto calculado en el espacio 3D
	 */
	Vec3 pointAtParameter(double t) const;

private:
	Vec3 origin;      ///< Punto de origen del rayo
	Vec3 direction;   ///< Vector de dirección del rayo
};