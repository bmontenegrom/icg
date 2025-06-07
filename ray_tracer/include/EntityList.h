/**
 * @file EntityList.h
 * @brief Clase que representa una lista de entidades en el sistema de ray tracing
 * 
 * Esta clase permite gestionar múltiples entidades geométricas, como esferas, planos, etc.,
 * y realizar operaciones de intersección de rayos con todas ellas.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once

#include <memory>
#include <vector>
#include "Entity.h"

class EntityList : public Entity {
public:
	/**
	 * @brief Constructor
	 */
	EntityList() = default;
	/**
	 * @brief Destructor
	 */
	~EntityList() override = default;
	
	/**
	 * @brief Agrega una entidad a la lista
	 * @param entity Puntero compartido a la entidad a agregar
	 */
	void addEntity(std::shared_ptr<Entity> entity);
	
	/**
	 * @brief Limpia la lista de entidades
	 */
	void clear();

	/**
	 * @brief Verifica si un rayo intersecta con alguna entidad en la lista
	 * @param ray El rayo a verificar
	 * @param ray_t El intervalo de parámetros del rayo
	 * @param rec Registro de intersección
	 * @return true si hay intersección, false en caso contrario
	 */
	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Entity>> entities; ///< Lista de entidades
};