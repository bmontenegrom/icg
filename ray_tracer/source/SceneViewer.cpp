// SceneViewer.cpp - Visualizador OpenGL para raytracer

#include "SceneLoader.h"
#include "Camera.h"
#include "WhittedTracer.h"
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <vector>
#include <iostream>

SDL_Window* window = nullptr;
SDL_GLContext gl_context;
GLuint texture_id;
std::vector<Color> framebuffer;

bool initializeOpenGL(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error al inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow("Raytracer Viewer",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_OPENGL);

    if (!window) {
        std::cerr << "No se pudo crear la ventana: " << SDL_GetError() << std::endl;
        return false;
    }

    gl_context = SDL_GL_CreateContext(window);
    if (!gl_context) {
        std::cerr << "No se pudo crear el contexto OpenGL: " << SDL_GetError() << std::endl;
        return false;
    }

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    return true;
}

void drawTexture(int width, int height) {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0); glVertex2f(0, 0);
    glTexCoord2f(1, 0); glVertex2f(width, 0);
    glTexCoord2f(1, 1); glVertex2f(width, height);
    glTexCoord2f(0, 1); glVertex2f(0, height);
    glEnd();

    SDL_GL_SwapWindow(window);
}

void updateTextureRow(int row, int width) {
    std::vector<unsigned char> row_data(width * 3);
    for (int i = 0; i < width; ++i) {
        const Color& c = framebuffer[row * width + i];
        row_data[i * 3 + 0] = c.getRbyte();
        row_data[i * 3 + 1] = c.getGbyte();
        row_data[i * 3 + 2] = c.getBbyte();
    }
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, row, width, 1, GL_RGB, GL_UNSIGNED_BYTE, row_data.data());
}

void mainLoop(const Scene& scene, Camera& camera, const WhittedTracer& tracer) {
    bool quit = false;
    SDL_Event e;
    int width = camera.getImageWidth();
    int height = camera.getImageHeight();
    int current_row = 0;

    framebuffer.resize(width * height);

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        if (current_row < height) {
            camera.renderRow(current_row, scene, tracer, framebuffer);
            updateTextureRow(current_row, width);
            drawTexture(width, height);
            ++current_row;
        }
    }
}
/*
int main(int argc, char* argv[]) {
    std::unique_ptr<Camera> camera;
    WhittedTracer tracer(10, 0.001);
    std::shared_ptr<Scene> scene = SceneLoader::loadFromXML("assets/scenes/planetaTierra.xml", camera, tracer);

    if (!scene || !camera) {
        std::cerr << "Error al cargar escena\n";
        return -1;
    }

    int width = camera->getImageWidth();
    int height = camera->getImageHeight();

    if (!initializeOpenGL(width, height)) return -1;

    mainLoop(*scene, *camera, tracer);

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
*/