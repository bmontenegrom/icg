#pragma once
#include "Entity.h"
#include "Color.h"

class Camera {
public:
	Camera(double aspect_ratio, int image_width, int sample_per_pixel);
	~Camera() = default;

	void render(const Entity& world) const;

private:
	int image_height;
	int image_width;
	double aspect_ratio;
	int samples_per_pixel;
	double pixel_sample_scale; // scale factor for pixel samples
	
	Vec3 center;
	Vec3 pixel00_loc; //position of the pixel at (0,0) in the viewport
	Vec3 pixel_delta_u; // horizontal delta vector from pixel to pixel
	Vec3 pixel_delta_v; // vertical delta vector from pixel to pixel
	void initialize();
	Color ray_color(const Ray& r, const Entity& world) const;
	Ray getRay(int i, int j) const; 
	Vec3 sample_square() const;

	std::string getTimestampedFilename() const;
};
