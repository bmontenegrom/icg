#pragma once  
#include "Ray.h"  
#include "HitRecord.h"
#include "Interval.h"

/**
 * @brief Clase base abstracta para todas las entidades geométricas en el ray tracer
 * 
 * Esta clase define la interfaz común para todos los objetos 3D que pueden
 * ser intersectados por rayos en el proceso de ray tracing. Incluye primitivas
 * como esferas, planos, triángulos, etc.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */
class Entity {  
public:  
    /**
     * @brief Destructor virtual por defecto
     */
    virtual ~Entity() = default;  

    /**
     * @brief Calcula la intersección entre un rayo y la entidad
     * 
     * Determina si un rayo intersecta con esta entidad geométrica dentro
     * del intervalo de parámetros especificado. Si hay intersección,
     * almacena la información relevante en el HitRecord.
     * 
     * @param ray El rayo a testear para intersección
     * @param ray_t Intervalo de parámetros t válidos para la intersección
     * @param rec Registro donde se almacena información de la intersección
     * @return true si hay intersección, false en caso contrario
     */
    virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const = 0;  
};