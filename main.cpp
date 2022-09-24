#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

int window_size_changed(void *userdata, SDL_Event *event);


int main(int argc, char* argv[]) {
    SDL_Init( SDL_INIT_EVERYTHING );

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow(
        "Hello Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_W,
        WINDOW_H,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );
    if (window == nullptr) {
        printf("Couldn't create window\n");
        return -1;
    }
    // get GL context
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (gl_context == nullptr){
        printf("Couldn't create OpenGL context!\n");
        return -1;
    }
    SDL_GL_SetSwapInterval(1);

    //load GLAD
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to init GLAD\n");
        return -1;
    }
    // set OpenGL where to draw
    glViewport(0,0,WINDOW_W, WINDOW_H);
    SDL_AddEventWatch(window_size_changed, window);
    bool quit {false};
    SDL_Event e;
    while (!quit) {
        SDL_GL_SwapWindow(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int window_size_changed(void *userdata, SDL_Event *event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        auto* window = (SDL_Window*)userdata;
        int new_window_w, new_window_h;
        SDL_GetWindowSize(window, &new_window_h, &new_window_w);
        glViewport(0,0, new_window_w, new_window_h);
        printf("Resize event.\n");
    }
    return 0;
}