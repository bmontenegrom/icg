/**
 * @file MaterialGlass.cpp
 * @brief Implementación del material transparente con reflexión y refracción
 *
 * Este archivo implementa el comportamiento óptico de materiales como el vidrio,
 * incluyendo reflexión especular (Fresnel), refracción (Snell), y reflexión total interna.
 * La interacción con la escena se realiza mediante el trazador de Whitted.
 *
 * @author Daniel García
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
    double eta_ratio = hit_record.frontFace ? (1.0 / ior) : ior;
    if (!hit_record.frontFace)
        normal = -normal;

    double cos_theta = fmin(dotProduct(-unit_direction, normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    bool total_internal_reflection = eta_ratio * sin_theta > 1.0;
    double reflect_prob = 0.05 + 0.95 * schlickApproximation(cos_theta, ior);

    Color reflect_color(0, 0, 0);
    Color refract_color(0, 0, 0);

    // Reflejo
    Vec3 reflected = reflect(unit_direction, normal);
    Ray reflect_ray(hit_record.p + normal * 1e-3, reflected);
    reflect_color = tracer.trace(reflect_ray, scene, depth - 1);

    // Refracción (solo si no hay reflexión total)
    if (!total_internal_reflection) {
        Vec3 refracted = refract(unit_direction, normal, eta_ratio);
        Ray refract_ray(hit_record.p - normal * 1e-3, refracted);
        refract_color = tracer.trace(refract_ray, scene, depth - 1);
    }

    // Mezcla de reflexión y refracción con Fresnel
    Color mixed = reflect_prob * reflect_color + (1.0 - reflect_prob) * refract_color;
    Color out = albedo * mixed;

    // Clamp para evitar colores saturados
    out.setR(std::max(0.0, std::min(out.getR(), 1.0)));
    out.setG(std::max(0.0, std::min(out.getG(), 1.0)));
    out.setB(std::max(0.0, std::min(out.getB(), 1.0)));
    return out;
}


double MaterialGlass::transparency() const {
    return 0.9;
}

double MaterialGlass::reflectivity() const {
    return 0.1;
}
