#include <FreeImage.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Constants.h"
#include "Color.h"
#include "Ray.h"
#include "Sphere.h"
#include "HitRecord.h"
#include "EntityList.h"
#include "Camera.h"






int main() {

	//esfera
    std::shared_ptr<EntityList> world = std::make_shared<EntityList>();
    world->addEntity(std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5)); // Center sphere
	world->addEntity(std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100)); // Ground sphere

	double aspect_ratio = 16.0 / 9.0; // Aspect ratio of the image
	int image_width = 800; // Width of the image in pixels
    
    std::unique_ptr<Camera>cam = std::make_unique<Camera>(aspect_ratio, image_width, 10);


	cam->render(*world);
   

}
