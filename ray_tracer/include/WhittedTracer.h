#pragma once
#include "Color.h"
#include "Ray.h"
#include "Entity.h"
#include "Light.h"
#include "Material.h"
#include "Scene.h"
#include <vector>
#include <memory>

/**
 * @brief Implementación del algoritmo de Ray Tracing de Whitted
 * 
 * Esta clase implementa el algoritmo clásico de ray tracing desarrollado por
 * Turner Whitted en 1980. El algoritmo traza rayos recursivamente para calcular
 * reflexiones y refracciones, produciendo imágenes con iluminación realista.
 * 
 * El algoritmo funciona de la siguiente manera:
 * 1. Para cada pixel, lanza un rayo primario desde la cámara
 * 2. Encuentra la intersección más cercana con objetos
 * 3. Calcula iluminación local usando el modelo de Phong
 * 4. Genera rayos secundarios para reflexión y refracción
 * 5. Combina recursivamente todos los componentes de color
 * 
 * Referencias:
 * - Whitted, T. (1980). "An improved illumination model for shaded display"
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * - https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-overview/
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class WhittedTracer {
public:
    /**
     * @brief Constructor del trazador de Whitted
     * @param max_depth Profundidad máxima de recursión
     * @param shadow_bias Pequeño offset para evitar self-shadowing
     */
    WhittedTracer(int max_depth = 10, double shadow_bias = 0.001);



    /**
     * @brief Traza un rayo y calcula el color resultante
     * 
     * Implementa el algoritmo principal de Whitted Ray Tracing:
     * - Encuentra intersección más cercana
     * - Calcula iluminación local usando el método shade del material
     * - Genera y traza rayos de reflexión
     * - Genera y traza rayos de refracción
     * - Combina todos los componentes
     * 
     * @param ray Rayo a trazar
     * @param scene Escena con objetos y luces
     * @param depth Profundidad actual de recursión
     * @return Color calculado para el rayo
     */
    Color trace(const Ray& ray, const Scene& scene, int depth = 0) const;

    /**
     * @brief Genera imagen auxiliar de coeficientes de reflexión
     * @param scene Escena con objetos y luces
     * @param camera Cámara para generar rayos
     * @param width Ancho de la imagen
     * @param height Alto de la imagen
     * @param filename Nombre del archivo de salida
     */
    void generateReflectionMap(const Scene& scene, class Camera& camera, 
                              int width, int height, const std::string& filename) const;

    /**
     * @brief Genera imagen auxiliar de coeficientes de transmisión
     * @param scene Escena con objetos y luces
     * @param camera Cámara para generar rayos
     * @param width Ancho de la imagen
     * @param height Alto de la imagen
     * @param filename Nombre del archivo de salida
     */
    void generateTransmissionMap(const Scene& scene, class Camera& camera,
                                int width, int height, const std::string& filename) const;

    int getMaxDepth() const;

private:
    int max_depth;      ///< Profundidad máxima de recursión
    double shadow_bias; ///< Offset para evitar self-shadowing

    /**
     * @brief Color de fondo cuando no hay intersección
     * @param ray Rayo que no intersectó ningún objeto
     * @return Color de fondo
     */
    Color backgroundColor(const Ray& ray) const;
}; 