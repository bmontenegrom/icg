/**
 * @file WhittedTracer.cpp
 * @brief Implementación del algoritmo de Ray Tracing de Whitted
 * 
 * Este archivo implementa el algoritmo clásico de ray tracing desarrollado por
 * Turner Whitted en 1980. El algoritmo traza rayos recursivamente para calcular
 * reflexiones y refracciones, produciendo imágenes con iluminación realista.
 * 
 * Características implementadas:
 * - Trazado recursivo de rayos con profundidad configurable
 * - Cálculo de iluminación local usando el modelo de Phong
 * - Generación de rayos de reflexión y refracción
 * - Manejo de sombras con bias para evitar self-shadowing
 * - Generación de mapas de reflexión y transmisión
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 */

#include "WhittedTracer.h"
#include "Constants.h"
#include "Camera.h"
#include "FreeImage.h"
#include "Vec3.h"
#include "Ray.h"
#include "Material.h"
#include "Scene.h"
#include "Interval.h"
#include <algorithm>
#include <iostream>
#include <iomanip>  // Para std::put_time
#include <sstream>  // Para std::ostringstream
#include <ctime>    // Para std::time, std::tm

/**
 * @brief Constructor de WhittedTracer
 * @param max_depth Profundidad máxima de recursión para el trazado de rayos
 * @param shadow_bias Valor de bias para evitar self-shadowing
 */
WhittedTracer::WhittedTracer(int max_depth, double shadow_bias)
    : max_depth(max_depth), shadow_bias(shadow_bias) {
}

/**
 * @brief Traza un rayo en la escena y calcula el color resultante
 * @param ray Rayo a trazar
 * @param scene Escena en la que se traza el rayo
 * @param depth Profundidad actual de recursión
 * @return Color resultante del trazado del rayo
 */
Color WhittedTracer::trace(const Ray& ray, const Scene& scene, int depth) const {
    // Verificar límite de profundidad de recursión
    
    
    // Encontrar la intersección más cercana
    HitRecord hit_record;
    if (!scene.hit(ray, Interval(0.001, infinity), hit_record)) {
        // No hay intersección, retornar color de fondo
        return backgroundColor(ray);
    }
    
    // Verificar si el objeto tiene material
    if (!hit_record.material_ptr) {
        // Material por defecto (difuso gris)
        return Color(0.5, 0.5, 0.5);
    }
    
    // Usar el método shade del material para calcular el color
    return hit_record.material_ptr->shade(ray, hit_record, scene, depth);
    
}

/**
 * @brief Calcula el color de fondo para un rayo que no intersecta con ningún objeto
 * @param ray Rayo para el cual calcular el color de fondo
 * @return Color de fondo (gradiente de cielo)
 */
