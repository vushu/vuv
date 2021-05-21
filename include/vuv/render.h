#pragma once

#include "cglm/types.h"
#include "vuv/datatypes.h"

int vuv_render_setup_sdl();

int vuv_render_init(vuv_render_data *render, int win_width, int win_height);

int vuv_render_compile_shaders(vuv_render_data *render);

void vuv_render_clear(vuv_render_data *render);

void vuv_render_free(vuv_render_data *render);

int vuv_render_create_program(vuv_render_data *render);

void vuv_render_create_vertex_buffer(vuv_render_data *render);

void vuv_render_free_vertex_buffer(vuv_render_data *render);

int vuv_render_create_shader_data(vuv_render_data *render);

void vuv_render_begin_batch(vuv_render_data* render);
void vuv_render_end_batch(vuv_render_data* render);
void vuv_render_flush_batch(vuv_render_data* render);
void vuv_render_end_flush_begin_batch(vuv_render_data* render);

void vuv_render_draw_quad(vuv_render_data* render, vec2 position, vec2 size, vec4 color);
void vuv_render_nk_draw();
void vuv_render_nk_clear(vec4 color);
void vuv_render_test_triangle(vuv_render_data* render);
void vuv_render_init_shader(vuv_render_data* render);
void flush2(vuv_render_data* render);

