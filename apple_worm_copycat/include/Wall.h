#include "Entity.h"

class Wall : public Entity {
public:
	Wall(double x, double y, double z, double width, double height, double depth);
	void render() override;
};