Color WhittedTracer::backgroundColor(const Ray& ray) const {
    // Gradiente azul simple para simular el cielo
    Vec3 unit_direction = unitVector(ray.getDirection());
    double t = 0.5 * (unit_direction.getY() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

/**
 * @brief Genera un mapa de reflexión de la escena
 * @param scene Escena para la cual generar el mapa
 * @param camera Cámara desde la cual se genera el mapa
 * @param width Ancho de la imagen resultante
 * @param height Alto de la imagen resultante
 * @param filename Nombre del archivo donde guardar el mapa
 */
void WhittedTracer::generateReflectionMap(const Scene& scene, Camera& camera,
                                        int width, int height) const {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    
    if (!bitmap) {
        std::cerr << "Error creando imagen de reflexión.\n";
        FreeImage_DeInitialise();
        return;
    }
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            HitRecord hit_record;
            
            double reflection_coeff = 0.0;
            if (scene.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    reflection_coeff = hit_record.material_ptr->getReflectivity();
                }
            }
            
            // Convertir coeficiente a escala de grises (0-255)
            BYTE gray_value = static_cast<BYTE>(reflection_coeff * 255);
            
            RGBQUAD color;
            color.rgbRed = gray_value;
            color.rgbGreen = gray_value;
            color.rgbBlue = gray_value;
            
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }
    }
    
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return;
    }

    std::ostringstream oss;
    oss << "images/reflection_map_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";
    
    if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
        std::cout << "Mapa de reflexión guardado: " << oss.str() << std::endl;
    } else {
        std::cerr << "Error guardando mapa de reflexión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}

/**
 * @brief Genera un mapa de transmisión de la escena
 * @param scene Escena para la cual generar el mapa
 * @param camera Cámara desde la cual se genera el mapa
 * @param width Ancho de la imagen resultante
 * @param height Alto de la imagen resultante
 * @param filename Nombre del archivo donde guardar el mapa
 */
void WhittedTracer::generateTransmissionMap(const Scene& scene, Camera& camera,
                                          int width, int height) const {
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    
    if (!bitmap) {
        std::cerr << "Error creando imagen de transmisión.\n";
        FreeImage_DeInitialise();
        return;
    }
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            HitRecord hit_record;
            
            double transmission_coeff = 0.0;
            if (scene.hit(ray, Interval(0.001, infinity), hit_record)) {
                if (hit_record.material_ptr) {
                    transmission_coeff = hit_record.material_ptr->getTransparency();
                }
            }
            
            // Convertir coeficiente a escala de grises (0-255)
            BYTE gray_value = static_cast<BYTE>(transmission_coeff * 255);
            
            RGBQUAD color;
            color.rgbRed = gray_value;
            color.rgbGreen = gray_value;
            color.rgbBlue = gray_value;
            
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }
    }
    
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return;
    }

    std::ostringstream oss;
    oss << "images/transmission_map_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";
    
    if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
        std::cout << "Mapa de transmisión guardado: " << oss.str() << std::endl;
    } else {
        std::cerr << "Error guardando mapa de transmisión.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}
int WhittedTracer::getMaxDepth() const
{
    return max_depth;
}

Color WhittedTracer::traceComponent(const Ray& ray, const Scene& scene, ShadeComponent component) const
{
    HitRecord hit;
    if (scene.hit(ray, Interval(0.001, infinity), hit)) {
        if (hit.material_ptr) {
            return hit.material_ptr->shadeComponent(component, ray, hit, scene);
        }
    }
    return Color(0, 0, 0);
}

void WhittedTracer::generateAuxImages(const Scene& scene, Camera& camera, int width, int height) const
{
    generateDiffuseImage(scene, camera, width, height);
    generateSpecularImage(scene, camera, width, height);
    generateAmbientImage(scene, camera, width, height);
	generateReflectionImage(scene, camera, width, height);
	generateTransmissionImage(scene, camera, width, height);
}

void WhittedTracer::generateDiffuseImage(const Scene& scene, Camera& camera, int width, int height) const
{
	FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);

	if (!bitmap) {
		std::cerr << "Error creando imagen de difusos.\n";
		FreeImage_DeInitialise();
		return;
	}

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            Color diffuse_color = traceComponent(ray, scene, ShadeComponent::Diffuse);
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(diffuse_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(diffuse_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(diffuse_color.getBbyte());

            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
	}

	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return;
	}
	std::ostringstream oss;
	oss << "images/diffuse_"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";

	if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
		std::cout << "Mapa difuso guardado: " << oss.str() << std::endl;
	}
	else {
		std::cerr << "Error guardando mapa difuso.\n";
	}

	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::generateSpecularImage(const Scene& scene, Camera& camera, int width, int height) const
{
	FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de especulares.\n";
		FreeImage_DeInitialise();
		return;
	}
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            Color specular_color = traceComponent(ray, scene, ShadeComponent::Specular);
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(specular_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(specular_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(specular_color.getBbyte());
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return;
	}
	std::ostringstream oss;
	oss << "images/specular_"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";
	if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
		std::cout << "Mapa especular guardado: " << oss.str() << std::endl;
	}
	else {
		std::cerr << "Error guardando mapa especular.\n";
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::generateAmbientImage(const Scene& scene, Camera& camera, int width, int height) const
{
	FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de ambiente.\n";
		FreeImage_DeInitialise();
		return;
	}
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            Color ambient_color = traceComponent(ray, scene, ShadeComponent::Ambient);
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(ambient_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(ambient_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(ambient_color.getBbyte());
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return;
	}
	std::ostringstream oss;
	oss << "images/ambient_"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";
	if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
		std::cout << "Mapa ambiental guardado: " << oss.str() << std::endl;
	}
	else {
		std::cerr << "Error guardando mapa ambiental.\n";
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::generateReflectionImage(const Scene& scene, Camera& camera, int width, int height) const
{
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de reflexión.\n";
		FreeImage_DeInitialise();
		return;
	}

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color reflection_color = traceComponent(ray, scene, ShadeComponent::Reflection);
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(reflection_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(reflection_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(reflection_color.getBbyte());
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
	}

	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return;
	}

	std::ostringstream oss;
	oss << "images/reflection_"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";

	if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
		std::cout << "Imagen de reflexión guardado: " << oss.str() << std::endl;
	}
	else {
		std::cerr << "Error guardando imaghen de reflexión.\n";
	}

	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::generateTransmissionImage(const Scene& scene, Camera& camera, int width, int height) const
{
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de reflexión.\n";
		FreeImage_DeInitialise();
		return;
	}

	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color transmission_color = traceComponent(ray, scene, ShadeComponent::Transmission);
			RGBQUAD color;
			color.rgbRed = static_cast<BYTE>(transmission_color.getRbyte());
			color.rgbGreen = static_cast<BYTE>(transmission_color.getGbyte());
			color.rgbBlue = static_cast<BYTE>(transmission_color.getBbyte());
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
	}

	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) != 0) {
		std::cerr << "Error al obtener la hora local.\n";
		return;
	}

	std::ostringstream oss;
	oss << "images/transmission"
		<< std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
		<< ".png";

	if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
		std::cout << "Imagen de transmisión guardado: " << oss.str() << std::endl;
	}
	else {
		std::cerr << "Error en imagen de reflexión.\n";
	}

	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::renderWhittedSceneLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
    int width = camera.getImageWidth();
    int height = camera.getImageHeight();
    int spp = camera.getSamplesPerPixel();

    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    if (!bitmap) {
        std::cerr << "Error creando imagen.\n";
        FreeImage_DeInitialise();
        return;
    }

    std::vector<uint8_t> framebuffer(width * height * 3, 0);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < spp; ++s) {
                Ray ray = camera.getRandomRay(i, j);
                pixel_color += trace(ray, scene);
            }
            pixel_color = pixel_color / static_cast<double>(spp);
            pixel_color = Color(
                std::sqrt(pixel_color.getR()),
                std::sqrt(pixel_color.getG()),
                std::sqrt(pixel_color.getB())
            );

            uint8_t r = pixel_color.getRbyte();
            uint8_t g = pixel_color.getGbyte();
            uint8_t b = pixel_color.getBbyte();

            int index = (j * width + i) * 3;
            framebuffer[index + 0] = r;
            framebuffer[index + 1] = g;
            framebuffer[index + 2] = b;

            RGBQUAD color;
            color.rgbRed = r;
            color.rgbGreen = g;
            color.rgbBlue = b;
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }

        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) == 0) {
        std::ostringstream oss;
        oss << "images/render_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
        if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
            std::cout << "Imagen guardada: " << oss.str() << std::endl;
        }
        else {
            std::cerr << "Error guardando la imagen.\n";
        }
    }

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}

