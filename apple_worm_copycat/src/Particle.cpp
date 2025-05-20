#include "Particle.h" 
#include <SDL.h>
#include <SDL_opengl.h>
#include <FreeImage.h>

#include <iostream>

Particle::Particle(double x, double y, double z) : Entity(x, y, z)  
{  
   this->setWidth(0.002f);
   this->setHeight(0.002f); 
   this->setDepth(0.002f);
   
   // Aumentamos significativamente la velocidad base de las partículas
   if (rand() % 2 == 0) {
       this->speedX = (rand() % 10) / 1000.0; // Aumentamos 10 veces la velocidad base
   }
   else {
       this->speedX = -(rand() % 10) / 1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedY = (rand() % 10) / 1000.0;
   }
   else {
       this->speedY = -(rand() % 10) / 1000.0;
   }
   if (rand() % 2 == 0) {
       this->speedZ = (rand() % 10) / 1000.0;
   }
   else {
       this->speedZ = -(rand() % 10) / 1000.0;
   }

   this->lifeTime = 60;
   this->isAlive = true;

   int textura = rand() % 3; // Selecciona una textura aleatoria
   switch (textura)
   {
   case 0:
	   this->texturaParticula = cargarTextura("assets/particle_texture/red.bmp");
	   break;
   case 1:
	   this->texturaParticula = cargarTextura("assets/particle_texture/green.bmp");
	   break;
   case 2:
	   this->texturaParticula = cargarTextura("assets/particle_texture/blue.bmp");
	   break;
   }
}  

Particle::~Particle()  
{  
	if (texturaParticula) {
		glDeleteTextures(1, &texturaParticula);
	}
}  

void Particle::render(bool texture)
{
    glPushMatrix();
    glTranslated(getX(), getY(), getZ());
    glScaled(getWidth(), getHeight(), getDepth());

    if (texture && lifeTime % 2 == 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texturaParticula);
         
    }

    glBegin(GL_QUADS);

    // Cara frontal
    glColor3f(1.0f, 1.0f, 1.0f);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.5);

    // Cara trasera
    glNormal3f(0.0f, 0.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);

    // Cara izquierda
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, -0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 0.5, -0.5);

    // Cara derecha
    glNormal3f(1.0f, 0.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 0.5, -0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, 0.5);

    // Cara superior
    glNormal3f(0.0f, 1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 0.5, -0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 0.5, 0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, 0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5, 0.5, -0.5);

    // Cara inferior
    glNormal3f(0.0f, -1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, -0.5, -0.5);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5, -0.5, -0.5);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5, -0.5, 0.5);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, -0.5, 0.5);

    glEnd();

    if (texture) {
        glDisable(GL_TEXTURE_2D);
    }

    glPopMatrix();
}



void Particle::update()
{
    if (this->isAlive) {
        // Aplicamos el multiplicador de velocidad al movimiento
        
        
        this->setX(this->getX() + speedX);
        this->setY(this->getY() + speedY);
        this->setZ(this->getZ() + speedZ);
        
        // Ajustamos el tiempo de vida según la velocidad
        this->lifeTime -= 1;
        if (this->lifeTime <= 0) {
            this->isAlive = false;
        }
    }
}

bool Particle::isDead() const
{
	return !this->isAlive;
}

GLuint Particle::cargarTextura(const char* path)
{
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
}


