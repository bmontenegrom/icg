/**
 * @file MaterialMirror.h
 * @brief Definición del material espejo con reflexión perfecta
 *
 * Este material simula un espejo ideal que refleja completamente
 * los rayos incidentes. Utiliza el trazador de Whitted para
 * calcular el color reflejado desde la escena.
 *
 * @author Daniel García
 * @date 11/06/2025
 */

#pragma once
#include "Material.h"
#include "Color.h"
#include "WhittedTracer.h"

 /**
  * @brief Material que representa un espejo con reflexión perfecta.
  */
class MaterialMirror : public Material {
public:
    Color albedo;                    ///< Color base del espejo (tinte de la reflexión)
    const WhittedTracer& tracer;    ///< Referencia al trazador de rayos de Whitted

    /**
     * @brief Constructor del material espejo
     * @param albedo Color base reflejado (generalmente blanco)
     * @param tracer Referencia al WhittedTracer para lanzar rayos reflejados
     */
    MaterialMirror(const Color& albedo, const WhittedTracer& tracer);

    /**
     * @brief Calcula el color reflejado desde la superficie del espejo
     * @param incident_ray Rayo que llega al espejo
     * @param hit_record Información del punto de intersección
     * @param scene Escena completa para trazar rayos reflejados
     * @param depth Profundidad de recursión restante
     * @return Color resultante del rayo reflejado
     */
    Color shade(const Ray& incident_ray, const HitRecord& hit_record,
        const class Scene& scene, int depth) const override;

    double reflectivity() const override { return 1.0; }   ///< Refleja todo
    double transparency() const override { return 0.0; }   ///< No transmite nada
};