void WhittedTracer::renderTransmissionLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
    int width = camera.getImageWidth();
    int height = camera.getImageHeight();

    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    if (!bitmap) {
        std::cerr << "Error creando imagen de transmisión.\n";
        FreeImage_DeInitialise();
        return;
    }

    std::vector<uint8_t> framebuffer(width * height * 3, 0);

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Ray ray = camera.getRay(i, j);
            Color transmission_color = traceComponent(ray, scene, ShadeComponent::Transmission);

            uint8_t r = transmission_color.getRbyte();
            uint8_t g = transmission_color.getGbyte();
            uint8_t b = transmission_color.getBbyte();

			int index = (j * width + i) * 3;
            framebuffer[index + 0] = r;
            framebuffer[index + 1] = g;
            framebuffer[index + 2] = b;

            RGBQUAD color;
            color.rgbRed = r;
            color.rgbGreen = g;
            color.rgbBlue = b;
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }

        SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
    }

    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) == 0) {
        std::ostringstream oss;
        oss << "images/transmission_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
        if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
            std::cout << "Imagen de transmisión guardada: " << oss.str() << std::endl;
        }
        else {
            std::cerr << "Error guardando mapa de transmisión.\n";
        }
    }

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

}

void WhittedTracer::renderReflectionLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
	int width = camera.getImageWidth();
	int height = camera.getImageHeight();
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de reflexión.\n";
		FreeImage_DeInitialise();
		return;
	}
	std::vector<uint8_t> framebuffer(width * height * 3, 0);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color reflection_color = traceComponent(ray, scene, ShadeComponent::Reflection);
			uint8_t r = reflection_color.getRbyte();
			uint8_t g = reflection_color.getGbyte();
			uint8_t b = reflection_color.getBbyte();
			int index = (j * width + i) * 3;
			framebuffer[index + 0] = r;
			framebuffer[index + 1] = g;
			framebuffer[index + 2] = b;
			RGBQUAD color;
			color.rgbRed = r;
			color.rgbGreen = g;
			color.rgbBlue = b;
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
		SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) == 0) {
		std::ostringstream oss;
		oss << "images/reflection_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
		if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
			std::cout << "Imagen de reflexión guardada: " << oss.str() << std::endl;
		}
		else {
			std::cerr << "Error guardando imagen de reflexión.\n";
		}
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::renderDiffuseLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
	int width = camera.getImageWidth();
	int height = camera.getImageHeight();
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de difusos.\n";
		FreeImage_DeInitialise();
		return;
	}
	std::vector<uint8_t> framebuffer(width * height * 3, 0);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color diffuse_color = traceComponent(ray, scene, ShadeComponent::Diffuse);
			uint8_t r = diffuse_color.getRbyte();
			uint8_t g = diffuse_color.getGbyte();
			uint8_t b = diffuse_color.getBbyte();
			int index = (j * width + i) * 3;
			framebuffer[index + 0] = r;
			framebuffer[index + 1] = g;
			framebuffer[index + 2] = b;
			RGBQUAD color;
			color.rgbRed = r;
			color.rgbGreen = g;
			color.rgbBlue = b;
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
		SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) == 0) {
		std::ostringstream oss;
		oss << "images/diffuse_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
		if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
			std::cout << "Imagen componente difusa guardada: " << oss.str() << std::endl;
		}
		else {
			std::cerr << "Error guardando imagen difusa.\n";
		}
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::renderSpecularLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
	int width = camera.getImageWidth();
	int height = camera.getImageHeight();
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de especulares.\n";
		FreeImage_DeInitialise();
		return;
	}
	std::vector<uint8_t> framebuffer(width * height * 3, 0);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color specular_color = traceComponent(ray, scene, ShadeComponent::Specular);
			uint8_t r = specular_color.getRbyte();
			uint8_t g = specular_color.getGbyte();
			uint8_t b = specular_color.getBbyte();
			int index = (j * width + i) * 3;
			framebuffer[index + 0] = r;
			framebuffer[index + 1] = g;
			framebuffer[index + 2] = b;
			RGBQUAD color;
			color.rgbRed = r;
			color.rgbGreen = g;
			color.rgbBlue = b;
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
		SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) == 0) {
		std::ostringstream oss;
		oss << "images/specular_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
		if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
			std::cout << "Imagen componente especular guardada: " << oss.str() << std::endl;
		}
		else {
			std::cerr << "Error guardando mapa especular.\n";
		}
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::renderAmbientLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
	int width = camera.getImageWidth();
	int height = camera.getImageHeight();
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	if (!bitmap) {
		std::cerr << "Error creando imagen de ambiente.\n";
		FreeImage_DeInitialise();
		return;
	}
	std::vector<uint8_t> framebuffer(width * height * 3, 0);
	for (int j = 0; j < height; ++j) {
		for (int i = 0; i < width; ++i) {
			Ray ray = camera.getRay(i, j);
			Color ambient_color = traceComponent(ray, scene, ShadeComponent::Ambient);
			uint8_t r = ambient_color.getRbyte();
			uint8_t g = ambient_color.getGbyte();
			uint8_t b = ambient_color.getBbyte();
			int index = (j * width + i) * 3;
			framebuffer[index + 0] = r;
			framebuffer[index + 1] = g;
			framebuffer[index + 2] = b;
			RGBQUAD color;
			color.rgbRed = r;
			color.rgbGreen = g;
			color.rgbBlue = b;
			FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
		}
		SDL_UpdateTexture(texture, nullptr, framebuffer.data(), width * 3);
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}
	std::time_t now = std::time(nullptr);
	std::tm tm_info{};
	if (localtime_s(&tm_info, &now) == 0) {
		std::ostringstream oss;
		oss << "images/ambient_" << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S") << ".png";
		if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
			std::cout << "Imagen componente ambiental guardada: " << oss.str() << std::endl;
		}
		else {
			std::cerr << "Error guardando mapa ambiental.\n";
		}
	}
	FreeImage_Unload(bitmap);
	FreeImage_DeInitialise();
}

void WhittedTracer::renderLive(const Scene& scene, Camera& camera, SDL_Renderer* renderer, SDL_Texture* texture)
{
	renderWhittedSceneLive(scene, camera, renderer, texture);
	renderDiffuseLive(scene, camera, renderer, texture);
	renderSpecularLive(scene, camera, renderer, texture);
	renderAmbientLive(scene, camera, renderer, texture);
	renderReflectionLive(scene, camera, renderer, texture);
	renderTransmissionLive(scene, camera, renderer, texture);
	std::cout << "Renderizado en vivo completado.\n";
	generateTransmissionMap(scene, camera, camera.getImageWidth(), camera.getImageHeight());
	generateReflectionMap(scene, camera, camera.getImageWidth(), camera.getImageHeight());

}

