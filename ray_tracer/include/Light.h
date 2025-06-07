#pragma once
#include "Vec3.h"
#include "Color.h"
#include "Entity.h"

/**
 * @brief Clase base abstracta para fuentes de luz en el ray tracer
 * 
 * Define la interfaz común para todos los tipos de luces utilizados
 * en el modelo de iluminación de Whitted. Soporta cálculo de sombras
 * y diferentes tipos de fuentes luminosas.
 * 
 * Referencias:
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class Light {
public:
    /**
     * @brief Destructor virtual por defecto
     */
    virtual ~Light() = default;

    /**
     * @brief Calcula la intensidad de luz que llega a un punto
     * @param point Punto donde se calcula la iluminación
     * @return Color/intensidad de la luz en ese punto
     */
    virtual Color getIntensity(const Vec3& point) const = 0;

    /**
     * @brief Obtiene la dirección desde un punto hacia la fuente de luz
     * @param point Punto desde el cual se calcula la dirección
     * @return Vector normalizado hacia la fuente de luz
     */
    virtual Vec3 getDirection(const Vec3& point) const = 0;

    /**
     * @brief Calcula la distancia desde un punto hasta la fuente de luz
     * @param point Punto desde el cual se calcula la distancia
     * @return Distancia a la fuente de luz
     */
    virtual double getDistance(const Vec3& point) const = 0;

    /**
     * @brief Verifica si un punto está en sombra respecto a esta luz
     * @param point Punto a verificar
     * @param world Lista de objetos en la escena
     * @return true si el punto está en sombra, false si está iluminado
     */
    virtual bool isInShadow(const Vec3& point, const Entity& world) const = 0;

    /**
     * @brief Obtiene el color base de la luz
     * @return Color de la fuente luminosa
     */
    virtual Color getColor() const = 0;
}; 