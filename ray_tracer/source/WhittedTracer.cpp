/**
 * @file WhittedTracer.cpp
 * @brief Implementación del algoritmo de Ray Tracing de Whitted
 * 
 * Este archivo implementa el algoritmo clásico de ray tracing desarrollado por
 * Turner Whitted en 1980. El algoritmo traza rayos recursivamente para calcular
 * reflexiones y refracciones, produciendo imágenes con iluminación realista.
 * 
 * Características implementadas:
 * - Trazado recursivo de rayos con profundidad configurable
 * - Cálculo de iluminación local usando el modelo de Phong
 * - Generación de rayos de reflexión y refracción
 * - Manejo de sombras con bias para evitar self-shadowing
 * - Generación de mapas de reflexión y transmisión
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "WhittedTracer.h"
#include "Constants.h"
#include "Camera.h"
#include "FreeImage.h"
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Scene.h"
#include "Interval.h"
#include <algorithm>
#include <iostream>
#include <iomanip>  // Para std::put_time
#include <sstream>  // Para std::ostringstream
#include <ctime>    // Para std::time, std::tm

/**
 * @brief Constructor de WhittedTracer
 * @param max_depth Profundidad máxima de recursión para el trazado de rayos
 * @param shadow_bias Valor de bias para evitar self-shadowing
 */
WhittedTracer::WhittedTracer(int max_depth, double shadow_bias)
    : max_depth(max_depth), shadow_bias(shadow_bias) {
}

/**
 * @brief Traza un rayo en la escena y calcula el color resultante
 * @param ray Rayo a trazar
 * @param scene Escena en la que se traza el rayo
 * @param depth Profundidad actual de recursión
 * @return Color resultante del trazado del rayo
 */
Color WhittedTracer::trace(const Ray& ray, const Scene& scene, int depth) const {
    // Verificar límite de profundidad de recursión
    
    
    // Encontrar la intersección más cercana
    HitRecord hit_record;
    if (!scene.hit(ray, Interval(0.001, infinity), hit_record)) {
        // No hay intersección, retornar color de fondo
        return backgroundColor(ray);
    }
    
    // Verificar si el objeto tiene material
    if (!hit_record.material_ptr) {
        // Material por defecto (difuso gris)
        return Color(0.5, 0.5, 0.5);
    }
    
    // Usar el método shade del material para calcular el color
    return hit_record.material_ptr->shade(ray, hit_record, scene, depth);
    
}

/**
 * @brief Calcula el color de fondo para un rayo que no intersecta con ningún objeto
 * @param ray Rayo para el cual calcular el color de fondo
 * @return Color de fondo (gradiente de cielo)
 */
Color WhittedTracer::backgroundColor(const Ray& ray) const {
    // Gradiente azul simple para simular el cielo
    Vec3 unit_direction = unitVector(ray.getDirection());
    double t = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

/**
 * @brief Genera un mapa de reflexión de la escena
 * @param scene Escena para la cual generar el mapa
 * @param camera Cámara desde la cual se genera el mapa
 * @param width Ancho de la imagen resultante
 * @param height Alto de la imagen resultante
 * @param filename Nombre del archivo donde guardar el mapa
 */
void WhittedTracer::generateReflectionMap(const Scene& scene, Camera& camera,
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
            if (scene.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    reflection_coeff = hit_record.material_ptr->reflectivity();
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
    
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return;
    }

    std::ostringstream oss;
    oss << "images/reflection_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";
    
    if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
        std::cout << "Mapa de reflexión guardado: " << oss.str() << std::endl;
    } else {
        std::cerr << "Error guardando mapa de reflexión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}

/**
 * @brief Genera un mapa de transmisión de la escena
 * @param scene Escena para la cual generar el mapa
 * @param camera Cámara desde la cual se genera el mapa
 * @param width Ancho de la imagen resultante
 * @param height Alto de la imagen resultante
 * @param filename Nombre del archivo donde guardar el mapa
 */
void WhittedTracer::generateTransmissionMap(const Scene& scene, Camera& camera,
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
            if (scene.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    transmission_coeff = hit_record.material_ptr->transparency();
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
    
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return;
    }

    std::ostringstream oss;
    oss << "images/transmission_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";
    
    if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
        std::cout << "Mapa de transmisión guardado: " << oss.str() << std::endl;
    } else {
        std::cerr << "Error guardando mapa de transmisión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}
int WhittedTracer::getMaxDepth() const
{
    return max_depth;
}
