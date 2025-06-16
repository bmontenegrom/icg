#pragma once

#include "Material.h"
#include "Texture.h"
#include "Color.h"
#include "Constants.h" 

class MaterialTextured : public Material {
public:
    MaterialTextured(const Texture& texture, double shininess);

    Color shade(const Ray& r_in, const HitRecord& rec, const Scene& scene, int depth) const override;
    Color shadeComponent(ShadeComponent component,
        const Ray& r_in,
        const HitRecord& rec,
        const Scene& scene) const override;

private:
    Texture texture;
    double shininess;
};