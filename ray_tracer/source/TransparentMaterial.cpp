/**
 * @file TransparentMaterial.cpp
 * @brief Implementación del material transparente con refracción
 * 
 * Implementa materiales transparentes que calculan tanto reflexión como refracción
 * usando las leyes de Snell y las ecuaciones de Fresnel, como parte del algoritmo
 * de Whitted Ray Tracing.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "TransparentMaterial.h"
#include "Constants.h"
#include "Interval.h"
#include <cmath>
#include <algorithm>

TransparentMaterial::TransparentMaterial(double refractive_index, double transparency, const Color& tint)
    : ref_index(refractive_index), transparency(Interval(0.0, 1.0).clamp(transparency)), tint(tint) {
}

Color TransparentMaterial::scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const {
    // Para materiales transparentes, el color viene principalmente de la refracción
    // Este método debe ser llamado por WhittedTracer que manejará la recursión
    
    // El color base es el tinte del material
    return tint;
}

double TransparentMaterial::getReflectionCoefficient() const {
    // Para materiales transparentes, la reflectancia depende del ángulo de incidencia
    // Esto debe calcularse dinámicamente usando las ecuaciones de Fresnel
    // Por ahora retornamos un valor base
    return 1.0 - transparency;
}

double TransparentMaterial::calculateReflectance(double cosine) const {
    // Calcula la reflectancia usando la aproximación de Schlick
    return schlickApproximation(cosine, ref_index);
}

// Nota: Los cálculos completos de reflexión y refracción, incluyendo
// el manejo de la ley de Snell y las ecuaciones de Fresnel, serían
// implementados en la clase WhittedTracer que tiene acceso completo
// a la geometría de la escena y puede manejar el trazado recursivo. 