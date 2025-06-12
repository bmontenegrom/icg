#include "Mesh.h"

Mesh::Mesh(const std::vector<Vec3>& vertices_raw, const std::vector<std::array<int, 3>>& indices, std::shared_ptr<Material> mat, const Vec3& scale, const Vec3& translate) {
    std::vector<Vec3> vertices;
    vertices.reserve(vertices_raw.size());

    for (const auto& v : vertices_raw) {
        vertices.emplace_back(v.getX() * scale.getX() + translate.getX(),
            v.getY() * scale.getY() + translate.getY(),
            v.getZ() * scale.getZ() + translate.getZ());
    }

    bool first = true;
    for (const auto& idx : indices) {
        auto tri = std::make_shared<Triangle>(vertices[idx[0]], vertices[idx[1]], vertices[idx[2]], mat);
        triangles.push_back(tri);

        AABB tri_box = tri->boundingBox();
        if (first) {
            bounding_box = tri_box;
            first = false;
        }
        else {
            bounding_box.expandToInclude(tri_box);
        }
    }
}

bool Mesh::hit(const Ray& r, Interval t, HitRecord& rec) const {
    if (!bounding_box.hit(r, t)) return false;

    bool hit_anything = false;
    auto closest_so_far = t.getMax();

    HitRecord temp_rec;
    for (const auto& tri : triangles) {
        if (tri->hit(r, Interval(t.getMin(), closest_so_far), temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

AABB Mesh::boundingBox() const {
    return bounding_box;
}

void Mesh::setMaterial(std::shared_ptr<Material> material)
{
	for (auto& tri : triangles) {
		tri->setMaterial(material);
	}
}
