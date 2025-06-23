#include "SceneLoader.h"
#include "EntityList.h"
#include "Scene.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "LambertianMaterial.h"
#include "MaterialMirror.h"
#include "MaterialGlass.h"
#include "PointLight.h"
#include "Camera.h"
#include "WhittedTracer.h"
#include "MaterialTextured.h"
#include "Texture.h"
#include "Triangle.h"
#include "Mesh.h"
#include "ObjectLoader.h"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <Quad.h>
#include <MaterialNormalMapped.h>

// Función utilitaria para extraer valores XML simples
static std::string getAttribute(const std::string& line, const std::string& key) {
    size_t start = line.find(key + "=\"");
    if (start == std::string::npos) return "";
    start += key.length() + 2;
    size_t end = line.find("\"", start);
    return line.substr(start, end - start);
}

static double parseDouble(const std::string& value) {
    return std::stod(value);
}

// Mapa global de materiales por ID
static std::unordered_map<std::string, std::shared_ptr<Material>> materialMap;

std::shared_ptr<Scene> SceneLoader::loadFromXML(const std::string& filename, std::unique_ptr<Camera>& out_camera, const WhittedTracer& tracer)
{
    auto world = std::make_shared<EntityList>();
    auto scene = std::make_shared<Scene>(world);

    std::ifstream file(filename);
    if (!file.is_open()) {
        return nullptr;
    }

    std::string line;
    double aspect = 1.0;
    int width = 800;
    int samples = 8;

    while (std::getline(file, line)) {
        // Camera
        if (line.find("<camera") != std::string::npos) {
            aspect = parseDouble(getAttribute(line, "aspect"));
            width = std::stoi(getAttribute(line, "width"));
            samples = std::stoi(getAttribute(line, "samples"));
        }

        // Materiales
        else if (line.find("<lambertian") != std::string::npos) {
            std::string id = getAttribute(line, "id");
            Color ambient(parseDouble(getAttribute(line, "ambientR")),
                parseDouble(getAttribute(line, "ambientG")),
                parseDouble(getAttribute(line, "ambientB")));
            Color diffuse(parseDouble(getAttribute(line, "diffuseR")),
                parseDouble(getAttribute(line, "diffuseG")),
                parseDouble(getAttribute(line, "diffuseB")));
            Color specular(parseDouble(getAttribute(line, "specularR")),
                parseDouble(getAttribute(line, "specularG")),
                parseDouble(getAttribute(line, "specularB")));
            double shininess = parseDouble(getAttribute(line, "shininess"));

            auto mat = std::make_shared<LambertianMaterial>(ambient, diffuse, specular, shininess);
            materialMap[id] = mat;
        }
        else if (line.find("<mirror") != std::string::npos) {
            std::string id = getAttribute(line, "id");
            Color albedo(parseDouble(getAttribute(line, "albedoR")),
                parseDouble(getAttribute(line, "albedoG")),
                parseDouble(getAttribute(line, "albedoB")));
            auto mat = std::make_shared<MaterialMirror>(albedo, tracer);
            materialMap[id] = mat;
        }
        else if (line.find("<glass") != std::string::npos) {
            std::string id = getAttribute(line, "id");
            Color albedo(parseDouble(getAttribute(line, "albedoR")),
                parseDouble(getAttribute(line, "albedoG")),
                parseDouble(getAttribute(line, "albedoB")));
            double ior = parseDouble(getAttribute(line, "ior"));
            auto mat = std::make_shared<MaterialGlass>(albedo, ior, tracer);
            materialMap[id] = mat;
        }

        // Entidades
        else if (line.find("<sphere") != std::string::npos) {
            Vec3 center(parseDouble(getAttribute(line, "cx")),
                parseDouble(getAttribute(line, "cy")),
                parseDouble(getAttribute(line, "cz")));
            double radius = parseDouble(getAttribute(line, "radius"));
            std::string mat_id = getAttribute(line, "material");

            auto sphere = std::make_shared<Sphere>(center, radius);
            if (materialMap.count(mat_id)) {
                sphere->setMaterial(materialMap[mat_id]);
            }
            world->addEntity(sphere);
        }
        else if (line.find("<cylinder") != std::string::npos) {
            Vec3 center(parseDouble(getAttribute(line, "cx")),
                parseDouble(getAttribute(line, "cy")),
                parseDouble(getAttribute(line, "cz")));
            double y0 = parseDouble(getAttribute(line, "y0"));
            double y1 = parseDouble(getAttribute(line, "y1"));
            double radius = parseDouble(getAttribute(line, "radius"));
            std::string mat_id = getAttribute(line, "material");

            auto cyl = std::make_shared<Cylinder>(center, y0, y1, radius);
            if (materialMap.count(mat_id)) {
                cyl->setMaterial(materialMap[mat_id]);
            }
            world->addEntity(cyl);
        }
        else if (line.find("<quad") != std::string::npos) {
            Vec3 minP, maxP;
            std::string min_str = getAttribute(line, "min");
            std::string max_str = getAttribute(line, "max");
            std::string mat_id = getAttribute(line, "material");
            int axis = std::stoi(getAttribute(line, "axis"));
            double value = parseDouble(getAttribute(line, "value"));

            // Parsear min
            std::stringstream ss_min(min_str);
            double minX, minY, minZ;
            char comma;
            ss_min >> minX >> comma >> minY >> comma >> minZ;
            minP.setX(minX);
            minP.setY(minY);
            minP.setZ(minZ);

            // Parsear max
            std::stringstream ss_max(max_str);
            double maxX, maxY, maxZ;
            ss_max >> maxX >> comma >> maxY >> comma >> maxZ;
            maxP.setX(maxX);
            maxP.setY(maxY);
            maxP.setZ(maxZ);

            auto quad = std::make_shared<Quad>(minP, maxP, axis, value);
            if (materialMap.count(mat_id)) {
                quad->setMaterial(materialMap[mat_id]);
            }
            world->addEntity(quad);
        }
        else if (line.find("<point") != std::string::npos) {
            Vec3 position(
                parseDouble(getAttribute(line, "x")),
                parseDouble(getAttribute(line, "y")),
                parseDouble(getAttribute(line, "z"))
            );
            Color intensity(
                parseDouble(getAttribute(line, "r")),
                parseDouble(getAttribute(line, "g")),
                parseDouble(getAttribute(line, "b"))
            );
            auto light = std::make_shared<PointLight>(position, intensity);
            scene->addLight(light);
            }
        else if (line.find("<textured") != std::string::npos) {
            std::string id = getAttribute(line, "id");
            std::string texturePath = getAttribute(line, "path");
            double shininess = parseDouble(getAttribute(line, "shininess"));
            Texture texture(texturePath);
            auto mat = std::make_shared<MaterialTextured>(texture, shininess);
            materialMap[id] = mat;
        }
        else if (line.find("<mesh") != std::string::npos) {
            std::string path = getAttribute(line, "path");
            std::string mat_id = getAttribute(line, "material");

            double sx = parseDouble(getAttribute(line, "scaleX"));
            double sy = parseDouble(getAttribute(line, "scaleY"));
            double sz = parseDouble(getAttribute(line, "scaleZ"));
            double tx = parseDouble(getAttribute(line, "translateX"));
            double ty = parseDouble(getAttribute(line, "translateY"));
            double tz = parseDouble(getAttribute(line, "translateZ"));

            Vec3 scale(sx, sy, sz);
            Vec3 translate(tx, ty, tz);

            std::shared_ptr<Material> mat = nullptr;
            if (materialMap.count(mat_id)) {
                mat = materialMap[mat_id];
            }

            auto mesh = ObjectLoader::loadObj(path, mat, scale, translate);
            if (mesh) {
                world->addEntity(mesh);
            }
        }
        else if (line.find("<normalmapped") != std::string::npos) {
            std::string id = getAttribute(line, "id");
            Color ambient(parseDouble(getAttribute(line, "ambientR")),
                parseDouble(getAttribute(line, "ambientG")),
                parseDouble(getAttribute(line, "ambientB")));
            Color diffuse(parseDouble(getAttribute(line, "diffuseR")),
                parseDouble(getAttribute(line, "diffuseG")),
                parseDouble(getAttribute(line, "diffuseB")));
            Color specular(parseDouble(getAttribute(line, "specularR")),
                parseDouble(getAttribute(line, "specularG")),
                parseDouble(getAttribute(line, "specularB")));
            double shininess = parseDouble(getAttribute(line, "shininess"));
            std::string normalMapPath = getAttribute(line, "normalmap");

            Texture normalMap(normalMapPath);
            auto mat = std::make_shared<MaterialNormalMapped>(ambient, diffuse, specular, shininess, normalMap);
            materialMap[id] = mat;
            }





    }
    file.close();

    out_camera = std::make_unique<Camera>(aspect, width, samples);

    return scene;
}
