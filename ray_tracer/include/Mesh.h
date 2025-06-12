#pragma once

#include <vector>
#include <memory>
#include <array>
#include "Entity.h"
#include "Vec3.h"
#include "Material.h"
#include "AABB.h"
#include "Triangle.h"

class Mesh : public Entity {
public:
    Mesh(const std::vector<Vec3>& vertices, const std::vector<std::array<int, 3>>& indices, std::shared_ptr<Material> mat, const Vec3& scale = Vec3(1, 1, 1), const Vec3& translate = Vec3(0, 0, 0));

    bool hit(const Ray& r, Interval t, HitRecord& rec) const override;
    AABB boundingBox() const;

    void setMaterial(std::shared_ptr<Material> material) override;

private:
    std::vector<std::shared_ptr<Triangle>> triangles;
    AABB bounding_box;
};
