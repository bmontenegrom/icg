#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//todo testear si funciona

//https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
ObjectLoader& ObjectLoader::getInstance()
{
    static ObjectLoader instance;
    return instance;
}

std::vector<ObjVertex> ObjectLoader::loadOBJ(const std::string& filename)
{
    std::vector<Vec3> tempPositions;
    std::vector<Vec3> tempNormals;
    std::vector<Vec2> tempTexCoords;
    std::vector<ObjVertex> vertices;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo OBJ: " << filename << std::endl;
        return vertices;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            Vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            tempPositions.push_back(pos);
        }
        else if (prefix == "vt") {
            Vec2 uv;
            iss >> uv.x >> uv.y;
            tempTexCoords.push_back(uv);
        }
        else if (prefix == "vn") {
            Vec3 norm;
            iss >> norm.x >> norm.y >> norm.z;
            tempNormals.push_back(norm);
        }
        else if (prefix == "f") {
            std::string vertexData;
            for (int i = 0; i < 3; ++i) {
                iss >> vertexData;
                std::stringstream vss(vertexData);
                std::string posStr, texStr, normStr;

                int posIdx = 0, texIdx = 0, normIdx = 0;

                std::getline(vss, posStr, '/');
                std::getline(vss, texStr, '/');
                std::getline(vss, normStr, '/');

                try {
                    posIdx = std::stoi(posStr);
                    if (!texStr.empty()) texIdx = std::stoi(texStr);
                    if (!normStr.empty()) normIdx = std::stoi(normStr);
                }
                catch (const std::exception& e) {
                    std::cerr << "Error parseando índices de vértice: " << vertexData << " (" << e.what() << ")" << std::endl;
                    continue;
                }

                ObjVertex vtx;
                vtx.position = tempPositions.at(posIdx - 1);
                vtx.texCoord = (texIdx > 0 && texIdx <= (int)tempTexCoords.size()) ? tempTexCoords[texIdx - 1] : Vec2{ 0.0f, 0.0f };
                vtx.normal = (normIdx > 0 && normIdx <= (int)tempNormals.size()) ? tempNormals[normIdx - 1] : Vec3{ 0.0f, 0.0f, 0.0f };

                vertices.push_back(vtx);
            }
        }
    }

    file.close();
    return vertices;
}
