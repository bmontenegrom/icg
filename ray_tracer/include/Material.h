#pragma once
#include "Color.h"
#include "Ray.h"
#include "HitRecord.h"
#include "Vec3.h"

/**
 * @brief Clase base abstracta para materiales en el sistema de ray tracing
 * 
 * Define la interfaz común para todos los tipos de materiales que pueden
 * aplicarse a objetos en la escena. Implementa el modelo de iluminación
 * de Whitted que incluye componentes ambiente, difusa, especular, reflexión
 * y refracción.
 * 
 * Referencias:
 * - Whitted, T. (1980). "An improved illumination model for shaded display"
 * - https://courses.cs.washington.edu/courses/cse557/06wi/projects/ray/handout.html
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */
class Material {
public:

    /**
     * @brief Calcula el vector de reflexión especular
     * 
     * No es virtual porque la reflexión especular sigue la ley de reflexión
     * R = I - 2(N·I)N que es igual para todos los materiales. Los materiales
     * específicos pueden variar cómo usan esta reflexión pero el cálculo del
     * vector reflejado es siempre el mismo.
     * 
     * @param incident Vector incidente (debe estar normalizado)
     * @param normal Vector normal a la superficie (debe estar normalizado)
     * @return Vector de reflexión normalizado
     */
    Vec3 reflect(const Vec3& incident, const Vec3& normal) const;

    /**
     * @brief Destructor virtual por defecto
     */
    virtual ~Material() = default;

    /**
     * @brief Calcula el color resultante de la interacción de un rayo con el material
     * 
     * Implementa el modelo de iluminación de Whitted, calculando los componentes
     * de iluminación local (ambiente, difusa, especular) y generando rayos
     * secundarios para reflexión y refracción según corresponda.
     * 
     * @param incident_ray Rayo incidente que golpea la superficie
     * @param hit_record Información de la intersección
     * @param depth Profundidad actual de recursión
     * @return Color resultante de la interacción
     */
    virtual Color shade(const Ray& incident_ray, const HitRecord& hit_record, int depth) const = 0;

    /**
     * @brief Obtiene el coeficiente de reflexión del material
     * @return Valor entre 0.0 (no reflectante) y 1.0 (totalmente reflectante)
     */
    virtual double reflectivity() const { return 0.0; }

    /**
     * @brief Obtiene el coeficiente de transmisión del material
     * @return Valor entre 0.0 (opaco) y 1.0 (totalmente transparente)
     */
    virtual double transparency() const { return 0.0; }
}; 