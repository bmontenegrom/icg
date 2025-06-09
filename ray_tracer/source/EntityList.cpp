/**
 * @file EntityList.cpp
 * @brief Implementación de la clase EntityList para el sistema de ray tracing
 * 
 * Este archivo implementa la gestión de múltiples entidades geométricas:
 * - Almacenamiento y gestión de objetos en la escena
 * - Cálculo de intersecciones con todos los objetos
 * - Búsqueda de la intersección más cercana
 * - Aplicación de materiales a múltiples objetos
 * 
 * La clase EntityList actúa como un contenedor de entidades que:
 * - Mantiene una lista de punteros compartidos a entidades
 * - Permite agregar y eliminar entidades dinámicamente
 * - Maneja la detección de colisiones con múltiples objetos
 * - Facilita la aplicación de materiales a grupos de objetos
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "EntityList.h"
#include "Material.h"

/**
 * @brief Agrega una entidad a la lista
 * 
 * Almacena un puntero compartido a una entidad en el vector de entidades.
 * El uso de punteros compartidos garantiza una gestión segura de la memoria.
 * 
 * @param entity Puntero compartido a la entidad a agregar
 */
void EntityList::addEntity(std::shared_ptr<Entity> entity) {
	entities.push_back(entity);
}

/**
 * @brief Limpia la lista de entidades
 * 
 * Elimina todas las entidades de la lista.
 * Los punteros compartidos se encargan de liberar la memoria automáticamente
 * cuando ya no hay referencias a las entidades.
 */
void EntityList::clear() {
	entities.clear();
}

/**
 * @brief Verifica si un rayo intersecta con alguna entidad de la lista
 * 
 * Itera sobre todas las entidades buscando intersecciones con el rayo.
 * Mantiene un registro de la intersección más cercana encontrada.
 * Actualiza el intervalo de búsqueda para optimizar el proceso.
 * 
 * @param ray El rayo a verificar
 * @param ray_t Intervalo de parámetros del rayo
 * @param rec Registro de intersección que se actualiza con la información del hit más cercano
 * @return true si hay intersección con alguna entidad, false en caso contrario
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

/**
 * @brief Asigna un material a todas las entidades en la lista
 * 
 * Itera sobre todas las entidades y les asigna el mismo material.
 * Útil para aplicar propiedades de material uniformes a grupos de objetos.
 * 
 * @param material Puntero compartido al material a asignar
 */
void EntityList::setMaterial(std::shared_ptr<Material> material) {
	for (auto& entity : entities) {
		entity->setMaterial(material);
	}
}

std::vector<std::shared_ptr<Entity>> EntityList::getEntities() const
{
	return entities;
}
