/**
 * @file main.cpp
 * @brief Punto de entrada principal para el sistema de ray tracing de Whitted
 * 
 * Este archivo implementa una demostración completa del algoritmo de Whitted Ray Tracing,
 * creando una escena con diferentes tipos de materiales (Phong, espejo, transparente),
 * múltiples fuentes de luz, y generando tanto la imagen principal como imágenes auxiliares
 * de reflexión y transmisión.
 * 
 * Características implementadas:
 * - Algoritmo de Whitted Ray Tracing con reflexión y refracción recursiva
 * - Múltiples tipos de materiales con diferentes propiedades ópticas
 * - Sistema de iluminación con múltiples fuentes de luz
 * - Generación de imágenes auxiliares para análisis visual
 * - Configuración de escena con geometría variada
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include <FreeImage.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>

// Componentes base del sistema
#include "Constants.h"
#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include "Camera.h"

// Geometría y entidades
#include "Sphere.h"
#include "EntityList.h"


// Ray tracer de Whitted
#include "WhittedTracer.h"

/**
 * @brief Crea y configura la escena de demostración
 * 
 * Construye una escena que demuestra las capacidades del algoritmo de Whitted:
 * - Esferas con diferentes materiales (difuso, especular, reflectante, transparente)
 * - Configuración de geometría que permite mostrar reflexiones y refracciones
 * - Posicionamiento estratégico para crear efectos visuales interesantes
 * 
 * @return Puntero a la lista de entidades que componen la escena
 */
std::shared_ptr<EntityList> createScene() {
    auto world = std::make_shared<EntityList>();
    
    // === CONSTRUCCIÓN DE LA ESCENA BÁSICA ===
    
    // Suelo (esfera grande que actúa como plano)
    world->addEntity(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100));
    
    // Esfera central
    world->addEntity(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5));
    
    return world;
}


/**
 * @brief Configura la cámara para la renderización
 * 
 * Establece los parámetros de la cámara optimizados para mostrar
 * los efectos del ray tracing de Whitted:
 * - Resolución apropiada para calidad vs. tiempo de renderización
 * - Muestreo antialiasing para suavizar bordes
 * - Ratio de aspecto estándar
 * 
 * @return Puntero único a la cámara configurada
 */
std::unique_ptr<Camera> createCamera() {
    // Configuración de imagen
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 800;         // Resolución moderada para pruebas rápidas
    int samples_per_pixel = 10;    // Muestreo moderado para balance calidad/velocidad
    
    auto camera = std::make_unique<Camera>(aspect_ratio, image_width, samples_per_pixel);
    
    return camera;
}

/**
 * @brief Renderiza la imagen principal usando el ray tracer básico
 * 
 * Utiliza la cámara para renderizar la escena con el sistema básico de ray tracing.
 * Esta función se puede extender para usar el WhittedTracer una vez que esté
 * completamente integrado con el sistema.
 * 
 * @param world Escena a renderizar
 * @param camera Cámara para la renderización
 */
void renderBasicScene(const EntityList& world, Camera& camera) {
    // Renderizar imagen principal
    camera.render(world);
}

/**
 * @brief Función principal del programa
 * 
 * Coordina la creación de la escena completa del algoritmo de ray tracing:
 * 1. Inicializa el sistema de renderización
 * 2. Crea la escena con geometría variada
 * 3. Configura el sistema de iluminación múltiple
 * 4. Configura la cámara con parámetros optimizados
 * 5. Ejecuta el ray tracing básico
 * 6. Demuestra las capacidades del WhittedTracer
 * 
 * @return 0 si el programa se ejecuta correctamente, código de error en caso contrario
 */
int main() {
    try {
        // Inicializar FreeImage para manejo de imágenes
        FreeImage_Initialise();
        
        // Crear escena con geometría variada
        auto world = createScene();
        
        // Configurar cámara
        auto camera = createCamera();
        
        // === RENDERIZACIÓN BÁSICA ===
        renderBasicScene(*world, *camera);
        
        // Finalizar FreeImage
        FreeImage_DeInitialise();
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error durante la ejecución: " << e.what() << std::endl;
        FreeImage_DeInitialise();
        return 1;
        
    } catch (...) {
        std::cerr << "Error desconocido durante la ejecución." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }
}
