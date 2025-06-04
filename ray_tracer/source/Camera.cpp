#include "Camera.h"
#include "FreeImage.h"
#include "Constants.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

Camera::Camera(double aspect_ratio, int image_width, int samples_per_pixel)
	: aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel) {
	initialize();
}

void Camera::initialize() {
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = image_height < 1 ? 1 : image_height; // Asegurarse de que la altura sea al menos 1
	pixel_sample_scale = 1.0 / static_cast<double>(samples_per_pixel);

	double focal_length = 1.0;
	double viewport_height = 2.0;
	double viewport_width = viewport_height * static_cast<double>(image_width) / image_height;
	center = Vec3(0, 0, 0);


	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	Vec3 viewport_u = Vec3(viewport_width, 0, 0);
	Vec3 viewport_v = Vec3(0, -viewport_height, 0);

	//Calculate the horizontal and vertical delta vectors from pixel to pixel.
	pixel_delta_u = viewport_u / static_cast<double>(image_width); //horizontal
	pixel_delta_v = viewport_v / static_cast<double>(image_height);//vertical

	// Calculate the position of the upper left pixel.
	Vec3 viewport_upper_left = center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void Camera::render(const Entity& world) const {
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(image_width, image_height, 24);
	if (!bitmap) {
		std::cerr << "Error al crear la imagen.\n";
		FreeImage_DeInitialise();
		return;
	}
	for (int j = 0; j < image_height; ++j) { // Invertir el orden de las filas
		for (int i = 0; i < image_width; ++i) {
			//Vec3 pixel_loc = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
			//Ray r(center, pixel_loc - center);
			//Color pixel_color = ray_color(r, world);
			// Set the pixel color in the bitmap
			Color pixel_color(0, 0, 0); // Initialize pixel color
			for (int s = 0; s < samples_per_pixel; ++s) {
				Ray r = getRay(i, j);
				pixel_color += ray_color(r, world);
			}
			pixel_color = pixel_color * pixel_sample_scale; // Scale the color by the number of samples
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(pixel_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(pixel_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(pixel_color.getBbyte());
			FreeImage_SetPixelColor(bitmap, i, image_height - 1 - j, &color);
		}
	}
	std::string filename = getTimestampedFilename();
	if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), 0)) {
		std::cout << "Imagen guardada como: " << filename << std::endl;
	}
	else {
		std::cerr << "Error al guardar la imagen.\n";
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
	
}

std::string Camera::getTimestampedFilename() const {
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};

	// localtime_s(destination, source)
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return "images/output_error.png";
	}

	std::ostringstream oss;
	oss << "images/output_"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";

	return oss.str();
}

Color Camera::ray_color(const Ray& r, const Entity& world) const {
	HitRecord rec;
	if (world.hit(r, Interval(0, infinity), rec)) {
		return 0.5 * (Color(1 + rec.normal.getX(), 1 + rec.normal.getY(), 1 + rec.normal.getZ())); // Color based on normal
	}
	Vec3 unit_direction = unitVector(r.getDirection());
	double t = 0.5 * (unit_direction.getY() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

Ray Camera::getRay(int i, int j) const {
	Vec3 offset = this->sample_square();

	Vec3 pixel_sample = pixel00_loc + ((i + offset.getX()) * pixel_delta_u) + ((j + offset.getY()) * pixel_delta_v);
	
	Vec3 ray_origin = center;
	Vec3 ray_direction = pixel_sample - ray_origin;
	return Ray(ray_origin, ray_direction);

}

Vec3 Camera::sample_square() const{
	return Vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
}