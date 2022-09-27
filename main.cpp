#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const int WINDOW_W = 800;
const int WINDOW_H = 600;


int window_size_changed(void *userdata, SDL_Event *event);

int main(int argc, char* argv[]) {
    SDL_Init( SDL_INIT_EVERYTHING );

    // Create SDL window
    SDL_Window* window = SDL_CreateWindow(
        "Hello ZORAN",
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

    // vertex object init
    float vertices[] = {
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 4
    };

    int img_w, img_h, num_channels;
    unsigned char *texture_data = stbi_load(
            "/home/wd-nikolad/projects/C/LearnOpenGL/spongbob.jpg",
            &img_w,
            &img_h,
            &num_channels,
            0
    );

    unsigned int VBO, VAO, EBO, texture;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // tell opengl how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0 /* where to start reading data */);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    Shader shader(
           "/home/wd-nikolad/projects/C/LearnOpenGL/shader.vs",
           "/home/wd-nikolad/projects/C/LearnOpenGL/shader.fs"
           );

    // draw loop
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    bool quit {false};
    SDL_Event e;
    while (!quit) {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        SDL_GL_SwapWindow(window);
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
    }
    return 0;
}