#pragma once
#include "Light.h"

/**
 * @brief Implementación de una luz direccional
 * 
 * Simula una fuente de luz distante (como el sol) que emite rayos paralelos
 * en una dirección específica. No tiene atenuación por distancia.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class DirectionalLight : public Light {
public:
    /**
     * @brief Constructor para luz direccional
     * @param direction Dirección de los rayos de luz (será normalizada)
     * @param color Color/intensidad de la luz
     */
    DirectionalLight(const Vec3& direction, const Color& color);

    /**
     * @brief Obtiene la intensidad de la luz en un punto
     * @param point Punto donde se calcula la intensidad
     * @return Intensidad de la luz en ese punto
     */
    Color getIntensity(const Vec3& point) const override;

    /**
     * @brief Obtiene la dirección de la luz en un punto
     * @param point Punto donde se calcula la dirección
     * @return Dirección de la luz en ese punto
     */
    Vec3 getDirection(const Vec3& point) const override;

    /**
     * @brief Obtiene la distancia desde un punto hasta la fuente de luz
     * @param point Punto desde el cual se calcula la distancia
     * @return Distancia a la fuente de luz
     */
    double getDistance(const Vec3& point) const override;

    /**
     * @brief Verifica si un punto está en sombra respecto a esta luz
     * @param point Punto a verificar
     * @param world Lista de objetos en la escena
     * @return true si el punto está en sombra, false si está iluminado
     */
    bool isInShadow(const Vec3& point, const Entity& world) const override;

    /**
     * @brief Obtiene el color de la luz
     * @return Color de la luz
     */
    Color getColor() const override { return light_color; }

private:
    Vec3 direction;        ///< Dirección normalizada de la luz
    Color light_color;     ///< Color de la luz
}; 