#pragma once
#include "Material.h"

/**
 * @brief Material transparente que implementa refracción según la ley de Snell
 * 
 * Simula materiales transparentes como vidrio, agua, etc. Calcula tanto
 * reflexión como refracción usando las ecuaciones de Fresnel para determinar
 * qué porción de luz se refleja vs se refracta.
 * 
 * La ley de Snell: n₁ * sin(θ₁) = n₂ * sin(θ₂)
 * Las ecuaciones de Fresnel determinan la reflectancia basada en el ángulo de incidencia.
 * 
 * Referencias:
 * - Whitted, T. (1980). "An improved illumination model for shaded display"
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class TransparentMaterial : public Material {
public:
    /**
     * @brief Constructor para material transparente
     * @param refractive_index Índice de refracción del material (ej: 1.5 para vidrio)
     * @param transparency Coeficiente de transparencia (0.0 a 1.0)
     * @param tint Color de tinte del material transparente
     */
    TransparentMaterial(double refractive_index, double transparency = 1.0, 
                       const Color& tint = Color(1.0, 1.0, 1.0));

    /**
     * @brief Implementa refracción con reflexión de Fresnel
     */
    Color scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const override;

    double getReflectionCoefficient() const override;
    double getTransmissionCoefficient() const override { return transparency; }

private:
    double ref_index;     ///< Índice de refracción del material
    double transparency;  ///< Coeficiente de transparencia
    Color tint;          ///< Color de tinte del material

    /**
     * @brief Calcula la reflectancia usando la aproximación de Schlick
     * @param cosine Coseno del ángulo de incidencia
     * @return Coeficiente de reflectancia
     */
    double calculateReflectance(double cosine) const;
}; 