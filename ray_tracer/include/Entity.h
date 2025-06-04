#pragma once  
#include "Ray.h"  
#include "HitRecord.h"  

class Entity {  
public:  
virtual ~Entity() = default;  

virtual bool hit(const Ray& ray, double tMin, double tMax, HitRecord& rec) const = 0;  
};