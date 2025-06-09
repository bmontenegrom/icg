/**
 * @file Camera.cpp
 * @brief Implementación de la clase Camera para el sistema de ray tracing
 * 
 * Este archivo implementa la cámara virtual que:
 * - Define el viewport y la resolución de la imagen
 * - Genera rayos primarios para cada píxel
 * - Maneja el muestreo antialiasing
 * - Proporciona funcionalidad de renderizado
 * 
 * La cámara se encarga de:
 * - Configurar el viewport y los parámetros de renderizado
 * - Generar rayos para cada píxel con antialiasing
 * - Renderizar la escena y guardar la imagen resultante
 * - Manejar la transformación de coordenadas de viewport a mundo
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#include "Camera.h"
#include "FreeImage.h"
#include "Constants.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

/**
 * @brief Constructor que inicializa la cámara con un aspect ratio, ancho de imagen y muestras por píxel
 * @param aspect_ratio Relación de aspecto de la imagen (ancho/alto)
 * @param image_width Ancho de la imagen en píxeles
 * @param samples_per_pixel Número de muestras por píxel para antialiasing
 */
Camera::Camera(double aspect_ratio, int image_width, int samples_per_pixel)
	: aspect_ratio(aspect_ratio), image_width(image_width), samples_per_pixel(samples_per_pixel) {
	initialize();
}

/**
 * @brief Inicializa la cámara con los parámetros necesarios para el renderizado
 * 
 * Configura:
 * - Dimensiones de la imagen
 * - Escala de muestreo para antialiasing
 * - Geometría del viewport
 * - Vectores delta para el movimiento entre píxeles
 * - Posición inicial del primer píxel
 */
void Camera::initialize() {
	image_height = static_cast<int>(image_width / aspect_ratio);
	image_height = image_height < 1 ? 1 : image_height;
	pixel_sample_scale = 1.0 / static_cast<double>(samples_per_pixel);

	double focal_length = 1.0;
	double viewport_height = 2.5;
	double viewport_width = viewport_height * aspect_ratio;

	// Cámara más cerca de la caja
	center = Vec3(1.0, 1.0, -1.0);

	// Dirección hacia el centro de la caja
	Vec3 look_direction = unitVector(Vec3(1.0, 1.0, 1.0) - center);

	Vec3 viewport_u = Vec3(viewport_width, 0, 0);
	Vec3 viewport_v = Vec3(0, -viewport_height, 0);

	pixel_delta_u = viewport_u / static_cast<double>(image_width);
	pixel_delta_v = viewport_v / static_cast<double>(image_height);

	Vec3 viewport_upper_left = center + focal_length * look_direction - viewport_u / 2 - viewport_v / 2;
	pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

/**
 * @brief Renderiza la escena utilizando el algoritmo de ray tracing
 * 
 * Para cada píxel:
 * - Genera múltiples rayos con antialiasing
 * - Calcula el color promedio de las muestras
 * - Almacena el resultado en un archivo PNG
 * 
 * @param world Entidad que representa la escena a renderizar
 */
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

/**
 * @brief Genera un nombre de archivo con timestamp
 * 
 * El formato del timestamp es: YYYY-MM-DD_HH-MM-SS
 * El archivo se guarda en el directorio 'images/'
 * 
 * @return String con el nombre del archivo incluyendo la ruta y extensión .png
 */
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

/**
 * @brief Calcula el color de un rayo en la escena
 * 
 * Si el rayo golpea un objeto:
 * - Calcula el color basado en la normal del punto de impacto
 * Si no golpea nada:
 * - Genera un gradiente de cielo basado en la dirección del rayo
 * 
 * @param r El rayo a calcular
 * @param world Entidad que representa la escena
 * @return Color resultante del cálculo
 */
Color Camera::ray_color(const Ray& r, const Entity& world) const {
	HitRecord rec;
	if (world.hit(r, Interval(0, infinity), rec)) {
		return 0.5 * (Color(1 + rec.normal.getX(), 1 + rec.normal.getY(), 1 + rec.normal.getZ())); // Color based on normal
	}
	Vec3 unit_direction = unitVector(r.getDirection());
	double t = 0.5 * (unit_direction.getY() + 1.0);
	return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

/**
 * @brief Obtiene un rayo para un píxel específico con antialiasing
 * 
 * Genera un rayo desde el origen de la cámara hasta un punto aleatorio
 * dentro del píxel especificado para crear el efecto de antialiasing
 * 
 * @param i Coordenada x del píxel en la imagen
 * @param j Coordenada y del píxel en la imagen
 * @return Rayo generado con origen en la cámara y dirección hacia el píxel
 */
Ray Camera::getRay(int i, int j) const {
	Vec3 offset = this->sample_square();

	Vec3 pixel_sample = pixel00_loc + ((i + offset.getX()) * pixel_delta_u) + ((j + offset.getY()) * pixel_delta_v);
	
	Vec3 ray_origin = center;
	Vec3 ray_direction = pixel_sample - ray_origin;
	return Ray(ray_origin, ray_direction);

}

/**
 * @brief Genera un punto de muestreo aleatorio dentro de un cuadrado unitario
 * 
 * Utilizado para antialiasing, genera un offset aleatorio dentro del píxel
 * para distribuir las muestras y suavizar los bordes
 * 
 * @return Vector con coordenadas aleatorias en el rango [-0.5, 0.5] para x e y, y 0 para z
 */
Vec3 Camera::sample_square() const{
	return Vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
}

/**
 * @brief Obtiene el ancho de la imagen en píxeles
 * @return Ancho de la imagen
 */
int Camera::getImageWidth() const {
	return image_width;
}

/**
 * @brief Obtiene el alto de la imagen en píxeles
 * @return Alto de la imagen
 */
int Camera::getImageHeight() const {
	return image_height;
}

/**
 * @brief Obtiene el número de muestras por píxel para antialiasing
 * @return Número de muestras por píxel
 */
int Camera::getSamplesPerPixel() const {
	return samples_per_pixel;
}