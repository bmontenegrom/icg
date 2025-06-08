/**
 * @file PointLight.cpp
 * @brief Implementación de la clase PointLight para el sistema de ray tracing
 * 
 * Este archivo implementa una fuente de luz puntual que:
 * - Emite luz en todas direcciones desde un punto específico
 * - Calcula la intensidad de luz en cualquier punto
 * - Maneja la verificación de sombras
 * - Proporciona información de dirección y distancia
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "PointLight.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Interval.h"
#include "Vec3.h"

/**
 * @brief Constructor de luz puntual
 * @param pos Posición de la luz en el espacio 3D
 * @param color Color e intensidad de la luz
 */
PointLight::PointLight(const Vec3& pos, const Color& color) 
    : position(pos), intensity(color) {
}

/**
 * @brief Obtiene la intensidad de la luz en un punto dado
 * @param point Punto donde se quiere calcular la intensidad
 * @return Color que representa la intensidad de la luz
 */
Color PointLight::getIntensity(const Vec3& point) const {
    // Para luces puntuales, la intensidad puede variar con la distancia
    // Por simplicidad, usamos intensidad constante
    return intensity;
}

/**
 * @brief Calcula la dirección desde un punto hacia la luz
 * @param point Punto desde donde se calcula la dirección
 * @return Vector unitario que apunta hacia la luz
 */
Vec3 PointLight::getDirection(const Vec3& point) const {
    // Dirección desde el punto hacia la luz
    return unitVector(position - point);
}

/**
 * @brief Calcula la distancia entre un punto y la luz
 * @param point Punto desde donde se mide la distancia
 * @return Distancia euclidiana al punto de luz
 */
double PointLight::getDistance(const Vec3& point) const {
    return (position - point).length();
}

/**
 * @brief Verifica si un punto está en sombra respecto a esta luz
 * 
 * Lanza un rayo desde el punto hacia la luz y verifica si hay
 * intersecciones con objetos en el camino. Incluye un pequeño
 * offset para evitar el self-shadowing.
 * 
 * @param point Punto a verificar
 * @param world Entidad que representa la escena
 * @return true si el punto está en sombra, false en caso contrario
 */
bool PointLight::isInShadow(const Vec3& point, const Entity& world) const {
    // Crear rayo desde el punto hacia la luz
    Vec3 to_light = position - point;
    double distance_to_light = to_light.length();
    Vec3 light_direction = to_light / distance_to_light;
    
    // Crear rayo de sombra con pequeño offset para evitar self-shadowing
    Ray shadow_ray(point + light_direction * 0.001, light_direction);
    
    // Verificar intersección entre el punto y la luz
    HitRecord temp_rec;
    Interval shadow_interval(0.001, distance_to_light - 0.001);
    
    return world.hit(shadow_ray, shadow_interval, temp_rec);
}

/**
 * @brief Obtiene el color base de la luz
 * @return Color que representa la intensidad base de la luz
 */
Color PointLight::getColor() const {
    return intensity;
} 