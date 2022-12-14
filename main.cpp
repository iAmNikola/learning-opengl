#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        "Vvindous",
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
    glEnable(GL_DEPTH_TEST);

    // vertex object init
    float vertices[] = {
            -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    };
    float cube_vertices[] = {
            -0.5f, -0.5f, -0.5f,0.0f,0.0f,
            0.5f, -0.5f, -0.5f,1.0f,0.0f,
            0.5f, 0.5f, -0.5f,1.0f,1.0f,
            0.5f, 0.5f, -0.5f,1.0f,1.0f,
            -0.5f, 0.5f, -0.5f,0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,0.0f, 0.0f,

            -0.5f, -0.5f,0.5f,0.0f,0.0f,
            0.5f, -0.5f,0.5f,1.0f,0.0f,
            0.5f, 0.5f,0.5f,1.0f,1.0f,
            0.5f, 0.5f,0.5f,1.0f,1.0f,
            -0.5f, 0.5f,0.5f,0.0f,1.0f,
            -0.5f, -0.5f,0.5f,0.0f,0.0f,

            -0.5f, 0.5f, 0.5f,1.0f,0.0f,
            -0.5f, 0.5f, -0.5f,1.0f,1.0f,
            -0.5f, -0.5f, -0.5f,0.0f,1.0f,
            -0.5f, -0.5f, -0.5f,0.0f,1.0f,
            -0.5f, -0.5f, 0.5f,0.0f,0.0f,
            -0.5f, 0.5f, 0.5f,1.0f,0.0f,

            0.5f, 0.5f, 0.5f,1.0f,0.0f,
            0.5f, 0.5f, -0.5f,1.0f,1.0f,
            0.5f, -0.5f, -0.5f,0.0f,1.0f,
            0.5f, -0.5f, -0.5f,0.0f,1.0f,
            0.5f, -0.5f, 0.5f,0.0f,0.0f,
            0.5f, 0.5f, 0.5f,1.0f,0.0f,

            -0.5f,-0.5f, -0.5f,0.0f,1.0f,
            0.5f,-0.5f, -0.5f,1.0f,1.0f,
            0.5f,-0.5f, 0.5f,1.0f,0.0f,
            0.5f,-0.5f, 0.5f,1.0f,0.0f,
            -0.5f,-0.5f, 0.5f,0.0f,0.0f,
            -0.5f,-0.5f, -0.5f,0.0f,1.0f,

            -0.5f,0.5f, -0.5f,0.0f,1.0f,
            0.5f,0.5f, -0.5f,1.0f,1.0f,
            0.5f,0.5f, 0.5f,1.0f,0.0f,
            0.5f,0.5f, 0.5f,1.0f,0.0f,
            -0.5f,0.5f, 0.5f,0.0f,0.0f,
            -0.5f,0.5f, -0.5f,0.0f,1.0f
    };

    glm::vec3 cubes_pos[] = {
            glm::vec3( 0.0f, 0.0f,0.0f),
            glm::vec3( 2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f, 2.0f, -2.5f),
            glm::vec3( 1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
    };
    unsigned int VBO, VAO, EBO, texture0, texture1;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int img_w, img_h, num_channels;
    unsigned char *texture_data = stbi_load(
            "/home/wd-nikolad/projects/C/LearnOpenGL/spongbob.jpg",
            &img_w,
            &img_h,
            &num_channels,
            0
    );
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    texture_data = stbi_load(
            "/home/wd-nikolad/projects/C/LearnOpenGL/fish.jpg",
            &img_w,
            &img_h,
            &num_channels,
            0
    );
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_w, img_h, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(texture_data);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // tell opengl how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0 /* where to start reading data */);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
//    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
    transform = glm::rotate(transform, glm::radians(40.0f), glm::vec3(0.0f,0.0f,1.0f));
//    transform = glm::scale(transform, glm::vec3 (1.5f, 1.5f, 1.5f));

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-30.0f)*(float)SDL_GetTicks(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(80.0f), static_cast<float>(WINDOW_W) / static_cast<float>(WINDOW_H), 0.1f, 100.0f);

    Shader shader(
           "/home/wd-nikolad/projects/C/LearnOpenGL/shader.vs",
           "/home/wd-nikolad/projects/C/LearnOpenGL/shader.fs"
           );

    // draw loop
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    shader.use();
    shader.setInt("my_texture0", 0);
    shader.setInt("my_texture1", 1);

    unsigned int transform_location = glGetUniformLocation(shader.ID, "transform");
    glUniformMatrix4fv(transform_location, 1 , GL_FALSE, glm::value_ptr(transform));
    unsigned int model_location = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(model_location, 1 , GL_FALSE, glm::value_ptr(model));
    unsigned int view_location = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(view_location, 1 , GL_FALSE, glm::value_ptr(view));
    unsigned int projection_location = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projection_location, 1 , GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    bool quit {false};
    SDL_Event e;
    while (!quit) {
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindVertexArray(VAO);
        for (int i = 0; i < std::size(vertices); i++) {
            model = glm::mat4(1.0f);
//            transform = glm::rotate(transform, (float)SDL_GetTicks(), glm::vec3(0.0f, 0.0f, 1.0f));
//            glUniformMatrix4fv(transform_location, 1 , GL_FALSE, glm::value_ptr(transform));

            model = glm::translate(model, cubes_pos[i]);
            model = glm::rotate(model, glm::radians(30.0f*i), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);
        SDL_GL_SwapWindow(window);
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT){
                quit = true;
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

int window_size_changed(void *userdata, SDL_Event *event) {
    if (event->type == SDL_WINDOWEVENT && event->window.event == SDL_WINDOWEVENT_RESIZED) {
        auto* window = (SDL_Window*)userdata;
        int new_window_w, new_window_h;
        SDL_GetWindowSize(window, &new_window_w, &new_window_h);
        glViewport(0,0, new_window_w, new_window_h);
    }
    return 0;
}