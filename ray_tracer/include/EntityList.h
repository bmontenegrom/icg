#pragma once

#include <memory>
#include <vector>
#include "Entity.h"

class EntityList : public Entity {
public:
	// Constructor
	EntityList() = default;
	// Destructor
	~EntityList() override = default;
	// Add an entity to the list
	void addEntity(std::shared_ptr<Entity> entity);
	
	void clear();

	bool hit(const Ray& ray, Interval ray_t, HitRecord& rec) const override;

private:
	std::vector<std::shared_ptr<Entity>> entities; // List of entities
};