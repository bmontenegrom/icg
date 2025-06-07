/**
 * @file WhittedTracer.cpp
 * @brief Implementación del algoritmo de Ray Tracing de Whitted
 * 
 * Implementa el algoritmo clásico de ray tracing de Turner Whitted con
 * soporte para reflexión, refracción, sombras múltiples luces y generación
 * de imágenes auxiliares de reflexión y transmisión.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "WhittedTracer.h"
#include "Constants.h"
#include "PhongMaterial.h"
#include "MirrorMaterial.h"
#include "Camera.h"
#include "TransparentMaterial.h"
#include "FreeImage.h"
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include <algorithm>
#include <iostream>

WhittedTracer::WhittedTracer(int max_depth, double shadow_bias)
    : max_depth(max_depth), shadow_bias(shadow_bias) {
}

void WhittedTracer::setLights(const std::vector<std::shared_ptr<Light>>& lights) {
    this->lights = lights;
}

Color WhittedTracer::trace(const Ray& ray, const Entity& world, int depth) const {
    // Verificar límite de profundidad de recursión
    if (depth >= max_depth) {
        return Color(0, 0, 0); // Negro si se excede la profundidad
    }
    
    // Encontrar la intersección más cercana
    HitRecord hit_record;
    if (!world.hit(ray, Interval(0.001, infinity), hit_record)) {
        // No hay intersección, retornar color de fondo
        return backgroundColor(ray);
    }
    
    // Verificar si el objeto tiene material
    if (!hit_record.material_ptr) {
        // Material por defecto (difuso gris)
        return Color(0.5, 0.5, 0.5);
    }
    
    Color final_color(0, 0, 0);
    
    // Calcular dirección hacia el observador
    Vec3 view_direction = unitVector(-ray.getDirection());
    
    // 1. Iluminación directa (componentes local de Phong)
    final_color += calculateDirectLighting(hit_record, *hit_record.material_ptr, 
                                          view_direction, world);
    
    // 2. Reflexión especular
    if (hit_record.material_ptr->getReflectionCoefficient() > 0.0) {
        Color reflection_color = calculateReflection(ray, hit_record, 
                                                   *hit_record.material_ptr, 
                                                   world, depth);
        final_color += reflection_color * hit_record.material_ptr->getReflectionCoefficient();
    }
    
    // 3. Refracción/Transmisión
    if (hit_record.material_ptr->getTransmissionCoefficient() > 0.0) {
        Color refraction_color = calculateRefraction(ray, hit_record, 
                                                   *hit_record.material_ptr, 
                                                   world, depth);
        final_color += refraction_color * hit_record.material_ptr->getTransmissionCoefficient();
    }
    
    return final_color;
}

Color WhittedTracer::calculateDirectLighting(const HitRecord& hit_record,
                                           const Material& material,
                                           const Vec3& view_direction,
                                           const Entity& world) const {
    Color direct_color(0, 0, 0);
    
    // Intentar cast a PhongMaterial para usar su método de iluminación
    const PhongMaterial* phong_material = dynamic_cast<const PhongMaterial*>(&material);
    if (phong_material) {
        direct_color = phong_material->calculateLocalIllumination(hit_record, lights, 
                                                                 view_direction, world);
    } else {
        // Para otros materiales, calcular iluminación básica
        for (const auto& light : lights) {
            if (!light->isInShadow(hit_record.point, world)) {
                Vec3 light_dir = light->getDirection(hit_record.point);
                Color light_intensity = light->getIntensity(hit_record.point);
                
                // Solo componente difusa básica
                double dot_nl = std::max(0.0, dotProduct(hit_record.normal, light_dir));
                direct_color += material.scatter(Ray(), hit_record, 0) * light_intensity * dot_nl;
            }
        }
    }
    
    return direct_color;
}

Color WhittedTracer::calculateReflection(const Ray& incident_ray,
                                       const HitRecord& hit_record,
                                       const Material& material,
                                       const Entity& world,
                                       int depth) const {
    // Calcular dirección de reflexión
    Vec3 reflected_direction = material.reflect(incident_ray.getDirection(), hit_record.normal);
    
    // Crear rayo reflejado con pequeño offset para evitar self-intersection
    Vec3 reflection_origin = hit_record.point + shadow_bias * hit_record.normal;
    Ray reflected_ray(reflection_origin, reflected_direction);
    
    // Trazar rayo reflejado recursivamente
    return trace(reflected_ray, world, depth + 1);
}

Color WhittedTracer::calculateRefraction(const Ray& incident_ray,
                                       const HitRecord& hit_record,
                                       const Material& material,
                                       const Entity& world,
                                       int depth) const {
    // Implementación básica de refracción
    // Para una implementación completa se necesitaría información sobre
    // índices de refracción y manejo de la ley de Snell
    
    const TransparentMaterial* transparent = dynamic_cast<const TransparentMaterial*>(&material);
    if (!transparent) {
        return Color(0, 0, 0);
    }
    
    // Por simplicidad, aquí solo simulamos transmisión directa
    // Una implementación completa calcularía la refracción usando la ley de Snell
    Vec3 transmission_direction = incident_ray.getDirection();
    Vec3 transmission_origin = hit_record.point - shadow_bias * hit_record.normal;
    Ray transmitted_ray(transmission_origin, transmission_direction);
    
    return trace(transmitted_ray, world, depth + 1);
}

Color WhittedTracer::backgroundColor(const Ray& ray) const {
    // Gradiente azul simple para simular el cielo
    Vec3 unit_direction = unitVector(ray.getDirection());
    double t = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void WhittedTracer::generateReflectionMap(const Entity& world, Camera& camera,
                                        int width, int height, 
                                        const std::string& filename) const {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    
    if (!bitmap) {
        std::cerr << "Error creando imagen de reflexión.\n";
        FreeImage_DeInitialise();
        return;
    }
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            HitRecord hit_record;
            
            double reflection_coeff = 0.0;
            if (world.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    reflection_coeff = hit_record.material_ptr->getReflectionCoefficient();
                }
            }
            
            // Convertir coeficiente a escala de grises (0-255)
            BYTE gray_value = static_cast<BYTE>(reflection_coeff * 255);
            
            RGBQUAD color;
            color.rgbRed = gray_value;
            color.rgbGreen = gray_value;
            color.rgbBlue = gray_value;
            
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }
    }
    
    if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), 0)) {
        std::cout << "Mapa de reflexión guardado: " << filename << std::endl;
    } else {
        std::cerr << "Error guardando mapa de reflexión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}

void WhittedTracer::generateTransmissionMap(const Entity& world, Camera& camera,
                                          int width, int height,
                                          const std::string& filename) const {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    
    if (!bitmap) {
        std::cerr << "Error creando imagen de transmisión.\n";
        FreeImage_DeInitialise();
        return;
    }
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            HitRecord hit_record;
            
            double transmission_coeff = 0.0;
            if (world.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    transmission_coeff = hit_record.material_ptr->getTransmissionCoefficient();
                }
            }
            
            // Convertir coeficiente a escala de grises (0-255)
            BYTE gray_value = static_cast<BYTE>(transmission_coeff * 255);
            
            RGBQUAD color;
            color.rgbRed = gray_value;
            color.rgbGreen = gray_value;
            color.rgbBlue = gray_value;
            
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }
    }
    
    if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), 0)) {
        std::cout << "Mapa de transmisión guardado: " << filename << std::endl;
    } else {
        std::cerr << "Error guardando mapa de transmisión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
} 