#pragma once
#include "Entity.h"
#include "Light.h"
#include "Ray.h"
#include "Vec3.h"
#include <vector>
#include <memory>

/**
 * @brief Clase que representa una escena con objetos y luces
 * 
 * Maneja la colección de entidades geométricas y fuentes de luz,
 * proporcionando funcionalidad para pruebas de sombras y renderizado.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class Scene {
public:
    std::vector<std::shared_ptr<Light>> lights;  ///< Fuentes de luz en la escena
    std::shared_ptr<Entity> world;               ///< Objetos geométricos en la escena
    
    /**
     * @brief Constructor
     * @param world_objects Objetos geométricos de la escena
     */
    Scene(std::shared_ptr<Entity> world_objects);
    
    /**
     * @brief Agrega una luz a la escena
     * @param light Puntero a la fuente de luz
     */
    void addLight(std::shared_ptr<Light> light);
    
    /**
     * @brief Verifica si un rayo está en sombra respecto a una posición de luz
     * @param shadow_ray Rayo de sombra desde el punto hacia la luz  
     * @param light_position Posición de la fuente de luz
     * @return true si el punto está en sombra
     */
    bool is_in_shadow(const Ray& shadow_ray, const Vec3& light_position) const;
    
    /**
     * @brief Verifica intersección con los objetos de la escena
     * @param ray Rayo a verificar
     * @param ray_t Intervalo de parámetros
     * @param rec Registro de intersección
     * @return true si hay intersección
     */
    bool hit(const Ray& ray, const Interval& ray_t, HitRecord& rec) const;
}; 