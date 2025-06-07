/**
 * @file MirrorMaterial.cpp
 * @brief Implementación del material espejo para superficies perfectamente reflectantes
 * 
 * Implementa un material que refleja perfectamente los rayos incidentes según
 * la ley de reflexión especular, utilizado en el algoritmo de Whitted Ray Tracing.
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "MirrorMaterial.h"
#include "Constants.h"
#include "Interval.h"
#include <algorithm>

MirrorMaterial::MirrorMaterial(double reflectance, const Color& tint)
    : reflectance(Interval(0.0, 1.0).clamp(reflectance)), tint(tint) {
}

Color MirrorMaterial::scatter(const Ray& incident_ray, const HitRecord& hit_record, int depth) const {
    // Para un espejo perfecto, todo el color viene de la reflexión
    // Este método debe ser llamado por WhittedTracer que manejará la recursión
    
    // El color base del espejo es negro ya que no emite ni absorbe luz,
    // solo refleja. El color real vendrá del rayo reflejado.
    return Color(0, 0, 0);
}

// Nota: Los métodos de reflexión y trazado recursivo serían manejados
// por la clase WhittedTracer que tiene acceso a la escena completa 