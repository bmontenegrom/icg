#include "ObjLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <direct.h>



//https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
//https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
ObjectLoader& ObjectLoader::getInstance()
{
    static ObjectLoader instance;
    return instance;
}

std::vector<ObjVertex> ObjectLoader::loadOBJ(const std::string& filename, float scale)
{
    char cwd[1024];
    if (_getcwd(cwd, sizeof(cwd)) != NULL) {
        // std::cout << "Directorio actual: " << cwd << std::endl;
    }

    std::vector<Vec3> tempPositions;
    std::vector<Vec3> tempNormals;
    std::vector<Vec2> tempTexCoords;
    std::vector<ObjVertex> vertices;

    std::ifstream file(filename);
    if (!file.is_open()) {
        // std::cerr << "No se pudo abrir el archivo OBJ: " << filename << std::endl;
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
            pos.x *= scale;
            pos.y *= scale;
            pos.z *= scale;
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
            std::vector<std::string> faceVertices;
            std::string vertexData;
            while (iss >> vertexData) {
                faceVertices.push_back(vertexData);
            }
            for (size_t i = 1; i + 1 < faceVertices.size(); ++i) {
                int posIdx[3] = {0, 0, 0};
                int texIdx[3] = {0, 0, 0};
                int normIdx[3] = {0, 0, 0};
                std::string vStr[3] = {faceVertices[0], faceVertices[i], faceVertices[i+1]};
                for (int j = 0; j < 3; ++j) {
                    std::stringstream vss(vStr[j]);
                    std::string posStr, texStr, normStr;
                    std::getline(vss, posStr, '/');
                    std::getline(vss, texStr, '/');
                    std::getline(vss, normStr, '/');
                    posIdx[j] = !posStr.empty() ? std::stoi(posStr) : 0;
                    texIdx[j] = !texStr.empty() ? std::stoi(texStr) : 0;
                    normIdx[j] = !normStr.empty() ? std::stoi(normStr) : 0;
                }
                for (int j = 0; j < 3; ++j) {
                    ObjVertex vtx;
                    vtx.position = (posIdx[j] > 0 && posIdx[j] <= (int)tempPositions.size()) ? tempPositions[posIdx[j] - 1] : Vec3{0,0,0};
                    vtx.texCoord = (texIdx[j] > 0 && texIdx[j] <= (int)tempTexCoords.size()) ? tempTexCoords[texIdx[j] - 1] : Vec2{0,0};
                    vtx.texCoord.y = 1.0f - vtx.texCoord.y;
                    vtx.normal = (normIdx[j] > 0 && normIdx[j] <= (int)tempNormals.size()) ? tempNormals[normIdx[j] - 1] : Vec3{0,0,0};
                    vertices.push_back(vtx);
                }
            }
        }
    }

    file.close();

    // Debug: imprimir los primeros 10 vértices cargados
    // std::cout << "Primeros 10 vértices cargados:" << std::endl;
    // for (size_t i = 0; i < vertices.size() && i < 10; ++i) {
    //     std::cout << "Vértice " << i << ": ("
    //               << vertices[i].position.x << ", "
    //               << vertices[i].position.y << ", "
    //               << vertices[i].position.z << ")" << std::endl;
    // }
    // std::cout << "Total vértices cargados: " << vertices.size() << std::endl;
    return vertices;
}
