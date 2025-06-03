#include <FreeImage.h>
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Constants.h"

std::string getTimestampedFilename() {
    std::time_t now = std::time(nullptr);
    std::tm tm_info{};

    // localtime_s(destination, source)
    if (localtime_s(&tm_info, &now) != 0) {
        std::cerr << "Error al obtener la hora local.\n";
        return "images/output_error.png";
    }

    std::ostringstream oss;
    oss << "images/output_"
        << std::put_time(&tm_info, "%Y-%m-%d_%H-%M-%S")
        << ".png";

    return oss.str();
}

int main() {
    FreeImage_Initialise();


    FIBITMAP* bitmap = FreeImage_Allocate(WIDTH, HEIGHT, 24);

    if (!bitmap) {
        std::cerr << "No se pudo crear el bitmap.\n";
        FreeImage_DeInitialise();
        return 1;
    }

    
	for (int y = 0; y < HEIGHT; ++y) {
		BYTE* bits = FreeImage_GetScanLine(bitmap, y);
		for (int x = 0; x < WIDTH; ++x) {
			double red = double(x)/(WIDTH-1);
			double green = double(HEIGHT-1-y) / (HEIGHT - 1);
			double blue = 0.0;

            //freeimage es bgr!!!
            bits[x * 3 + 0] = int(255.999 * blue);
			bits[x * 3 + 1] = int(255.999 * green);
			bits[x * 3 + 2] = int(255.999 * red);
		}
	}


    std::string filename = getTimestampedFilename();
    if (FreeImage_Save(FIF_PNG, bitmap, filename.c_str(), PNG_DEFAULT)) {
        std::cout << "Imagen guardada como: " << filename << "\n";
    }
    else {
        std::cerr << "Error al guardar la imagen.\n";
    }

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    return 0;
}
