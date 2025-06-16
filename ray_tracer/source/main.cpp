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

// Geometría adicional
#include "Quad.h"

// Luces
#include "PointLight.h"

// Escena y Ray tracer de Whitted
#include "Scene.h"
#include "WhittedTracer.h"

#include "MaterialGlass.h"
#include "MaterialMirror.h"

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
 * @brief Crea la famosa escena Cornell Box
 * 
 * Implementa la clásica Cornell Box con:
 * - Paredes blancas, roja (izquierda) y verde (derecha)
 * - Piso y techo blancos
 * - Luz puntual en el centro superior
 * - Dos esferas dentro de la escena como objetos de prueba
 * 
 * @return Puntero a la escena Cornell Box
 */
std::shared_ptr<Scene> createCornellBoxScene(const WhittedTracer& tracer) {
    auto world = std::make_shared<EntityList>();

    // === MATERIALES DE LA CORNELL BOX ===
    auto red_material = std::make_shared<LambertianMaterial>(
        Color(0.1, 0.0, 0.0),
        Color(0.65, 0.05, 0.05),
        Color(0.1, 0.1, 0.1),
        10.0
    );
    auto green_material = std::make_shared<LambertianMaterial>(
        Color(0.0, 0.1, 0.0),
        Color(0.12, 0.45, 0.15),
        Color(0.1, 0.1, 0.1),
        10.0
    );
    auto white_material = std::make_shared<LambertianMaterial>(
        Color(0.1, 0.1, 0.1),
        Color(0.73, 0.73, 0.73),
        Color(0.1, 0.1, 0.1),
        10.0
    );

    // === CONSTRUCCIÓN DE LAS PAREDES (Cornell Box clásica) ===
    
    // Pared izquierda (roja) - X = 0
    auto left_wall = std::make_shared<Quad>(
        Vec3(0, 0, 0), Vec3(0, 2, 2), 0, 0.0  // min, max, eje X fijo, valor X=0
    );
    left_wall->setMaterial(red_material);
    world->addEntity(left_wall);

    // Pared derecha (verde) - X = 2
    auto right_wall = std::make_shared<Quad>(
        Vec3(2, 0, 0), Vec3(2, 2, 2), 0, 2.0  // min, max, eje X fijo, valor X=2
    );
    right_wall->setMaterial(green_material);
    world->addEntity(right_wall);

    // Pared trasera (blanca) - Z = 2
    auto back_wall = std::make_shared<Quad>(
        Vec3(0, 0, 2), Vec3(2, 2, 2), 2, 2.0  // min, max, eje Z fijo, valor Z=2
    );
    back_wall->setMaterial(white_material);
    world->addEntity(back_wall);

    // Piso (blanco) - Y = 0
    auto floor_quad = std::make_shared<Quad>(
        Vec3(0, 0, 0), Vec3(2, 0, 2), 1, 0.0  // min, max, eje Y fijo, valor Y=0
    );
    floor_quad->setMaterial(white_material);
    world->addEntity(floor_quad);

    // Techo (blanco) - Y = 2
    auto ceiling = std::make_shared<Quad>(
        Vec3(0, 2, 0), Vec3(2, 2, 2), 1, 2.0  // min, max, eje Y fijo, valor Y=2
    );
    ceiling->setMaterial(white_material);
    world->addEntity(ceiling);

    // === OBJETOS DENTRO DE LA CAJA ===
    
    // Esfera central
    /*
    auto sphere_material = std::make_shared<LambertianMaterial>(
        Color(0.1, 0.1, 0.1),
        Color(0.7, 0.3, 0.3),
        Color(0.5, 0.5, 0.5),
        32.0
    );
    auto sphere = std::make_shared<Sphere>(Vec3(0.7, 0.5, 0.7), 0.3);
    sphere->setMaterial(sphere_material);
    world->addEntity(sphere);
    /*
    // Otra esfera
    auto sphere2_material = std::make_shared<LambertianMaterial>(
        Color(0.0, 0.0, 0.1),
        Color(0.3, 0.3, 0.7),
        Color(0.5, 0.5, 0.5),
        32.0
    );
    auto sphere2 = std::make_shared<Sphere>(Vec3(1.3, 0.3, 1.3), 0.40);
    sphere2->setMaterial(sphere2_material);
    world->addEntity(sphere2);
    */
    // Crear escena
    auto scene = std::make_shared<Scene>(world);

    // === LUCES ===
    
    // Luz principal en el techo
    scene->addLight(std::make_shared<PointLight>(
        Vec3(1.0, 1.8, 1.0),
        Color(1.0, 1.0, 1.0)  // Luz más intensa
    ));

	scene->addLight(std::make_shared<PointLight>(
		Vec3(0.5, 1.8, 0.5),
		Color(0.8, 0.8, 0.8)  // Luz secundaria
	));

    // === ESFERA DE VIDRIO ===
    auto glass_color = Color(0.8, 0.9, 1.0); // Leve tinte azul
    double glass_ior = 1.5;
    auto glass_material = std::make_shared<MaterialGlass>(glass_color, glass_ior, tracer);

    auto glass_sphere = std::make_shared<Sphere>(Vec3(1.0, 0.5, 1.0), 0.3);
    glass_sphere->setMaterial(glass_material);
    world->addEntity(glass_sphere);

    //cilindro
    /*
	auto cylinder_material = std::make_shared<LambertianMaterial>(
        Color(0.0, 0.0, 0.1),
        Color(0.3, 0.3, 0.7),
        Color(0.5, 0.5, 0.5),
        32.0
	);
    
	auto cylinder = std::make_shared<Cylinder>(Vec3(1.3, 0.0, 1.3),0.0, 0.4, 0.4);
	cylinder->setMaterial(cylinder_material);
	world->addEntity(cylinder);
    
    // === ESFERA ESPEJO ===
    auto mirror_color = Color(1.0, 1.0, 1.0); 
    auto mirror_material = std::make_shared<MaterialMirror>(mirror_color, tracer);
    auto mirror_sphere = std::make_shared<Sphere>(Vec3(1.5, 0.5, 0.5), 0.25);
    mirror_sphere->setMaterial(mirror_material);
    world->addEntity(mirror_sphere);
    */
   
    auto apple_material = std::make_shared<LambertianMaterial>(
        Color(0.0, 0.0, 0.1),
        Color(0.3, 0.3, 0.7),
        Color(0.5, 0.5, 0.5),
        32.0
    );
    auto apple_mesh = ObjectLoader::loadObj("assets/apple/apple.obj", apple_material, Vec3(0.3, 0.3, 0.3), Vec3(0.5, 0.5, 0.5));
    if (!apple_mesh) {
        std::cerr << "Error al cargar el modelo apple.obj\n";
    }
    else {
        world->addEntity(apple_mesh); // O el método equivalente que uses
    }


    return scene;
}


