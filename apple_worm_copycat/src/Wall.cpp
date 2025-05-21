#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include <iostream>
#include <FreeImage.h>
#include "Wall.h"
#include "ObjLoader.h"
#include "Constants.h"

Wall::Wall(double x, double y, double z, double width, double height, double depth)
    : Entity(x, y, z, width, height, depth) {
    ObjectLoader& loader = ObjectLoader::getInstance();
    // Escalar el modelo a 0.095f para que coincida con el tamaño del gusano
    vertices = loader.loadOBJ("assets/cube_brick/Cube.obj", 0.5f);
	texturaMuro = cargarTextura("assets/cube_brick/Cube_texture.png");
}

void Wall::render( bool texture) {
    

    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(getWidth(), getHeight(), getDepth());

    glColor3f(1.0f, 1.0f, 1.0f); // Color blanc
        
    // Renderizar el modelo cargado
	if (texturaMuro != 0 && texture) {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texturaMuro);
	}
        
    glBegin(GL_TRIANGLES);
		
    for (const auto& v : vertices) {
	    if (texturaMuro != 0 && texture) {
			glTexCoord2f(v.texCoord.x, v.texCoord.y);
		}
        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
        glVertex3f(v.position.x, v.position.y, v.position.z);
    }

    glEnd();
    if (texturaMuro != 0 && texture) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}

EntityType Wall::getType() const
{
    return EntityType::WALL;
}

GLuint Wall::cargarTextura(const char* path)
{
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(path, 0);
    if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(path);
    if (fif == FIF_UNKNOWN) return 0;

    FIBITMAP* dib = FreeImage_Load(fif, path);
    if (!dib) return 0;

    FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(dib);
    if (!dib32) return 0;

    int width = FreeImage_GetWidth(dib32);
    int height = FreeImage_GetHeight(dib32);
    FreeImage_FlipVertical(dib32);

    BYTE* bits = FreeImage_GetBits(dib32);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, bits);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    FreeImage_Unload(dib32);
    return texID;;
}
