#pragma once
#include "Material.h"
#include "Light.h"
#include <vector>
#include <memory>

/**
 * @brief Material que implementa el modelo de iluminación de Phong
 * 
 * Implementa el modelo de iluminación de Phong con componentes ambiente,
 * difusa y especular. También puede manejar reflexión especular para
 * el algoritmo de Whitted Ray Tracing.
 * 
 * El modelo de Phong calcula la iluminación como:
 * I = I_a * k_a + Σ[I_i * (k_d * (L·N) + k_s * (R·V)^n)]
 * 
 * Referencias:
 * - Phong, B. T. (1975). "Illumination for computer generated pictures"
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class PhongMaterial : public Material {
public:
    /**
     * @brief Constructor para material Phong
     * @param ambient Coeficiente de reflexión ambiente (ka)
     * @param diffuse Coeficiente de reflexión difusa (kd)
     * @param specular Coeficiente de reflexión especular (ks)
     * @param shininess Exponente de Phong (n) - controla el tamaño del highlight
     * @param reflection Coeficiente de reflexión especular para ray tracing
     */
    PhongMaterial(const Color& ambient, const Color& diffuse, const Color& specular, 
                  double shininess, double reflection = 0.0);

    /**
     * @brief Implementa el scattering de Phong con reflexión
     */
    Color scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const override;

    /**
     * @brief Calcula la iluminación local usando el modelo de Phong
     * @param hit_record Información de la intersección
     * @param lights Vector de luces en la escena
     * @param view_direction Dirección hacia el observador
     * @param world Objetos en la escena (para cálculo de sombras)
     * @return Color resultante de la iluminación local
     */
    Color calculateLocalIllumination(const HitRecord& hit_record, 
                                   const std::vector<std::shared_ptr<Light>>& lights,
                                   const Vec3& view_direction,
                                   const Entity& world) const;

    double getReflectionCoefficient() const override { return reflection_coeff; }
    double getTransmissionCoefficient() const override { return 0.0; }

private:
    Color ambient_color;     ///< Color ambiente (ka)
    Color diffuse_color;     ///< Color difuso (kd)
    Color specular_color;    ///< Color especular (ks)
    double shininess;        ///< Exponente de Phong (n)
    double reflection_coeff; ///< Coeficiente de reflexión especular
}; 