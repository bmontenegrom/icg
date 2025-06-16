/**
 * @file Material.cpp
 * @brief Implementación de la clase base Material para el sistema de ray tracing
 * 
 * Este archivo implementa las funcionalidades base para todos los materiales:
 * - Cálculo de reflexión especular
 * - Cálculo de refracción usando la ley de Snell
 * - Aproximación de Schlick para la reflectancia de Fresnel
 * - Funciones utilitarias compartidas por todos los materiales
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "Material.h"
#include "Constants.h"
#include "Vec3.h"
#include <cmath>
#include <algorithm>

/**
 * @brief Calcula la dirección de reflexión especular
 * 
 * Implementa la ley de reflexión: ángulo de incidencia = ángulo de reflexión
 * usando la fórmula R = I - 2(N·I)N donde:
 * - I es el vector incidente
 * - N es la normal normalizada
 * - R es el vector reflejado
 * 
 * @param incident Vector de dirección incidente normalizado
 * @param normal Vector normal a la superficie normalizado
 * @return Vector de dirección reflejada
 */
Vec3 Material::reflect(const Vec3& incident, const Vec3& normal) const {
    return incident - 2.0 * dotProduct(incident, normal) * normal;
}

/**
 * @brief Calcula la dirección de refracción usando la ley de Snell
 * 
 * Implementa la refracción considerando:
 * - Ley de Snell (n1*sin(θ1) = n2*sin(θ2))
 * - Componentes perpendicular y paralela del rayo refractado
 * - Índice de refracción relativo entre medios
 * 
 * @param incident Vector de dirección incidente normalizado
 * @param normal Vector normal a la superficie normalizado
 * @param ref_idx Índice de refracción relativo (n1/n2)
 * @return Vector de dirección refractada
 */
Vec3 Material::refract(const Vec3& incident, const Vec3& normal, double ref_idx) const {
    double cos_theta = std::min(-dotProduct(incident, normal), 1.0);
    Vec3 r_out_perp = ref_idx * (incident + cos_theta * normal);
    Vec3 r_out_parallel = -std::sqrt(std::abs(1.0 - r_out_perp.lengthSquared())) * normal;
    return r_out_perp + r_out_parallel;
}

/**
 * @brief Calcula la reflectancia usando la aproximación de Schlick
 * 
 * Implementa la aproximación de Christophe Schlick para el
 * coeficiente de reflexión de Fresnel. La fórmula es:
 * R(θ) = R0 + (1 - R0)(1 - cos(θ))^5
 * donde R0 = ((n1-n2)/(n1+n2))^2
 * 
 * @param cosine Coseno del ángulo entre la normal y el rayo
 * @param ref_idx Índice de refracción relativo
 * @return Coeficiente de reflexión de Fresnel
 */
double Material::schlickApproximation(double cosine, double ref_idx) const {
    // Usar la aproximación de Schlick para la reflectancia de Fresnel
    double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * std::pow(1.0 - cosine, 5.0);
}
Color Material::shadeComponent(ShadeComponent component, const Ray& ray, const HitRecord& hit, const Scene& scene) const
{
    return Color(0,0,0);
}
