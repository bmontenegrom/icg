#pragma once
#include "Material.h"

/**
 * @brief Material que simula una superficie perfectamente reflectante (espejo)
 * 
 * Implementa un material que refleja perfectamente los rayos incidentes
 * según la ley de reflexión: ángulo de incidencia = ángulo de reflexión.
 * Utilizado en el algoritmo de Whitted Ray Tracing para generar reflexiones.
 * 
 * Referencias:
 * - Whitted, T. (1980). "An improved illumination model for shaded display"
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class MirrorMaterial : public Material {
public:
    /**
     * @brief Constructor para material espejo
     * @param reflectance Coeficiente de reflectancia (0.0 a 1.0)
     * @param tint Color de tinte del espejo (Color(1,1,1) para espejo perfecto)
     */
    MirrorMaterial(double reflectance = 1.0, const Color& tint = Color(1.0, 1.0, 1.0));

    /**
     * @brief Implementa reflexión especular perfecta
     */
    Color scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const override;

    double getReflectionCoefficient() const override { return reflectance; }
    double getTransmissionCoefficient() const override { return 0.0; }

private:
    double reflectance;  ///< Coeficiente de reflectancia
    Color tint;          ///< Color de tinte del espejo
}; 