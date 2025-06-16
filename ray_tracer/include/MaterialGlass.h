/*
 * @file MaterialGlass.h
 * @brief Definición del material transparente con reflexión y refracción (vidrio)
 *
 * Este material implementa la física de materiales dieléctricos (como el vidrio),
 * utilizando:
 * - Refracción según la ley de Snell
 * - Reflexión especular basada en Fresnel (aproximación de Schlick)
 * - Refracción interna con verificación de reflexión total
 * - Combinación ponderada entre reflexión y transmisión
 *
 * El albedo representa el tinte del material, y el índice de refracción (ior)
 * determina el comportamiento de la desviación de los rayos.
 *
 * Referencias:
 * - Turner Whitted, "An Improved Illumination Model for Shaded Display" (1980)
 * - https://en.wikipedia.org/wiki/Fresnel_equations
 * - https://www.scratchapixel.com/lessons/3d-basic-rendering/reflection-refraction-fresnel
 *
 * @author Daniel García
 * @date 08/06/2025
 */

#pragma once
#include "Material.h"
#include "Color.h"
#include "WhittedTracer.h"

/**
 * @brief Material que representa un vidrio con refracción y reflexión combinadas.
 */
class MaterialGlass : public Material {
public:
    Color albedo;   // Tinte del material (color que multiplica la luz transmitida/reflejada)
    double ior;     //  Índice de refracción del material
    /**
   * @brief Constructor del material transparente
   * @param albedo Color base del vidrio (tinte)
   * @param ior Índice de refracción
   * @param tracer Referencia al trazador de rayos Whitted para lanzar rayos secundarios
   */
    MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer );

    MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer, double reflectivity, double transparency);
    /**
     * @brief Calcula el color resultante aplicando refracción o reflexión
     * @param incident_ray Rayo que incide sobre la superficie
     * @param hit_record Información de la intersección
     * @param scene Escena completa
     * @param depth Profundidad de recursión restante
     * @return Color resultante del rayo refractado o reflejado
     */
    Color shade(const Ray& incident_ray, const HitRecord& hit_record, const class Scene& scene, int depth) const override;

    double getReflectivity() const override;     // Ajustable
    double getTransparency() const override;   // Mayormente transparente

    virtual Color shadeComponent(ShadeComponent component,
        const Ray& ray,
        const HitRecord& hit,
        const Scene& scene) const override;
private:
    const WhittedTracer& tracer;
    double reflectivity;
	double transparency; 
};
