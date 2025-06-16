#pragma once

#include <string>
#include <vector>

#include "Color.h"



class Texture {
public:
    Texture();  // Constructor vacío
    Texture(const std::string& filepath); // Constructor que carga desde archivo

    bool isLoaded() const;
    Color sample(double u, double v) const; // Devuelve el color de la textura en (u, v)

private:
    int width = 0;
    int height = 0;
    std::vector<Color> data; // Imagen cargada
    bool loaded = false;

    void loadFromFile(const std::string& filepath);
};