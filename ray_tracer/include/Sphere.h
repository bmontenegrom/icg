/**
 * @file Sphere.h
 * @brief Clase que representa una esfera en el sistema de ray tracing
 * 
 * Esta clase implementa la geometría de una esfera y proporciona métodos para
 * calcular intersecciones de rayos con la esfera.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once

#include "Entity.h"
#include "Vec3.h"

class Sphere : public Entity {
public:
	/**
	 * @brief Constructor que inicializa la esfera con un centro y un radio
	 * @param center Centro de la esfera
	 * @param radius Radio de la esfera
	 */
	Sphere(const Vec3& center, double radius);
	
	/**
	 * @brief Destructor por defecto
	 */
	~Sphere() = default;
	
	/**
	 * @brief Verifica si un rayo intersecta con la esfera
	 * @param ray El rayo a verificar
	 * @param ray_t El intervalo de parámetros del rayo
	 * @param rec Registro de intersección
	 * @return true si hay intersección, false en caso contrario
	 */
	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
private:
	Vec3 center;  ///< Centro de la esfera
	double radius; ///< Radio de la esfera
};