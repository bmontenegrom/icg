#pragma once
#include <string>
#include <memory>
#include "Mesh.h"

class ObjectLoader {
public:
    /**
     * @brief Carga un archivo .obj y construye una Mesh con el material dado
     * @param filename Ruta del archivo .obj
     * @param material Material para todos los triángulos
     * @return Puntero compartido a Mesh
     */
    static std::shared_ptr<Mesh> loadObj(const std::string& filepath,
        std::shared_ptr<Material> mat,
        const Vec3& scale = Vec3(1, 1, 1),
        const Vec3& translate = Vec3(0, 0, 0));
};