/**
 * @file DirectionalLight.cpp
 * @brief Implementación de la clase DirectionalLight para luces direccionales
 * 
 * Implementa una fuente de luz direccional que simula luz distante (como el sol)
 * con rayos paralelos y sin atenuación por distancia.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "DirectionalLight.h"
#include "Constants.h"
#include "Interval.h"

DirectionalLight::DirectionalLight(const Vec3& direction, const Color& color)
    : direction(unitVector(-direction)), light_color(color) {
    // Nota: negamos la dirección porque queremos la dirección HACIA la luz,
    // no la dirección DE la luz
}

Color DirectionalLight::getIntensity(const Vec3& point) const {
    // Las luces direccionales no tienen atenuación por distancia
    return light_color;
}

Vec3 DirectionalLight::getDirection(const Vec3& point) const {
    // Para luces direccionales, la dirección es constante desde cualquier punto
    return direction;
}

double DirectionalLight::getDistance(const Vec3& point) const {
    // Las luces direccionales están "infinitamente" lejos
    return infinity;
}

bool DirectionalLight::isInShadow(const Vec3& point, const Entity& world) const {
    // Crear un rayo desde el punto en la dirección de la luz
    Vec3 shadow_ray_origin = point + 0.001 * direction; // Offset para evitar self-shadowing
    Ray shadow_ray(shadow_ray_origin, direction);
    
    // Para luces direccionales, verificamos intersecciones hasta el infinito
    HitRecord temp_rec;
    Interval shadow_interval(0.001, infinity);
    
    // Si hay cualquier intersección en la dirección de la luz, está en sombra
    return world.hit(shadow_ray, shadow_interval, temp_rec);
} 