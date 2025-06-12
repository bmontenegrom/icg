#pragma once

#include "Vec3.h"
#include "Ray.h"
#include "Interval.h"

class AABB {
public:
    AABB();
    AABB(const Vec3& min_point, const Vec3& max_point);

    bool hit(const Ray& r, const Interval& ray_t) const;
    static AABB surroundingBox(const AABB& box0, const AABB& box1);
    void expandToInclude(const AABB& other);

    Vec3 getMin() const;
    Vec3 getMax() const;

private:
    Vec3 minimum;
    Vec3 maximum;
};