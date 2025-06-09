/**
 * @file MaterialMirror.cpp
 * @brief Implementaci�n del material espejo (reflexi�n perfecta)
 *
 * El color reflejado se obtiene trazando un rayo reflejado a partir del punto
 * de intersecci�n usando el vector de reflexi�n ideal.
 *
 * @author Daniel Garc�a
 * @date 09/06/2025
 */

#include "MaterialMirror.h"
#include "Scene.h"
#include "Ray.h"
#include <cmath>

MaterialMirror::MaterialMirror(const Color& albedo, const WhittedTracer& tracer)
    : albedo(albedo), tracer(tracer) {
}

Color MaterialMirror::shade(const Ray& incident_ray, const HitRecord& hit_record,
    const Scene& scene, int depth) const {
    if (depth <= 0) return Color(0, 0, 0);

    Vec3 unit_direction = unitVector(incident_ray.getDirection());
    Vec3 reflected = reflect(unit_direction, hit_record.normal);
    Ray reflected_ray(hit_record.point + reflected * 1e-4, reflected);

    Color reflected_color = tracer.trace(reflected_ray, scene, depth - 1);
    return albedo * reflected_color;
}
