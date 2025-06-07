#pragma once
#include "Light.h"

/**
 * @brief Implementación de una luz puntual
 * 
 * Simula una fuente de luz puntual que emite luz uniformemente en todas
 * las direcciones desde una posición específica. La intensidad disminuye
 * con el cuadrado de la distancia (atenuación cuadrática).
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class PointLight : public Light {
public:
    /**
     * @brief Constructor para luz puntual
     * @param position Posición de la luz en el espacio 3D
     * @param color Color/intensidad de la luz
     * @param constant_attenuation Factor de atenuación constante
     * @param linear_attenuation Factor de atenuación lineal
     * @param quadratic_attenuation Factor de atenuación cuadrática
     */
    PointLight(const Vec3& position, const Color& color, 
               double constant_attenuation = 1.0,
               double linear_attenuation = 0.0,
               double quadratic_attenuation = 0.0);

    Color getIntensity(const Vec3& point) const override;
    Vec3 getDirection(const Vec3& point) const override;
    double getDistance(const Vec3& point) const override;
    bool isInShadow(const Vec3& point, const Entity& world) const override;
    Color getColor() const override { return light_color; }

private:
    Vec3 position;                  ///< Posición de la luz
    Color light_color;              ///< Color de la luz
    double const_attenuation;       ///< Atenuación constante
    double linear_attenuation;      ///< Atenuación lineal
    double quadratic_attenuation;   ///< Atenuación cuadrática
}; 