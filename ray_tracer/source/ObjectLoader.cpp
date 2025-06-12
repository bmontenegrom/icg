#include "ObjectLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>


std::shared_ptr<Mesh> ObjectLoader::loadObj(const std::string& filepath,
    std::shared_ptr<Material> mat,
    const Vec3& scale,
    const Vec3& translate) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << filepath << std::endl;
        return nullptr;
    }

    std::vector<Vec3> vertices;
    std::vector<std::array<int, 3>> indices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(
                x * scale.getX() + translate.getX(),
                y * scale.getY() + translate.getY(),
                z * scale.getZ() + translate.getZ()
            );
        }
        else if (prefix == "f") {
            std::vector<int> face_indices;
            std::string token;
            while (iss >> token) {
                size_t pos = token.find('/');
                if (pos != std::string::npos) {
                    token = token.substr(0, pos);
                }
                face_indices.push_back(std::stoi(token) - 1);
            }
            // triangulación fan
            for (size_t i = 1; i + 1 < face_indices.size(); ++i) {
                indices.push_back({ face_indices[0], face_indices[i], face_indices[i + 1] });
            }
        }
    }

    return std::make_shared<Mesh>(vertices, indices, mat);
}