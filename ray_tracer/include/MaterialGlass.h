/*
 * @file MaterialGlass.h
 * @brief Definici�n del material transparente con reflexi�n y refracci�n (vidrio)
 *
 * Este material implementa la f�sica de materiales diel�ctricos (como el vidrio),
 * utilizando:
 * - Refracci�n seg�n la ley de Snell
 * - Reflexi�n especular basada en Fresnel (aproximaci�n de Schlick)
 * - Refracci�n interna con verificaci�n de reflexi�n total
 * - Combinaci�n ponderada entre reflexi�n y transmisi�n
 *
 * El albedo representa el tinte del material, y el �ndice de refracci�n (ior)
 * determina el comportamiento de la desviaci�n de los rayos.
 *
 * Referencias:
 * - Turner Whitted, "An Improved Illumination Model for Shaded Display" (1980)
 * - https://en.wikipedia.org/wiki/Fresnel_equations
 * - https://www.scratchapixel.com/lessons/3d-basic-rendering/reflection-refraction-fresnel
 *
 * @author Daniel Garc�a
 * @date 08/06/2025
 */

#pragma once
#include "Material.h"
#include "Color.h"
#include "WhittedTracer.h"

/**
 * @brief Material que representa un vidrio con refracci�n y reflexi�n combinadas.
 */
class MaterialGlass : public Material {
public:
    Color albedo;   // Tinte del material (color que multiplica la luz transmitida/reflejada)
    double ior;     //  �ndice de refracci�n del material
    /**
   * @brief Constructor del material transparente
   * @param albedo Color base del vidrio (tinte)
   * @param ior �ndice de refracci�n
   * @param tracer Referencia al trazador de rayos Whitted para lanzar rayos secundarios
   */
    MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer );

    MaterialGlass(const Color& albedo, double ior, const WhittedTracer& tracer, double reflectivity, double transparency);
    /**
     * @brief Calcula el color resultante aplicando refracci�n o reflexi�n
     * @param incident_ray Rayo que incide sobre la superficie
     * @param hit_record Informaci�n de la intersecci�n
     * @param scene Escena completa
     * @param depth Profundidad de recursi�n restante
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
