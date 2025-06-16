#include "MaterialTextured.h"
#include "Scene.h"
#include <cmath>

MaterialTextured::MaterialTextured(const Texture& tex, double shin)
    : texture(tex), shininess(shin) {
}

Color MaterialTextured::shade(const Ray& r_in, const HitRecord& rec, const Scene& scene, int depth) const {
    Color tex_color = texture.sample(rec.u, rec.v);

    // Componente ambiente (basada en la textura)
    Color result = tex_color; // ambiente global

    for (const auto& light : scene.lights) {
        Vec3 to_light = unitVector(light->getDirection(rec.point));
        Ray shadow_ray(rec.point + rec.normal * 0.001f, to_light);
        double dist = light->getDistance(rec.point);
        Color transmission = scene.transmissionAlong(shadow_ray, dist);

        // Difusa (Lambert)
        double cos_theta = std::max(0.0, dotProduct(rec.normal, to_light));
        Color diffuse = (tex_color / PI) * light->getIntensity(rec.point) * cos_theta;

        // Especular (Phong)
        Vec3 reflect_dir = reflect(-to_light, rec.normal);
        Vec3 view_dir = unitVector(-r_in.getDirection());
        double spec = std::pow(std::max(dotProduct(view_dir, reflect_dir), 0.0), shininess);
        Color specular = tex_color * light->getIntensity(rec.point) * spec;

        result += transmission * (diffuse + specular);
    }

    return result;
}

Color MaterialTextured::shadeComponent(ShadeComponent component,
    const Ray& r_in,
    const HitRecord& rec,
    const Scene& scene) const {
    Color tex_color = texture.sample(rec.u, rec.v);

    if (component == ShadeComponent::Ambient) {
        return tex_color;
    }

    Color result(0, 0, 0);
    for (const auto& light : scene.lights) {
        Vec3 to_light = unitVector(light->getDirection(rec.point));
        Ray shadow_ray(rec.point + rec.normal * 0.001f, to_light);
        double dist = light->getDistance(rec.point);
        Color transmission = scene.transmissionAlong(shadow_ray, dist);

        if (component == ShadeComponent::Diffuse) {
            double cos_theta = std::max(0.0, dotProduct(rec.normal, to_light));
            Color diffuse = (tex_color / PI) * light->getIntensity(rec.point) * cos_theta;
            result += transmission * diffuse;
        }

        if (component == ShadeComponent::Specular) {
            Vec3 reflect_dir = reflect(-to_light, rec.normal);
            Vec3 view_dir = unitVector(-r_in.getDirection());
            double spec = std::pow(std::max(dotProduct(view_dir, reflect_dir), 0.0), shininess);
            Color specular = tex_color * light->getIntensity(rec.point) * spec;
            result += transmission * specular;
        }
    }

    return result;
}