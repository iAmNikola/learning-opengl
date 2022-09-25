#include <iostream>
#include <glad/glad.h>
#include <SDL2/SDL.h>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

const char* vertex_shader_source = "#version 330 core \n"
                                   "layout (location = 0) in vec3 pos;\n"
                                   "void main() { gl_Position = vec4(pos.x, pos.y, pos.z, 1.0); }";

const char* fragment_shader_source = "#version 330 core \n"
                                     "out vec4 FragColor;\n"
                                     "void main() { FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f); }";


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

    // vertex object init
    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // tell opengl how to interpret vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0 /* where to start reading data */);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // vertex shader init
    unsigned int vertex_shader {};
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    // check if vertex shader compiled
    int success {};
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
        printf("VERTEX SHADER ERROR: %s", info_log);
    }

    // fragment shader init
    unsigned int fragment_shader {};
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // check if fragment shader compiled
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
        printf("FRAGMENT SHADER ERROR: %s", info_log);
    }

    // init shader program
    unsigned int shader_program { glCreateProgram() };
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    // check if shader program linked correct
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader_program, 512, nullptr, info_log);
        printf("SHADER PROGRAM LINKING ERROR: %s", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);


    // draw loop
    bool quit {false};
    SDL_Event e;
    while (!quit) {
        glClearColor(0.0f, 0.2f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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