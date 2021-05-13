#pragma once

#include "vuv/datatypes.h"
#include <SDL2/SDL.h>

int vuv_render_setup_sdl();

int vuv_render_init(vuv_render_data *render);

int vuv_render_compile_shaders(vuv_render_data *render);

void vuv_render_clear(vuv_render_data *render);

void vuv_render_free(vuv_render_data *render);

int vuv_render_create_program(vuv_render_data *render);

void vuv_render_create_vertex_buffer(vuv_render_data *render);

void vuv_render_free_vertex_buffer(vuv_render_data *render);

int vuv_render_create_gpu_data(vuv_render_data *render);

void vuv_render_end_batch(vuv_render_data* render);

void vuv_render_draw_quad(vuv_render_data* render, vec2 position);
void vuv_render_test_triangle(vuv_render_data* render);

