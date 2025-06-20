#pragma once

#include "Material.h"
#include "Texture.h"
#include "Color.h"
#include "Constants.h"

/**
 * @brief Material Lambertiano con normal mapping. Usa colores fijos pero modifica la normal con una textura.
 */
class MaterialNormalMapped : public Material {
public:
    MaterialNormalMapped(const Color& ambient,
        const Color& diffuse,
        const Color& specular,
        float shininess,
        const Texture& normalMap);

    Color shade(const Ray& r_in, const HitRecord& rec, const Scene& scene, int depth) const override;
    Color shadeComponent(ShadeComponent component,
        const Ray& r_in,
        const HitRecord& rec,
        const Scene& scene) const override;


private:
    Color ambient;
    Color diffuse;
    Color specular;
    float shininess;
    Texture normalMap;

    Vec3 perturbNormal(const HitRecord& rec) const;
};