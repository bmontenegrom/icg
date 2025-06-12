#include "Vec3.h"
#include "Ray.h"
#include "Interval.h"
#include "Material.h"
#include "HitRecord.h"
#include "Entity.h"
#include "AABB.h"
#include <memory>

class Triangle : public Entity {
public:
    Triangle(const Vec3& a, const Vec3& b, const Vec3& c, std::shared_ptr<Material> m);

    bool hit(const Ray& r, Interval t, HitRecord& rec) const override;
    AABB boundingBox() const;

    Vec3 getV0() const;
    Vec3 getV1() const;
    Vec3 getV2() const;
    Vec3 getNormal() const;
    std::shared_ptr<Material> getMaterial() const;
    void setMaterial(std::shared_ptr<Material> material) override;

private:
    Vec3 v0, v1, v2;
    Vec3 normal;
    std::shared_ptr<Material> material_ptr;
    AABB box;
};
