/**
 * @file Camera.h
 * @brief Definición de la clase Camera para el sistema de ray tracing
 * 
 * Esta clase representa una cámara en el espacio 3D y proporciona métodos para
 * renderizar la escena utilizando el algoritmo de ray tracing.
 * 
 * @author Benjamin Montenegro
 * @date 07/06/2025
 */

#pragma once
#include <vector>
#include "Vec3.h"
#include "Ray.h"
#include "Color.h"
#include "Entity.h"
#include "Scene.h"
#include "WhittedTracer.h"

class Camera {
public:
	/**
	 * @brief Constructor que inicializa la cámara con parámetros específicos
	 * @param aspect_ratio Relación de aspecto de la imagen
	 * @param image_width Ancho de la imagen en píxeles
	 * @param sample_per_pixel Número de muestras por píxel
	 */
	Camera(double aspect_ratio, int image_width, int sample_per_pixel);
	~Camera() = default;

	/**
	 * @brief Renderiza la escena utilizando el algoritmo de ray tracing
	 * @param world Entidad que representa la escena
	 */
	void render(const Entity& world) const;

	/**
	 * @brief Obtiene un rayo para un píxel específico
	 * @param i Coordenada x del píxel
	 * @param j Coordenada y del píxel
	 * @return Rayo generado
	 */
	Ray getRandomRay(int i, int j) const;
	
	Ray getRay(int i, int j) const;

	/**
	 * @brief Obtiene el ancho de la imagen
	 * @return Ancho de la imagen en píxeles
	 */
	int getImageWidth() const;
	
	/**
	 * @brief Obtiene la altura de la imagen
	 * @return Altura de la imagen en píxeles
	 */
	int getImageHeight() const;
	
	/**
	 * @brief Obtiene el número de muestras por píxel
	 * @return Número de muestras por píxel
	 */
	int getSamplesPerPixel() const; 

	void renderRow(int j, const Scene& scene, const WhittedTracer& tracer, std::vector<Color>& buffer) const;




private:
	int image_height; ///< Altura de la imagen en píxeles
	int image_width; ///< Ancho de la imagen en píxeles
	double aspect_ratio; ///< Relación de aspecto de la imagen
	int samples_per_pixel; ///< Número de muestras por píxel
	double pixel_sample_scale; ///< Factor de escala para las muestras de píxeles
	
	Vec3 center; ///< Centro de la cámara
	Vec3 pixel00_loc; ///< Posición del píxel (0,0) en el viewport
	Vec3 pixel_delta_u; ///< Vector delta horizontal entre píxeles
	Vec3 pixel_delta_v; ///< Vector delta vertical entre píxeles

	/**
	 * @brief Inicializa los parámetros de la cámara
	 */
	void initialize();

	/**
	 * @brief Calcula el color de un rayo en la escena
	 * @param r El rayo a calcular
	 * @param world Entidad que representa la escena
	 * @return Color resultante
	 */
	Color ray_color(const Ray& r, const Entity& world) const;

	/**
	 * @brief Muestra un cuadrado para el muestreo
	 * @return Vector de muestreo
	 */
	Vec3 sample_square() const;

	/**
	 * @brief Genera un nombre de archivo con timestamp
	 * @return Nombre de archivo con timestamp
	 */
	std::string getTimestampedFilename() const;
};
