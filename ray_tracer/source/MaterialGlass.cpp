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
    /*
    if (depth >= 10) return Color(0, 0, 0);

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

    Color reflect_color = Color(0, 0, 0);
    Color refract_color = Color(0, 0, 0);




    
    Vec3 direction;
    if (total_internal_reflection || reflect_prob > static_cast<double>(rand()) / RAND_MAX) {
        direction = reflect(unit_direction, normal);
    }
    else {
        direction = refract(unit_direction, normal, eta_ratio);
    }

    Ray scattered(hit_record.p + direction * 1e-4, direction);
    Color result = tracer.trace(scattered, scene, depth + 1);
    return albedo*result;
    

    // Reflexión
    Vec3 reflect_dir = reflect(unit_direction, normal);
    Ray reflect_ray(hit_record.point + reflect_dir * 1e-4, reflect_dir);
    reflect_color = tracer.trace(reflect_ray, scene, depth + 1);

    // Refracción (si no hay reflexión total)
    if (!total_internal_reflection) {
        Vec3 refract_dir = refract(unit_direction, normal, eta_ratio);
        Ray refract_ray(hit_record.point + refract_dir * 1e-4, refract_dir);
        refract_color = tracer.trace(refract_ray, scene, depth + 1);
    }

    // Mezcla: parte reflejada y parte transmitida
    Color result = reflect_prob * reflect_color + (1.0 - reflect_prob) * refract_color;

    // Atenuación por color del vidrio
    return albedo * result;

    */


    if (depth >= 10) return Color(0, 0, 0);

    Vec3 unit_dir = unitVector(incident_ray.getDirection());
    Vec3 normal = hit_record.normal;
    bool front_face = hit_record.frontFace;

    double eta_ratio = front_face ? (1.0 / ior) : ior;
    if (!front_face) normal = -normal;

    double cos_theta = fmin(dotProduct(-unit_dir, normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool total_internal_reflection = eta_ratio * sin_theta > 1.0;

    double reflect_prob = schlickApproximation(cos_theta, eta_ratio);
    Color reflection_color(0, 0, 0);
    Color transmission_color(0, 0, 0);

    // Calcular reflexión
    Vec3 reflected = reflect(unit_dir, normal);
    Ray reflected_ray(hit_record.point + reflected * 1e-4, reflected);
    if (reflectivity() > 0.01) {
        reflection_color = tracer.trace(reflected_ray, scene, depth + 1);
    }

    // Calcular refracción si no hay reflexión total
    if (!total_internal_reflection) {
        Vec3 refracted = refract(unit_dir, normal, eta_ratio);
        Ray refracted_ray(hit_record.point + refracted * 1e-4, refracted);
        transmission_color =tracer.trace(refracted_ray, scene, depth + 1);
    }

    // Mezcla según Schlick
    //Color combined = reflect_prob * reflection_color + (1.0 - reflect_prob) * transmission_color;

    // Atenuar con albedo del vidrio (color del material)
    //return albedo * combined;
    return albedo * (reflection_color + transmission_color);
}
