#pragma once
#include "Entity.h"
#include "Color.h"

class Camera {
public:
	Camera(double aspect_ratio, int image_width);
	~Camera() = default;

	void render(const Entity& world) const;

private:
	int image_height;
	int image_width;
	double aspect_ratio;
	
	Vec3 center;
	Vec3 pixel00_loc; //position of the pixel at (0,0) in the viewport
	Vec3 pixel_delta_u; // horizontal delta vector from pixel to pixel
	Vec3 pixel_delta_v; // vertical delta vector from pixel to pixel
	void initialize();
	Color ray_color(const Ray& r, const Entity& world) const;

	std::string getTimestampedFilename() const;
};
