#include "AABB.h"
#include <algorithm>
#include <cmath>

AABB::AABB() = default;

AABB::AABB(const Vec3& min_point, const Vec3& max_point)
    : minimum(min_point), maximum(max_point) {
}

bool AABB::hit(const Ray& r, const Interval& ray_t) const {
    for (int axis = 0; axis < 3; ++axis) {
        double originComponent, directionComponent, minComponent, maxComponent;
        switch (axis) {
        case 0:
            originComponent = r.getOrigin().getX();
            directionComponent = r.getDirection().getX();
            minComponent = minimum.getX();
            maxComponent = maximum.getX();
            break;
        case 1:
            originComponent = r.getOrigin().getY();
            directionComponent = r.getDirection().getY();
            minComponent = minimum.getY();
            maxComponent = maximum.getY();
            break;
        case 2:
            originComponent = r.getOrigin().getZ();
            directionComponent = r.getDirection().getZ();
            minComponent = minimum.getZ();
            maxComponent = maximum.getZ();
            break;
        }

        double invD = 1.0 / directionComponent;
        double t0 = (minComponent - originComponent) * invD;
        double t1 = (maxComponent - originComponent) * invD;
        if (invD < 0.0) {
            std::swap(t0, t1);
        }
        if (t1 < ray_t.getMin() || t0 > ray_t.getMax()) {
            return false;
        }
    }
    return true;
}

AABB AABB::surroundingBox(const AABB& box0, const AABB& box1) {
    Vec3 small(
        std::fmin(box0.minimum.getX(), box1.minimum.getX()),
        std::fmin(box0.minimum.getY(), box1.minimum.getY()),
        std::fmin(box0.minimum.getZ(), box1.minimum.getZ()));
    Vec3 big(
        std::fmax(box0.maximum.getX(), box1.maximum.getX()),
        std::fmax(box0.maximum.getY(), box1.maximum.getY()),
        std::fmax(box0.maximum.getZ(), box1.maximum.getZ()));
    return AABB(small, big);
}

void AABB::expandToInclude(const AABB& other) {
    minimum = Vec3(
        std::fmin(minimum.getX(), other.minimum.getX()),
        std::fmin(minimum.getY(), other.minimum.getY()),
        std::fmin(minimum.getZ(), other.minimum.getZ()));
    maximum = Vec3(
        std::fmax(maximum.getX(), other.maximum.getX()),
        std::fmax(maximum.getY(), other.maximum.getY()),
        std::fmax(maximum.getZ(), other.maximum.getZ()));
}

Vec3 AABB::getMin() const
{
    return minimum;
}

Vec3 AABB::getMax() const
{
    return maximum;
}
