#include "Cylinder.h"

Cylinder::Cylinder(const Vec3& center, double y0, double y1, double radius) : center(center), y0(y0), y1(y1), radius(radius) {}

bool Cylinder::hit(const Ray& ray, Interval ray_t, HitRecord& rec) const
{
    Vec3 origin = ray.getOrigin() - center;
    Vec3 dir = ray.getDirection();

    double a = dir.getX() * dir.getX() + dir.getZ() * dir.getZ();
    double b = 2.0 * (origin.getX() * dir.getX() + origin.getZ() * dir.getZ());
    double c = origin.getX() * origin.getX() + origin.getZ() * origin.getZ() - radius * radius;

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return false; 
    }

    double sqrt_d = sqrt(discriminant);
    double t1 = (-b - sqrt_d) / (2 * a);
    double t2 = (-b + sqrt_d) / (2 * a);

    for (double t : {t1, t2}) {
        if (!ray_t.contains(t)) { 
            continue; 
        }
        Vec3 point = ray.pointAtParameter(t);
        if (point.getY() < center.getY() + y0 || point.getY() > center.getY() + y1) { 
            continue;
        }

        rec.t = t;
        rec.point = point;
        Vec3 outward_normal =unitVector(Vec3(point.getX() - center.getX(), 0, point.getZ() - center.getZ()));
        rec.setFaceNormal(ray, outward_normal);
        rec.material_ptr = material_ptr;
        return true;
    }

    // Revisar intersección con tapas (discos)
    for (double y : {y0, y1}) {
        double plane_y = center.getY() + y;
        double t = (plane_y - ray.getOrigin().getY()) / dir.getY();
        if (!ray_t.contains(t)) {
            continue;
        }
        Vec3 p = ray.pointAtParameter(t);
        if ((p.getX() - center.getX()) * (p.getX() - center.getX()) + (p.getZ() - center.getZ()) * (p.getZ() - center.getZ()) > radius * radius) continue;

        rec.t = t;
        rec.point = p;
        Vec3 normal = Vec3(0, (y == y1) ? 1 : -1, 0);
        rec.setFaceNormal(ray, normal);
        rec.material_ptr = material_ptr;
        return true;
    }

    return false;
}


void Cylinder::setMaterial(std::shared_ptr<Material> material)
{
	material_ptr = material;
}



