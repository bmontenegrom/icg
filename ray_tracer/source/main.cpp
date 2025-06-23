/**
 * @file main.cpp
 * @brief Punto de entrada principal para el sistema de ray tracing de Whitted
 * 
 * Este archivo implementa una demostración completa del algoritmo de Whitted Ray Tracing,
 * creando una escena con diferentes tipos de materiales (Phong, espejo, transparente),
 * múltiples fuentes de luz, y generando tanto la imagen principal como imágenes auxiliares
 * de reflexión y transmisión.
 * 
 * Características implementadas:
 * - Algoritmo de Whitted Ray Tracing con reflexión y refracción recursiva
 * - Múltiples tipos de materiales con diferentes propiedades ópticas
 * - Sistema de iluminación con múltiples fuentes de luz
 * - Generación de imágenes auxiliares para análisis visual
 * - Configuración de escena con geometría variada
 * 
 * @author Valentin Dutra
 * @date 07/06/2025
 * @version 1.0
 * @author Valentin Dutra
 * @date 08/06/2025
 * @version 1.1
 */
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>

#include <FreeImage.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <memory>

// Componentes base del sistema
#include "Constants.h"
#include "Color.h"
#include "Ray.h"
#include "Vec3.h"
#include "Camera.h"
#include "ObjectLoader.h"

// Geometría y entidades
#include "Sphere.h"
#include "EntityList.h"
#include "Cylinder.h"

// Materiales
#include "LambertianMaterial.h"
#include "Texture.h"
#include "MaterialTextured.h"
#include "MaterialNormalMapped.h"

// Geometría adicional
#include "Quad.h"

// Luces
#include "PointLight.h"

// Escena y Ray tracer de Whitted
#include "Scene.h"
#include "WhittedTracer.h"

#include "MaterialGlass.h"
#include "MaterialMirror.h"

#include "SceneLoader.h"




SDL_Window* window = nullptr;
SDL_GLContext gl_context;
GLuint texture_id;
std::vector<Color> framebuffer;

/**
 * @brief Crea y configura la escena de demostración
 * 
 * Construye una escena que demuestra las capacidades del algoritmo de Whitted:
 * - Esferas con diferentes materiales (difuso, especular, reflectante, transparente)
 * - Configuración de geometría que permite mostrar reflexiones y refracciones
 * - Posicionamiento estratégico para crear efectos visuales interesantes
 * 
 * @return Puntero a la lista de entidades que componen la escena
 */
std::shared_ptr<Scene> createScene() {
    auto world = std::make_shared<EntityList>();
    
    // === CONSTRUCCIÓN DE LA ESCENA BÁSICA ===
    
    // Crear materiales
    auto ground_material = std::make_shared<LambertianMaterial>(
        Color(0.1, 0.1, 0.1),  // Ambiente
        Color(0.5, 0.5, 0.5),  // Difuso
        Color(0.1, 0.1, 0.1),  // Especular
        10.0                   // Brillo
    );
    
    auto sphere_material = std::make_shared<LambertianMaterial>(
        Color(0.1, 0.0, 0.0),  // Ambiente
        Color(0.7, 0.3, 0.3),  // Difuso
        Color(0.5, 0.5, 0.5),  // Especular
        32.0                   // Brillo
    );
    
    // Suelo (esfera grande que actúa como plano)
    auto ground = std::make_shared<Sphere>(Vec3(0, -100.5, -1), 100);
    ground->setMaterial(ground_material);
    world->addEntity(ground);
    
    // Esfera central
    auto sphere = std::make_shared<Sphere>(Vec3(0, 0, -1), 0.5);
    sphere->setMaterial(sphere_material);
    world->addEntity(sphere);
    
    // Crear escena
    auto scene = std::make_shared<Scene>(world);
    
    // Agregar luces
    scene->addLight(std::make_shared<PointLight>(Vec3(-2, 2, 1), Color(0.9, 0.9, 0.9)));
    //scene->addLight(std::make_shared<PointLight>(Vec3(2, 2, 1), Color(0.6, 0.6, 0.8)));
    
    return scene;
}





/**
 * @brief Renderiza la imagen principal usando el WhittedTracer
 * 
 * Utiliza el WhittedTracer para renderizar la escena con iluminación avanzada,
 * reflexiones y refracciones.
 * 
 * @param scene Escena a renderizar
 * @param camera Cámara para la renderización
 */
void renderWhittedScene(const Scene& scene, Camera& camera) {
    // Crear el ray tracer de Whitted
    WhittedTracer tracer(10, 0.001); // 10 niveles de recursión, bias de 0.001
    
    // Obtener dimensiones de la imagen
    int width = camera.getImageWidth();
    int height = camera.getImageHeight();
    
    // Renderizar usando el WhittedTracer
    FreeImage_Initialise();
    FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
    
    if (!bitmap) {
        std::cerr << "Error creando imagen.\n";
        FreeImage_DeInitialise();
        return;
    }
    
    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Color pixel_color(0, 0, 0);
            
            // Muestreo múltiple para antialiasing
            int samples = camera.getSamplesPerPixel();
            for (int s = 0; s < samples; ++s) {
                Ray ray = camera.getRandomRay(i, j);
                pixel_color += tracer.trace(ray, scene);
            }
            
            // Promedio de las muestras
            pixel_color = pixel_color / static_cast<double>(samples);
            pixel_color = Color(std::sqrt(pixel_color.getR()), std::sqrt(pixel_color.getG()), std::sqrt(pixel_color.getB())); //gamma correction
            RGBQUAD color;
            color.rgbRed = pixel_color.getRbyte(); //getByte satura en 0 y 255
            color.rgbGreen = pixel_color.getGbyte();
            color.rgbBlue = pixel_color.getBbyte();
            
            FreeImage_SetPixelColor(bitmap, i, height - 1 - j, &color);
        }
        
        // Mostrar progreso
        if (j % 50 == 0) {
            std::cout << "Progreso: " << (100 * j / height) << "%\n";
        }
    }
    
    // Guardar imagen
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return;
    }

    std::ostringstream oss;
    oss << "images/output_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";

    if (FreeImage_Save(FIF_PNG, bitmap, oss.str().c_str(), 0)) {
        std::cout << "Imagen guardada: " << oss.str() << std::endl;
    } else {
        std::cerr << "Error guardando imagen.\n";
    }
    
    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();
}








/**
 * @brief Función principal del programa
 * 
 * Coordina la creación de la escena completa del algoritmo de ray tracing:
 * 1. Inicializa el sistema de renderización
 * 2. Crea la escena con geometría variada
 * 3. Configura el sistema de iluminación múltiple
 * 4. Configura la cámara con parámetros optimizados
 * 5. Ejecuta el ray tracing básico
 * 6. Demuestra las capacidades del WhittedTracer
 * 
 * @return 0 si el programa se ejecuta correctamente, código de error en caso contrario
 */
int main() {

    FreeImage_Initialise();

    std::unique_ptr<Camera> camera;
    std::unique_ptr<WhittedTracer> tracer;

    auto scene = SceneLoader::loadFromXML("assets/scenes/XMLscene.xml", camera, tracer);

    if (!scene || !camera) {
        std::cerr << "Error al cargar la escena desde XML.\n";
        return 1;
    }

  
	int width = camera->getImageWidth();
	int height = camera->getImageHeight();

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error inicializando SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ray Tracer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error creando ventana: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error creando renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!texture) {
        std::cerr << "Error creando textura: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }


    (*tracer).renderLive(*scene, *camera, renderer, texture);
	

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}
