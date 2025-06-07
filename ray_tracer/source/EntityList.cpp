/**
 * @file EntityList.cpp
 * @brief Implementación de la clase EntityList para el sistema de ray tracing
 * 
 * Esta clase representa una lista de entidades en la escena y proporciona métodos
 * para gestionar y realizar intersecciones con ellas.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "EntityList.h"

/**
 * @brief Agrega una entidad a la lista
 * @param entity Puntero compartido a la entidad a agregar
 */
void EntityList::addEntity(std::shared_ptr<Entity> entity) {
	entities.push_back(entity);
}

/**
 * @brief Limpia la lista de entidades
 */
void EntityList::clear() {
	entities.clear();
}

/**
 * @brief Verifica si un rayo intersecta con alguna entidad de la lista
 * @param ray El rayo a verificar
 * @param ray_t Intervalo de parámetros del rayo
 * @param rec Registro de intersección
 * @return true si hay intersección, false en caso contrario
 */
bool EntityList::hit(const Ray& ray, Interval ray_t, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = ray_t.getMax();
	for (const auto& entity : entities) {
		if (entity->hit(ray, Interval(ray_t.getMin(), closestSoFar), tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec; // Update the record with the closest hit
		}
	}
	return hitAnything;
}