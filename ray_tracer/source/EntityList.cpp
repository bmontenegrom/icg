#include "EntityList.h"

void EntityList::addEntity(std::shared_ptr<Entity> entity) {
	entities.push_back(entity);
}

void EntityList::clear() {
	entities.clear();
}

bool EntityList::hit(const Ray& ray, Interval ray_t, HitRecord& rec) const {
	HitRecord tempRec;
	bool hitAnything = false;
	double closestSoFar = ray_t.getMax();
	for (const auto& entity : entities) {
		if (entity->hit(ray, Interval(ray_t.getMin(), closestSoFar), tempRec)) {
			hitAnything = true;
			closestSoFar = tempRec.t;
			rec = tempRec; // Update the record with the closest hit
		}
	}
	return hitAnything;
}