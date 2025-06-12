#include "Triangle.h"

Triangle::Triangle(const Vec3& a, const Vec3& b, const Vec3& c, std::shared_ptr<Material> m)
    : v0(a), v1(b), v2(c), material_ptr(m) {
    normal = unitVector(crossProduct(v1 - v0, v2 - v0));
    Vec3 min_point(
        std::min({ v0.getX(), v1.getX(), v2.getX() }),
        std::min({ v0.getY(), v1.getY(), v2.getY() }),
        std::min({ v0.getZ(), v1.getZ(), v2.getZ() })
    );
    Vec3 max_point(
        std::max({ v0.getX(), v1.getX(), v2.getX() }),
        std::max({ v0.getY(), v1.getY(), v2.getY() }),
        std::max({ v0.getZ(), v1.getZ(), v2.getZ() })
    );
    box = AABB(min_point, max_point);
}

bool Triangle::hit(const Ray& r, Interval t, HitRecord& rec) const {
    const double EPSILON = 1e-8;
    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;
    Vec3 h = crossProduct(r.getDirection(), edge2);
    double a = dotProduct(edge1, h);
    if (fabs(a) < EPSILON)
        return false;

    double f = 1.0 / a;
    Vec3 s = r.getOrigin() - v0;
    double u = f * dotProduct(s, h);
    if (u < 0.0 || u > 1.0)
        return false;

    Vec3 q = crossProduct(s, edge1);
    double v = f * dotProduct(r.getDirection(), q);
    if (v < 0.0 || u + v > 1.0)
        return false;

    double t_hit = f * dotProduct(edge2, q);
    if (!t.surrounds(t_hit))
        return false;

    rec.t = t_hit;
    rec.point = r.pointAtParameter(t_hit);
    rec.setFaceNormal(r, normal);
    rec.material_ptr = material_ptr;
	//std::cout << "Hit triangle at t = " << rec.t << std::endl;
    return true;
}

AABB Triangle::boundingBox() const {
    return box;
}

Vec3 Triangle::getV0() const
{
    return v0;
}

Vec3 Triangle::getV1() const
{
    return v1;
}

Vec3 Triangle::getV2() const
{
    return v2;
}

Vec3 Triangle::getNormal() const
{
    return normal;
}

std::shared_ptr<Material> Triangle::getMaterial() const
{
    return material_ptr;
}

void Triangle::setMaterial(std::shared_ptr<Material> material)
{
	material_ptr = material;
}
