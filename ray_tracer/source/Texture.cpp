#include "Texture.h"
#include "Constants.h"
#include <FreeImage.h>
#include <iostream>
#include <algorithm>

Texture::Texture() {}

Texture::Texture(const std::string& filepath) {
    loadFromFile(filepath);
}

bool Texture::isLoaded() const {
    return loaded;
}

void Texture::loadFromFile(const std::string& filepath) {
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filepath.c_str(), 0);
    if (format == FIF_UNKNOWN) {
        format = FreeImage_GetFIFFromFilename(filepath.c_str());
    }

    if (format == FIF_UNKNOWN) {
        std::cerr << "Formato desconocido para la textura: " << filepath << std::endl;
        return;
    }

    FIBITMAP* bitmap = FreeImage_Load(format, filepath.c_str());
    if (!bitmap) {
        std::cerr << "No se pudo cargar la textura: " << filepath << std::endl;
        return;
    }

    FIBITMAP* bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_Unload(bitmap);

    width = FreeImage_GetWidth(bitmap32);
    height = FreeImage_GetHeight(bitmap32);

    data.resize(width * height);

    for (int y = 0; y < height; ++y) {
        BYTE* bits = FreeImage_GetScanLine(bitmap32, y);
        for (int x = 0; x < width; ++x) {
            float r = bits[FI_RGBA_RED] / 255.0f;
            float g = bits[FI_RGBA_GREEN] / 255.0f;
            float b = bits[FI_RGBA_BLUE] / 255.0f;
            data[y * width + x] = Color(r, g, b);
            bits += 4;
        }
    }

    loaded = true;
    FreeImage_Unload(bitmap32);
}

Color Texture::sample(double u, double v) const {
    if (!loaded || width == 0 || height == 0) {
        return Color(1, 0, 1); // Magenta: error
    }

    u = clamp(u, 0.0, 1.0);
    v = clamp(v, 0.0, 1.0);

    int x = std::min(int(u * width), width - 1);
    int y = std::min(int((1.0f - v) * height), height - 1); // invertir v: FreeImage es bottom-up

    return data[y * width + x];
}