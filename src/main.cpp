#include <stdio.h>
#include <stdlib.h>

// clang-format off
#include <HandmadeMath.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
// clang-format on

#include "resource_manager.h"
#include "sprite_renderer.h"

#define ZOOM_SPEED 0.03
#define DRAG_SPEED 0.1
const int WINDOW_WIDTH = 3840 / 2;
const int WINDOW_HEIGHT = 2160 / 2;
const bool WINDOW_FULLSCREEN = true;

static void sdl_die(const char *message);

void debug_message_callback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam);

int main(int argc, char *argv[]) {
    atexit(SDL_Quit);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        sdl_die("Couldn't initialize SDL");
    }

    SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 16);

    SDL_GL_SetSwapInterval(1);

    SDL_Window *window;
    if (WINDOW_FULLSCREEN) {
        window = SDL_CreateWindow(
            "LearnOpenGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0,
            0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    } else {
        window = SDL_CreateWindow("LearnOpenGL", SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                                  WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    }

    if (window == NULL) {
        sdl_die("Couldn't set video mode");
    }

    if (!SDL_GL_CreateContext(window)) {
        sdl_die("Failed to create OpenGL context");
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        sdl_die("Failed to initialize GLAD");
    }

    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debug_message_callback, 0);

    auto sprite = ResourceManager::load_shader("sprite", "..\\src\\shader.vs",
                                               "..\\src\\shader.fs");
    auto tapir = ResourceManager::load_texture("tapir", "..\\tapir.png", false);

    auto projection =
        HMM_Orthographic(0.0f, static_cast<float>(WINDOW_WIDTH),
                         static_cast<float>(WINDOW_HEIGHT), 0.0f, -1.0f, 1.0f);
    sprite.use();
    sprite.integer("sprite", 0);
    sprite.matrix4("projection", projection);

    auto *sr = new SpriteRenderer(ResourceManager::get_shader("sprite"));
    float x = 100.0;
    float y = 10.0;

    auto size = HMM_Vec2(500.0f, 500.0f);
    auto rotation_degrees = 0.0f;

    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = 0;
    double dt = 0.0;
    double test = 0.0;

    SDL_Event event;
    bool quit = false;
    while (!quit) {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (double)((NOW - LAST) / (double)SDL_GetPerformanceFrequency());
        test += dt;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        y += 5 * sin(test);

        rotation_degrees += 500 * dt;
        sr->draw(tapir, HMM_Vec2(x, y), size);
        sr->draw(tapir, HMM_Vec2(1000.0f, 500.0f),
                 HMM_Vec2(size.X - 100, size.Y - 100), rotation_degrees);

        SDL_GL_SwapWindow(window);
    }

    SDL_Quit();
    return 0;
}

static void sdl_die(const char *message) {
    fprintf(stderr, "%s: %s\n", message, SDL_GetError());
    exit(2);
}

void debug_message_callback(GLenum source, GLenum type, GLuint id,
                            GLenum severity, GLsizei length,
                            const GLchar *message, const void *userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}