/**
 * @brief Configura la cámara específicamente para la Cornell Box
 * 
 * Establece los parámetros de la cámara optimizados para la Cornell Box:
 * - Posición de la cámara para ver la escena completa
 * - Ratio de aspecto cuadrado (1:1) típico de la Cornell Box
 * - Resolución y muestreo balanceados
 * 
 * @return Puntero único a la cámara configurada para Cornell Box
 */
std::unique_ptr<Camera> createCornellBoxCamera() {
    // Configuración de imagen para Cornell Box
    double aspect_ratio = 1.0;     // Relación de aspecto cuadrada
    int image_width = 800;         // Resolución
    int samples_per_pixel = 16;    // Muestras por pixel

    auto camera = std::make_unique<Camera>(aspect_ratio, image_width, samples_per_pixel);
    return camera;
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
                Ray ray = camera.getRay(i, j);
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
    try {
        // Inicializar FreeImage para manejo de imágenes
        FreeImage_Initialise();
        
        WhittedTracer tracer(10, 0.001);
        auto scene = createCornellBoxScene(tracer);
   

        
        // Configurar cámara para Cornell Box
        auto camera = createCornellBoxCamera();
        
        // === RENDERIZACIÓN CON WHITTED RAY TRACING ===
        renderWhittedScene(*scene, *camera);
        
		tracer.generateReflectionMap(*scene, *camera, camera->getImageWidth(), camera->getImageHeight());
		tracer.generateTransmissionMap(*scene, *camera, camera->getImageWidth(), camera->getImageHeight());
        // Finalizar FreeImage
        FreeImage_DeInitialise();
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Error durante la ejecución: " << e.what() << std::endl;
        FreeImage_DeInitialise();
        return 1;
        
    } catch (...) {
        std::cerr << "Error desconocido durante la ejecución." << std::endl;
        FreeImage_DeInitialise();
        return 1;
    }
}
