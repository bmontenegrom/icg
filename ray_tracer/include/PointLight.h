#pragma once
#include "Light.h"
#include "Vec3.h"
#include "Color.h"

/**
 * @brief Implementación de una fuente de luz puntual
 * 
 * Representa una fuente de luz omnidireccional localizada en un punto específico
 * del espacio, con intensidad que puede variar con la distancia.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class PointLight : public Light {
public:
    Vec3 position;      ///< Posición de la luz en el espacio
    Color intensity;    ///< Intensidad/color de la luz
    
    /**
     * @brief Constructor
     * @param pos Posición de la luz
     * @param color Intensidad/color de la luz
     */
    PointLight(const Vec3& pos, const Color& color);
    
    /**
     * @brief Calcula la intensidad de luz que llega a un punto
     * @param point Punto donde se calcula la iluminación
     * @return Intensidad de la luz en ese punto
     */
    Color getIntensity(const Vec3& point) const override;
    
    /**
     * @brief Obtiene la dirección desde un punto hacia la fuente de luz
     * @param point Punto desde el cual se calcula la dirección
     * @return Vector normalizado hacia la fuente de luz
     */
    Vec3 getDirection(const Vec3& point) const override;
    
    /**
     * @brief Calcula la distancia desde un punto hasta la fuente de luz
     * @param point Punto desde el cual se calcula la distancia
     * @return Distancia a la fuente de luz
     */
    double getDistance(const Vec3& point) const override;
    
    /**
     * @brief Verifica si un punto está en sombra respecto a esta luz
     * @param point Punto a verificar
     * @param world Lista de objetos en la escena
     * @return true si el punto está en sombra
     */
    bool isInShadow(const Vec3& point, const Entity& world) const override;
    
    /**
     * @brief Obtiene el color base de la luz
     * @return Color de la fuente luminosa
     */
    Color getColor() const override;
}; 