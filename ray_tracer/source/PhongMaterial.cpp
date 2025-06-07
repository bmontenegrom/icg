/**
 * @file PhongMaterial.cpp
 * @brief Implementación del material que utiliza el modelo de iluminación de Phong
 * 
 * Implementa el modelo clásico de Phong con componentes ambiente, difusa y especular,
 * además de capacidades de reflexión para el algoritmo de Whitted Ray Tracing.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "PhongMaterial.h"
#include "WhittedTracer.h"
#include "Constants.h"
#include "Vec3.h"
#include <cmath>
#include <algorithm>
#include <Color.h>

PhongMaterial::PhongMaterial(const Color& ambient, const Color& diffuse, 
                            const Color& specular, double shininess, double reflection)
    : ambient_color(ambient), diffuse_color(diffuse), specular_color(specular),
      shininess(shininess), reflection_coeff(reflection) {
}

Color PhongMaterial::scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const {
    // Este método sería llamado por WhittedTracer
    // Por ahora retornamos solo el color difuso
    // La implementación completa requiere acceso a las luces de la escena
    return diffuse_color;
}

Color PhongMaterial::calculateLocalIllumination(const HitRecord& hit_record,
                                               const std::vector<std::shared_ptr<Light>>& lights,
                                               const Vec3& view_direction,
                                               const Entity& world) const {
    Color final_color(0, 0, 0);
    
    // Componente ambiente
    Color ambient_component = ambient_color;
    final_color += ambient_component;
    
    // Iterar sobre todas las luces para calcular componentes difusa y especular
    for (const auto& light : lights) {
        // Verificar si el punto está en sombra
        if (light->isInShadow(hit_record.point, world)) {
            continue; // Esta luz está bloqueada
        }
        
        // Obtener información de la luz
        Vec3 light_dir = light->getDirection(hit_record.point);
        Color light_intensity = light->getIntensity(hit_record.point);
        
        // Componente difusa (Lambert)
        double dot_nl = std::max(0.0, dotProduct(hit_record.normal, light_dir));
        Color diffuse_component = diffuse_color * light_intensity * dot_nl;
        final_color += diffuse_component;
        
        // Componente especular (Phong)
        if (dot_nl > 0.0) { // Solo calcular si hay iluminación difusa
            Vec3 reflection_dir = reflect(-light_dir, hit_record.normal);
            double dot_rv = std::max(0.0, dotProduct(reflection_dir, view_direction));
            double specular_factor = std::pow(dot_rv, shininess);
            
            Color specular_component = specular_color * light_intensity * specular_factor;
            final_color += specular_component;
        }
    }
    
    return final_color;
} 