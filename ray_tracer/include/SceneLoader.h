#pragma once

#include <string>
#include <memory>
#include "Scene.h"
#include "Camera.h"
#include "WhittedTracer.h"

class SceneLoader {
public:
    /**
     * @brief Carga una escena y una cámara desde un archivo XML simple
     * @param filename Nombre del archivo XML a leer
     * @param out_camera Referencia a la cámara que se debe construir
     * @param tracer Trazador de Whitted para materiales complejos
     * @return Escena construida desde el archivo
     */
    static std::shared_ptr<Scene> loadFromXML(
        const std::string& filename,
        std::unique_ptr<Camera>& out_camera,
        const WhittedTracer& tracer
    );
};
