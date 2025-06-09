/**
 * @file MaterialMirror.h
 * @brief Definición de material espejo (reflexión perfecta)
 *
 * Este material refleja todos los rayos entrantes usando la ley de reflexión.
 * No tiene componente difusa ni ambiente, y es ideal para superficies reflectivas
 * como espejos o metal pulido.
 *
 * Referencias:
 * - Whitted, T. (1980). "An improved illumination model for shaded display"
 *
 * @author Daniel García
 * @date 09/06/2025
 */

#pragma once
#include "Material.h"
#include "Color.h"
#include "WhittedTracer.h"

class MaterialMirror : public Material {
public:
    Color albedo;                   ///< Intensidad del reflejo (color reflectado)
    const WhittedTracer& tracer;   ///< Referencia al trazador

    MaterialMirror(const Color& albedo, const WhittedTracer& tracer);

    Color shade(const Ray& incident_ray, const HitRecord& hit_record,
        const Scene& scene, int depth) const override;

    double reflectivity() const override { return 1.0; }
    double transparency() const override { return 0.0; }
};
