#include "Entity.h"

class Apple : public Entity {
public:
	Apple(double x, double y, double z, double width, double height, double depth);
	void render() override;
};