#pragma once  
#include "Ray.h"  
#include "HitRecord.h"
#include "Interval.h"

class Entity {  
public:  
virtual ~Entity() = default;  

virtual bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const = 0;  
};