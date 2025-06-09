/**
 * @file MaterialGlass.cpp
 * @brief Implementaci�n del material transparente con reflexi�n y refracci�n
 *
 * Este archivo implementa el comportamiento �ptico de materiales como el vidrio,
 * incluyendo reflexi�n especular (Fresnel), refracci�n (Snell), y reflexi�n total interna.
 * La interacci�n con la escena se realiza mediante el trazador de Whitted.
 *
 * @author Daniel Garc�a
 * @date 08/06/2025
 */
#include "MaterialGlass.h"
#include "Scene.h"
#include "Ray.h"
#include <cmath>
#include <iomanip>

MaterialGlass::MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer)
    : albedo(albedo), ior(ior), tracer(tracer) {
}

Color MaterialGlass::shade(const Ray& incident_ray, const HitRecord& hit_record, const Scene& scene, int depth) const {
    if (depth <= 0) return Color(0, 0, 0);

    Vec3 unit_direction = unitVector(incident_ray.getDirection());
    Vec3 normal = hit_record.normal;
    bool going_outside = hit_record.frontFace;
    double eta_ratio = going_outside ? (1.0 / ior) : ior;
    if (!going_outside)
        normal = -normal;

    double cos_theta = fmin(dotProduct(-unit_direction, normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool total_internal_reflection = eta_ratio * sin_theta > 1.0;
    double reflect_prob = schlickApproximation(cos_theta, eta_ratio);

    Vec3 direction;
    if (total_internal_reflection || reflect_prob > static_cast<double>(rand()) / RAND_MAX) {
        direction = reflect(unit_direction, normal);
    }
    else {
        direction = refract(unit_direction, normal, eta_ratio);
    }

    Ray scattered(hit_record.p + direction * 1e-4, direction);
    Color result = tracer.trace(scattered, scene, depth - 1);
    return albedo * result;
}
