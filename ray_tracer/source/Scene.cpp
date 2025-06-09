/**
 * @file Scene.cpp
 * @brief Implementación de la clase Scene para el sistema de ray tracing
 * 
 * Este archivo implementa la gestión de la escena, incluyendo:
 * - Manejo de objetos geométricos y luces
 * - Cálculo de intersecciones con todos los objetos
 * - Verificación de sombras
 * - Coordinación entre objetos y materiales
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "Scene.h"
#include "Interval.h"
#include "EntityList.h"
#include "Material.h"
#include "MaterialGlass.h"
#include <algorithm>
#include <LambertianMaterial.h>

/**
 * @brief Constructor de la escena
 * @param world_objects Puntero compartido a la entidad que representa el mundo
 */
Scene::Scene(std::shared_ptr<Entity> world_objects) : world(world_objects) {
}

/**
 * @brief Agrega una luz a la escena
 * @param light Puntero compartido a la luz a agregar
 */
void Scene::addLight(std::shared_ptr<Light> light) {
    lights.push_back(light);
}

/**
 * @brief Verifica si un punto está en sombra respecto a una luz
 * 
 * Lanza un rayo desde el punto hacia la luz y verifica si hay
 * intersecciones con objetos en el camino.
 * 
 * @param shadow_ray Rayo desde el punto hacia la luz
 * @param light_position Posición de la luz
 * @return true si el punto está en sombra, false en caso contrario
 */
bool Scene::is_in_shadow(const Ray& shadow_ray, const Vec3& light_position) const {  
    // Calcular la distancia a la luz
    double distance_to_light = (light_position - shadow_ray.getOrigin()).length();
    
    // Verificar si hay algún objeto entre el punto y la luz
    HitRecord temp_rec;
    
    // Usar un intervalo que va desde un pequeño epsilon hasta la distancia a la luz
    Interval shadow_interval(0.001, distance_to_light - 0.001);
    
    return world->hit(shadow_ray, shadow_interval, temp_rec);
}

/**
 * @brief Verifica si un rayo intersecta con algún objeto de la escena
 * 
 * Delega la verificación de intersección al objeto world que contiene
 * todas las entidades de la escena.
 * 
 * @param ray Rayo a verificar
 * @param ray_t Intervalo válido para el parámetro t del rayo
 * @param rec Registro donde se almacena la información de la intersección
 * @return true si hay intersección, false en caso contrario
 */
bool Scene::hit(const Ray& ray, const Interval& ray_t, HitRecord& rec) const {
    return world->hit(ray, ray_t, rec);
}

Color Scene::transmissionAlong(const Ray& shadow_ray, double distance) const
{
    Color transmission(1.0, 1.0, 1.0);
    HitRecord rec;
    std::shared_ptr<EntityList> entities = std::dynamic_pointer_cast<EntityList>(world);
 
 
    
    
    if (entities) {
        
        for (const auto& entity : entities->getEntities()) {
			
            if (entity->hit(shadow_ray, Interval(0.001, distance + 0.001), rec)) {
			
                if (rec.material_ptr->transparency() == 0.0) {
					
                    return Color(0.0, 0.0, 0.0);
                }

                //es un vidrio!!
				auto glass = std::dynamic_pointer_cast<MaterialGlass >(rec.material_ptr);
                if (glass) {
					
					transmission *= glass->albedo * glass->transparency();
                }
            }

        }
   
    }

    return transmission;
}


