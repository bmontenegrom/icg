/**
 * @file PointLight.cpp
 * @brief Implementación de la clase PointLight para luces puntuales
 * 
 * Implementa una fuente de luz puntual con atenuación por distancia,
 * utilizada en el modelo de iluminación de Whitted Ray Tracing.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "PointLight.h"
#include "Constants.h"
#include "Interval.h"
#include "Color.h"
#include <cmath>
#include <algorithm>

PointLight::PointLight(const Vec3& position, const Color& color,
                       double constant_attenuation, double linear_attenuation, 
                       double quadratic_attenuation)
    : position(position), light_color(color),
      const_attenuation(constant_attenuation),
      linear_attenuation(linear_attenuation),
      quadratic_attenuation(quadratic_attenuation) {
}

Color PointLight::getIntensity(const Vec3& point) const {
    // Calcular la distancia desde el punto hasta la luz
    double distance = getDistance(point);
    
    // Calcular atenuación: 1 / (kc + kl*d + kq*d²)
    double attenuation = const_attenuation + 
                        linear_attenuation * distance + 
                        quadratic_attenuation * distance * distance;
    
    // Evitar división por cero
    attenuation = std::max(attenuation, 0.001);
    
    // Aplicar atenuación al color de la luz
    return light_color / attenuation;
}

Vec3 PointLight::getDirection(const Vec3& point) const {
    // Dirección desde el punto hacia la luz
    Vec3 light_vector = position - point;
    return unitVector(light_vector);
}

double PointLight::getDistance(const Vec3& point) const {
    // Distancia euclidiana desde el punto hasta la luz
    Vec3 distance_vector = position - point;
    return distance_vector.length();
}

bool PointLight::isInShadow(const Vec3& point, const Entity& world) const {
    // Crear un rayo desde el punto hacia la luz
    Vec3 light_direction = getDirection(point);
    double light_distance = getDistance(point);
    
    // Aplicar un pequeño offset para evitar self-shadowing
    Vec3 shadow_ray_origin = point + 0.001 * light_direction;
    Ray shadow_ray(shadow_ray_origin, light_direction);
    
    // Verificar intersecciones entre el punto y la luz
    HitRecord temp_rec;
    Interval shadow_interval(0.001, light_distance - 0.001);
    
    // Si hay una intersección antes de llegar a la luz, está en sombra
    return world.hit(shadow_ray, shadow_interval, temp_rec);
} 