/**
 * @file Quad.h
 * @brief Clase que representa un cuadrilátero/rectángulo en el sistema de ray tracing
 * 
 * Esta clase implementa la geometría de un cuadrilátero definido por puntos mínimo y máximo
 * para crear paredes, pisos y superficies planas de la Cornell Box.

 * Referencias:
 * - https://imgeself.github.io/posts/2019-07-20-raytracer-6-rectangles-cornell/
 * 
 * @author Valentin Dutra
 * @date 08/06/2025
 */

#pragma once

#include "Entity.h"
#include "Vec3.h"
#include <memory>

// Forward declaration
class Material;

class Quad : public Entity {
public:
    /**
     * @brief Constructor que inicializa el cuadrilátero con puntos min y max
     * @param minPoint Punto mínimo del rectángulo
     * @param maxPoint Punto máximo del rectángulo
     * @param axis Eje fijo (0=X, 1=Y, 2=Z)
     * @param value Valor del eje fijo
     */
    Quad(const Vec3& minPoint, const Vec3& maxPoint, int axis, double value);
    
    /**
     * @brief Destructor por defecto
     */
    ~Quad() = default;
    
    /**
     * @brief Verifica si un rayo intersecta con el cuadrilátero
     * @param ray El rayo a verificar
     * @param ray_t El intervalo de parámetros del rayo
     * @param rec Registro de intersección
     * @return true si hay intersección, false en caso contrario
     */
    bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;
    
    /**
     * @brief Establece el material del cuadrilátero
     * @param material Puntero compartido al material
     */
    void setMaterial(std::shared_ptr<Material> material) override;
    
private:
    Vec3 minPoint;     ///< Punto mínimo del rectángulo
    Vec3 maxPoint;     ///< Punto máximo del rectángulo
    int fixedAxis;     ///< Eje fijo (0=X, 1=Y, 2=Z)
    double fixedValue; ///< Valor del eje fijo
    std::shared_ptr<Material> material_ptr; ///< Material del cuadrilátero
}; 