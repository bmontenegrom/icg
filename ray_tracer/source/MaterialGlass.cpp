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
    : albedo(albedo), ior(ior), tracer(tracer), reflectivity(0.1), transparency(0.9)  {
}

MaterialGlass::MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer, double reflectivity, double transparency)
	: albedo(albedo), ior(ior), tracer(tracer), reflectivity(reflectivity), transparency(transparency) {}

Color MaterialGlass::shade(const Ray& incident_ray, const HitRecord& hit_record, const Scene& scene, int depth) const {
    if (depth >= tracer.getMaxDepth()) {
        return albedo;
    }

    Vec3 unit_dir = unitVector(incident_ray.getDirection());
    Vec3 normal = hit_record.normal;
    bool front_face = hit_record.frontFace;

    double eta_ratio = front_face ? (1.0 / ior) : ior;
    //if (!front_face) normal = -normal;

    double cos_theta = fmin(dotProduct(-unit_dir, normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool total_internal_reflection = eta_ratio * sin_theta > 1.0;

    double reflect_prob = schlickApproximation(cos_theta, eta_ratio);

    // Calcular reflexión
    Vec3 reflected = reflect(unit_dir, normal);
    Ray reflected_ray(hit_record.point + reflected * 1e-4, reflected);
    Color reflection_color = tracer.trace(reflected_ray, scene, depth + 1);

    // Calcular refracción solo si no hay reflexión total
    Color transmission_color(0, 0, 0);
    if (!total_internal_reflection) {
        Vec3 refracted = refract(unit_dir, normal, eta_ratio);
        Ray refracted_ray(hit_record.point + refracted * 1e-4, refracted);
        transmission_color = tracer.trace(refracted_ray, scene, depth + 1);
    }

    // Mezcla ponderada según Fresnel
    Color final_color = getReflectivity()* reflect_prob * reflection_color + getTransparency()*(1 - reflect_prob) * transmission_color;
    return albedo * final_color;
}

double MaterialGlass::getReflectivity() const {
	return reflectivity;
}
double MaterialGlass::getTransparency() const {
    return transparency;
}