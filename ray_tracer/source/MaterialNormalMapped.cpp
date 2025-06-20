#include "MaterialNormalMapped.h"
#include "Scene.h"

MaterialNormalMapped::MaterialNormalMapped(const Color& ambient, const Color& diffuse, const Color& specular, float shininess, const Texture& normalMap)
	: ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess), normalMap(normalMap) {
}

Color MaterialNormalMapped::shade(const Ray& r_in, const HitRecord& rec, const Scene& scene, int depth) const
{
    Vec3 perturbed_normal = perturbNormal(rec);

    Color result = ambient;

    for (const auto& light : scene.lights) {
        Vec3 to_light = unitVector(light->getDirection(rec.point));
        Ray shadow_ray(rec.point + rec.normal * 0.001, to_light);
        double distance_to_light = light->getDistance(rec.point);
        Color transmission = scene.transmissionAlong(shadow_ray, distance_to_light);

        // Difusa Lambertiana
        double cos_theta = std::max(0.0, dotProduct(perturbed_normal, to_light));
        Color diffuse_contribution = (diffuse / PI) * light->getIntensity(rec.point) * cos_theta;

        // Especular (Phong)
        Vec3 reflect_dir = reflect(-to_light, perturbed_normal);
        Vec3 view_dir = unitVector(-r_in.getDirection());
        double spec_intensity = std::pow(std::max(0.0, dotProduct(view_dir, reflect_dir)), shininess);
        Color specular_contribution = specular * light->getIntensity(rec.point) * spec_intensity;

        result += transmission * (diffuse_contribution + specular_contribution);
    }

    return result;
}

Color MaterialNormalMapped::shadeComponent(ShadeComponent component, const Ray& r_in, const HitRecord& rec, const Scene& scene) const
{
    Vec3 perturbed_normal = perturbNormal(rec);
    Color result(0, 0, 0);

    if (component == ShadeComponent::Ambient) {
        return ambient;
    }

    for (const auto& light : scene.lights) {
        Vec3 to_light = unitVector(light->getDirection(rec.point));
        Ray shadow_ray(rec.point + rec.normal * 0.001, to_light);
        double distance_to_light = light->getDistance(rec.point);
        Color transmission = scene.transmissionAlong(shadow_ray, distance_to_light);

        if (component == ShadeComponent::Diffuse) {
            double cos_theta = std::max(0.0, dotProduct(perturbed_normal, to_light));
            Color diffuse_contribution = (diffuse / PI) * light->getIntensity(rec.point) * cos_theta;
            result += transmission * diffuse_contribution;
        }

        if (component == ShadeComponent::Specular) {
            Vec3 reflect_dir = reflect(-to_light, perturbed_normal);
            Vec3 view_dir = unitVector(-r_in.getDirection());
            double spec_intensity = std::pow(std::max(0.0, dotProduct(view_dir, reflect_dir)), shininess);
            Color specular_contribution = specular * light->getIntensity(rec.point) * spec_intensity;
            result += transmission * specular_contribution;
        }
    }

    return result;
}

Vec3 MaterialNormalMapped::perturbNormal(const HitRecord& rec) const
{
    Color map = normalMap.sample(rec.u, rec.v); //  [0, 1]
    Vec3 n_map(map.getR() * 2.0 - 1.0,
        map.getG() * 2.0 - 1.0,
        map.getB() * 2.0 - 1.0); //[-1, 1]

    Vec3 N = unitVector(rec.normal);
    Vec3 T = unitVector(std::abs(N.getX()) > 0.9 ? Vec3(0, 1, 0) : crossProduct(Vec3(1, 0, 0), N));
    Vec3 B = crossProduct(N, T);

    Vec3 perturbed = n_map.getX() * T + n_map.getY() * B + n_map.getZ() * N;
    return unitVector(perturbed);
}
