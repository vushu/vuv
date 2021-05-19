#pragma once

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_video.h>
#include <cglm/call.h>
#include <stdbool.h>

#include "vuv/nuklear.h"

typedef struct {
    float r, g, b, a;
} vuv_vector;

typedef struct {
    SDL_Window* sdl_window;
    SDL_GLContext gl_context;
    SDL_Event* event;
    struct nk_context* nk_context;

} vuv_context;

typedef struct {
    SDL_Event* event;
} vuv_event;

typedef struct {
    void (*key_down)(void*, SDL_Event);

    void (*key_up)(void*, SDL_Event);

} vuv_input;

typedef struct {
    char* title;
    int height, width;

} vuv_window_data;

typedef struct {
    vec3 position[3];
    vec4 color[4];
    vec2 texture_coord[2];
    float texture_id;
    float type_id;
} vuv_render_vertex;

typedef struct {
    vec2 position;
    mat4 project_matrix;
    mat4 view_matrix;
    mat4 inverse_view_matrix;
    mat4 inverse_project_matrix;
    float width, height, speed;

} vuv_camera;

typedef struct {
    vuv_vector clear_color;
    bool vsync_on;
    //GPU DATA
    const char* vertex_shader_src;
    const char* fragment_shader_src;
    unsigned int compiled_vertex_shader,
        compiled_fragment_shader,
        shader_program,
        gl_VA, gl_VB, gl_EB;

    vuv_render_vertex* vertex_buffer;
    vuv_render_vertex* vertex_buffer_ptr;
    vuv_camera* camera;
    uint32_t render_indices_count;

} vuv_render_data;

typedef struct {
    int state;
    vuv_context* context;
    vuv_input* input;
    vuv_render_data* render;
    vuv_window_data* window;
    void (*game_loop_callback)(void*);

} vuv_application;
