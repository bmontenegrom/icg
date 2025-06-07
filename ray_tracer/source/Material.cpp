/**
 * @file Material.cpp
 * @brief Implementación de la clase base Material para el sistema de ray tracing
 * 
 * Contiene las implementaciones de los métodos utilitarios compartidos por todos
 * los tipos de materiales, incluyendo cálculos de reflexión, refracción y Fresnel.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "Material.h"
#include "Constants.h"
#include "Vec3.h"
#include <cmath>
#include <algorithm>

Vec3 Material::reflect(const Vec3& incident, const Vec3& normal) const {
    // Fórmula de reflexión: R = I - 2 * (I · N) * N
    // donde I es el vector incidente y N es la normal
    return incident - 2 * dotProduct(incident, normal) * normal;
}

bool Material::refract(const Vec3& incident, const Vec3& normal, double eta_ratio, Vec3& refracted) const {
    // Implementación de la ley de Snell para refracción
    // eta_ratio = eta_i / eta_t (ratio de índices de refracción)
    
    Vec3 unit_incident = unitVector(incident);
    double cos_theta = std::min(dotProduct(-unit_incident, normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
    
    // Verificar si hay reflexión total interna
    bool cannot_refract = eta_ratio * sin_theta > 1.0;
    
    if (cannot_refract) {
        return false; // Reflexión total interna
    }
    
    // Calcular el vector refractado usando la ley de Snell
    Vec3 r_out_perp = eta_ratio * (unit_incident + cos_theta * normal);
    Vec3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.lengthSquared())) * normal;
    refracted = r_out_perp + r_out_parallel;
    
    return true;
}

double Material::schlickApproximation(double cosine, double ref_idx) const {
    // Aproximación de Schlick para la reflectancia de Fresnel
    // Más rápida que el cálculo exacto de Fresnel
    
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
} 