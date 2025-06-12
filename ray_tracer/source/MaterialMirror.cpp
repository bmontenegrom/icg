/**
 * @file MaterialMirror.cpp
 * @brief Implementación del material espejo (reflexión perfecta)
 *
 * El color reflejado se obtiene trazando un rayo reflejado a partir del punto
 * de intersección usando el vector de reflexión ideal.
 *
 * @author Daniel García
 * @date 11/06/2025
 */

#include "MaterialMirror.h"
#include "Scene.h"
#include "Ray.h"
#include <cmath>

MaterialMirror::MaterialMirror(const Color& albedo, const WhittedTracer& tracer)
    : albedo(albedo), tracer(tracer), reflectivity(1.0), transparency(0.0) {
}

MaterialMirror::MaterialMirror(const Color& albedo, const WhittedTracer& tracer, double reflectivity, double transparency)
	: albedo(albedo), tracer(tracer), reflectivity(reflectivity), transparency(transparency)
{
}


Color MaterialMirror::shade(const Ray& incident_ray, const HitRecord& hit_record, const Scene& scene, int depth) const {
    if (depth >= tracer.getMaxDepth()) {
        return Color(0, 0, 0);
    }

    Vec3 unit_direction = unitVector(incident_ray.getDirection());
    Vec3 reflected = reflect(unit_direction, hit_record.normal);

    Ray reflected_ray(hit_record.point + reflected * 1e-4, reflected);
    Color reflected_color = tracer.trace(reflected_ray, scene, depth + 1);

    return albedo * reflected_color * getReflectivity();
}

double MaterialMirror::getReflectivity() const
{
    return reflectivity;
}

double MaterialMirror::getTransparency() const
{
    return transparency;
}


