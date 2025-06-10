/**
 * @file LambertianMaterial.cpp
 * @brief Implementación del material Lambertiano con modelo de Phong
 * 
 * Este archivo implementa el material Lambertiano que incluye:
 * - Componente ambiental para iluminación global
 * - Componente difusa Lambertiana normalizada
 * - Componente especular de Phong
 * - Manejo de sombras y reflexiones
 * 
 * El material implementa el modelo completo de Phong con las tres
 * componentes de iluminación: ambiental, difusa y especular.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "LambertianMaterial.h"
#include "Scene.h"
#include "Vec3.h"
#include "Ray.h"
#include "Constants.h"
#include <algorithm>
#include <cmath>

/**
 * @brief Constructor del material Lambertiano
 * @param a Color del componente ambiental
 * @param d Color del componente difuso
 * @param s Color del componente especular
 * @param sh Exponente de brillo (shininess) para el componente especular
 */
LambertianMaterial::LambertianMaterial(const Color& a, const Color& d, const Color& s, double sh)
    : ambient(a), diffuse(d), specular(s), shininess(sh) {
}

/**
 * @brief Calcula el color resultante para un punto de intersección usando el modelo de Phong
 * 
 * Este método implementa el modelo de iluminación de Phong completo:
 * 1. Calcula la iluminación ambiental global
 * 2. Para cada luz en la escena:
 *    - Verifica si el punto está en sombra
 *    - Calcula la componente difusa usando la ley de Lambert
 *    - Calcula la componente especular usando el modelo de Phong
 * 3. Combina todas las componentes para obtener el color final
 * 
 * @param r_in Rayo incidente que intersecta con la superficie
 * @param rec Registro con la información de la intersección
 * @param scene Escena que contiene las luces y objetos
 * @param depth Profundidad actual de la recursión para reflexiones
 * @return Color resultante después de aplicar el modelo de iluminación
 */
Color LambertianMaterial::shade(const Ray& r_in, const HitRecord& rec, const Scene& scene, int depth) const {
    // Componente ambiental (iluminación global básica)
    Color result = ambient;

    for (const auto& light : scene.lights) {
        Vec3 to_light = unitVector(light->getDirection(rec.point));
        /*
        // Verificar sombras
        if (light->isInShadow(rec.point, *scene.world))
            continue;
        */
		Ray shadow_ray(rec.point + rec.normal * 0.001, to_light);
        double distance_to_light = light->getDistance(rec.point);
		Color transmission = scene.transmissionAlong(shadow_ray, distance_to_light);
		/*
		if (transmission.nearZero()) {
			continue; // Si está en sombra, no contribuye a la iluminación
		}
        */
        // Componente difusa Lambertiana
        // Ecuación: k_d * (N·L) * intensidad_luz con normalización por π
        double cos_theta = std::max(0.0, dotProduct(rec.normal, to_light));
        Color diffuse_contribution = (diffuse / PI) * light->getIntensity(rec.point) * cos_theta;
        
        // Componente especular (Phong)
        // Ecuación: k_s * (R·V)^α * intensidad_luz
        Vec3 reflect_dir = reflect(-to_light, rec.normal);
        Vec3 view_dir = unitVector(-r_in.getDirection());
        double spec_intensity = std::pow(std::max(0.0, dotProduct(view_dir, reflect_dir)), shininess);
        Color specular_contribution = specular * light->getIntensity(rec.point) * spec_intensity;

        result += transmission * (diffuse_contribution + specular_contribution);
    }

    return result;
} 
