#pragma once
#include "Material.h"
#include "Color.h"
#include "Vec3.h"
#include "Ray.h"
#include "HitRecord.h"

/**
 * @brief Material que implementa el modelo de iluminación de Phong
 * 
 * Implementa el modelo completo de Phong que incluye tres componentes:
 * 1. Ambiente: Iluminación global uniforme
 * 2. Difusa: Reflexión Lambertiana (superficies mate)
 * 3. Especular: Reflexión direccional (superficies brillantes)
 * 
 * El nombre "LambertianMaterial" se refiere a que la componente difusa
 * sigue la ley de Lambert, pero el material completo implementa Phong.
 * 
 * Ecuación de Phong:
 * I = k_a*i_a + Σ[k_d*(N·L)*i_d + k_s*(R·V)^α*i_s]
 * 
 * Donde:
 * - k_a, k_d, k_s: coeficientes ambiente, difuso y especular
 * - N·L: producto punto normal-luz (componente Lambertiana)
 * - R·V: producto punto reflexión-vista elevado a α (componente especular)
 * - La componente difusa se normaliza por π para conservación de energía
 * 
 * Referencias:
 * - Bui Tuong Phong, "Illumination for computer generated pictures" (1975)
 * - https://en.wikipedia.org/wiki/Phong_reflection_model
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class LambertianMaterial : public Material {
public:
    Color ambient;      ///< Coeficiente de reflexión ambiental (k_a)
    Color diffuse;      ///< Coeficiente de reflexión difusa Lambertiana (k_d)
    Color specular;     ///< Coeficiente de reflexión especular (k_s)
    double shininess;   ///< Exponente especular (α) - controla el tamaño del highlight

    /**
     * @brief Constructor del material de Phong
     * @param a Color/coeficiente ambiental
     * @param d Color/coeficiente difuso (Lambertiano)
     * @param s Color/coeficiente especular
     * @param sh Exponente de brillo especular (mayor = más brillante)
     */
    LambertianMaterial(const Color& a, const Color& d, const Color& s = Color(0,0,0), double sh = 0.0);

    /**
     * @brief Calcula el color resultante usando el modelo completo de Phong
     * 
     * Implementa la ecuación de Phong:
     * color = ambiente + Σ[difusa_normalizada*(N·L) + especular*(R·V)^α] * luz
     * 
     * La componente difusa sigue la ley de Lambert normalizada por π.
     * La componente especular usa el vector de reflexión perfecta.
     * 
     * @param r_in Rayo incidente
     * @param rec Información de la intersección
     * @param scene Escena con objetos y luces
     * @param depth Profundidad actual de recursión
     * @return Color resultante
     */
    Color shade(const Ray& r_in, const HitRecord& rec, const class Scene& scene, int depth) const override;
    //double transparency() const;
}; 