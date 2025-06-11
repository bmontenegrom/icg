#pragma once
#include "Entity.h"


class Cylinder : public Entity {
public:
	Cylinder(const Vec3& center, double y0, double y1, double radius);

	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;

	void setMaterial(std::shared_ptr<Material> material) override;

private:
	double y0;          ///< Coordenada Y inferior del cilindro
	double y1;          ///< Coordenada Y superior del cilindro
	double radius;      ///< Radio del cilindro
	Vec3 center;      ///< Centro del cilindro 
	std::shared_ptr<Material> material_ptr; ///< Material del cilindro
};

