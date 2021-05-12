#include "vuv/render.h"
#include "SDL_log.h"
#include "SDL_video.h"
#include "vuv/datatypes.h"
#include "vuv/errors.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_QUAD_COUNT 100
#define MAX_INDEX_COUNT MAX_QUAD_COUNT * 6
#define MAX_VERTEX_COUNT MAX_QUAD_COUNT * 4

int checkGLError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        break;
    }
    return err;
}

void begin_batch(vuv_render_data *render) {
    render->vertex_buffer_ptr = render->vertex_buffer;
}

void end_batch(vuv_render_data *render) {
    GLsizeiptr size = (uint8_t *) render->vertex_buffer_ptr - (uint8_t *) render->vertex_buffer;
    glBindBuffer(GL_ARRAY_BUFFER, render->gl_VB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, render->vertex_buffer);
}

void create_gpu_data_mat4(int program_id, char *var_name, mat4 matrix) {
    glUniformMatrix4fv(glGetUniformLocation(program_id, var_name), 1, GL_FALSE, matrix);
}

void create_gpu_data_mat3(int program_id, char *var_name, mat3 matrix) {
    glUniformMatrix3fv(glGetUniformLocation(program_id, var_name), 1, GL_FALSE, matrix);
}

void create_gpu_data_vec3(int program_id, char *var_name, vec3 vec) {
    glUniform3fv(glGetUniformLocation(program_id, var_name), 1, vec);
}

void create_gpu_data_vec2(int program_id, char *var_name, vec2 vec) {
    glUniform2fv(glGetUniformLocation(program_id, var_name), 1, vec);
}

void create_gpu_data_float(int program_id, char *var_name, float value) {
    glUniform1f(glGetUniformLocation(program_id, var_name), value);
}

char *read_file(char *fn) {

    FILE *fp;
    char *content = NULL;

    int count = 0;

    if (fn != NULL) {
        fp = fopen(fn, "rt");

        if (fp != NULL) {

            fseek(fp, 0, SEEK_END);
            count = ftell(fp);
            rewind(fp);

            if (count > 0) {
                content = (char *) malloc(sizeof(char) * (count + 1));
                count = fread(content, sizeof(char), count, fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

void set_render_vertex(vuv_render_data *render, vec2 position, vec2 texture_coord) {
    // test no texture
    render->vertex_buffer_ptr->position[0] = position[0];
    render->vertex_buffer_ptr->position[1] = position[1];
    render->vertex_buffer_ptr->position[2] = 0.0f;

    render->vertex_buffer_ptr->color[0] = 1.0f;
    render->vertex_buffer_ptr->color[1] = 0.0f;
    render->vertex_buffer_ptr->color[2] = 0.0f;
    render->vertex_buffer_ptr->color[3] = 1.0f;

    render->vertex_buffer_ptr->texture_id = 0.0f;
    render->vertex_buffer_ptr->type_id = 0.0f;

    render->vertex_buffer_ptr->texture_coord[0] = texture_coord[0];
    render->vertex_buffer_ptr->texture_coord[1] = texture_coord[1];
    render->vertex_buffer_ptr++;

}

void set_render_vertices(vuv_render_data *render, vec2 position, vec2 size) {

    //BR
    vec2 bot_right;
    bot_right[0] = position[0] + size[0];
    bot_right[1] = position[1] + size[1];
    vec2 bot_right_texture_coord;
    bot_right_texture_coord[0] = 1.0f;
    bot_right_texture_coord[1] = 1.0f;

    //TR
    vec2 top_right;
    top_right[0] = position[0] + size[0];
    top_right[1] = position[1];
    vec2 top_right_texture_coord;
    top_right_texture_coord[0] = 1.0f;
    top_right_texture_coord[1] = 0.0f;


    //TL
    vec2 top_left;
    top_left[0] = position[0];
    top_left[1] = position[1];
    vec2 top_left_texture_coord;
    top_left_texture_coord[0] = 0.0f;
    top_left_texture_coord[1] = 0.0f;


    //BL
    vec2 bot_left;
    bot_left[0] = position[0];
    bot_left[1] = position[1] + size[1];
    vec2 bot_left_texture_coord;
    bot_left_texture_coord[0] = 0.0f;
    bot_left_texture_coord[1] = 1.0f;


    set_render_vertex(render, bot_right, bot_right_texture_coord); //BR
    set_render_vertex(render, top_right, top_right_texture_coord); //TR
    set_render_vertex(render, top_left, top_left_texture_coord); //TL
    set_render_vertex(render, bot_left, bot_left_texture_coord); //BL

}

void increment_number_of_vertices(vuv_render_data *render) {
    render->render_indices_count += 6;
}

int read_shaders(vuv_render_data *render) {
    render->vertex_shader_src = read_file("/home/vushu/coding/c/vuv/resources/shaders/shader.vert");
    render->fragment_shader_src = read_file("/home/vushu/coding/c/vuv/resources/shaders/shader.frag");
    return VUV_OK;
}

void get_view_matrix(vuv_render_data *render) {
}

void flush(vuv_render_data *render) {
    glUseProgram(render->shader_program);

    create_gpu_data_mat4(render->shader_program, "uMvp", GLM_MAT4_IDENTITY);

    //texture bind no implemented yet

    glBindVertexArray(render->gl_VA);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    glDrawElements(GL_TRIANGLES, render->render_indices_count, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    glBindVertexArray(0);
    // unbind texture here later
    glUseProgram(0);
    render->render_indices_count = 0;

}

void check_maximum_render(vuv_render_data *render) {
    if (render->render_indices_count >= MAX_INDEX_COUNT) {
        end_batch(render);
        flush(render);
        begin_batch(render);
    }
}

int vuv_render_setup_sdl() {
    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return VUV_SDL_INIT_FAILED;
    }

    // Request OpenGL ES 3.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    // Want double-buffering
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return VUV_OK;
}

int vuv_render_compile_shaders(vuv_render_data *render) {
    render->compiled_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(render->compiled_vertex_shader, 1, &render->vertex_shader_src, NULL);
    glCompileShader(render->compiled_vertex_shader);
    int success;
    char infoLog[512];
    glGetShaderiv(render->compiled_vertex_shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(render->compiled_vertex_shader, 512, NULL, infoLog);
        printf("COMPILE MESSAGE: %s", infoLog);
        return VUV_RENDER_SHADER_FAILED;
    }

    render->compiled_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(render->compiled_fragment_shader, 1, &render->fragment_shader_src, NULL);
    glCompileShader(render->compiled_fragment_shader);
    glGetShaderiv(render->compiled_fragment_shader, GL_COMPILE_STATUS, &success);

    if (!success)
        return VUV_RENDER_SHADER_FAILED;
    return VUV_OK;
}

int vuv_render_init(vuv_render_data *render) {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        return VUV_GLAD_FAILED;
    }

    if (read_shaders(render) < 0) {
        return VUV_FAIL;
    }
    render->clear_color.r = 0.08f;
    render->clear_color.g = 0.08f;
    render->clear_color.b = 0.09f;
    render->clear_color.a = 1.0f;

    render->camera = malloc(sizeof(vuv_camera));
    if (vuv_render_compile_shaders(render) < 1) {
        return VUV_RENDER_SHADER_FAILED;
    }
    if (vuv_render_create_program(render) < 1) {
        return VUV_RENDER_PROGRAM_FAILED;
    }

    if (vuv_render_create_gpu_data(render) < 1) {
        return VUV_RENDER_GPU_DATA_FAILED;
    }
    return VUV_OK;

}

int vuv_render_create_program(vuv_render_data *render) {
    render->shader_program = glCreateProgram();

    glAttachShader(render->shader_program, render->compiled_vertex_shader);
    glAttachShader(render->shader_program, render->compiled_fragment_shader);
    glLinkProgram(render->shader_program);

    int success;
    char infoLog[512];

    glGetProgramiv(render->shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(render->shader_program, 512, NULL, infoLog);
        printf("PROGRAM FAILED: %s", infoLog);
        return VUV_RENDER_PROGRAM_FAILED;
    }

    glDeleteShader(render->compiled_vertex_shader);
    glDeleteShader(render->compiled_fragment_shader);

    return VUV_OK;
}

void vuv_render_clear(vuv_render_data *render) {
    if (render->vsync_on) {
        SDL_GL_SetSwapInterval(1);
    }

    glClearColor(render->clear_color.r, render->clear_color.g, render->clear_color.b, render->clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void vuv_render_free(vuv_render_data *render) {
    glDeleteProgram(render->shader_program);
    vuv_render_free_vertex_buffer(render);
    free(render->camera);
    free(render->vertex_shader_src);
    free(render->fragment_shader_src);

    free(render);
}

void vuv_render_create_vertex_buffer(vuv_render_data *render) {
    render->vertex_buffer = malloc(MAX_VERTEX_COUNT * sizeof(vuv_render_vertex));
}

void vuv_render_free_vertex_buffer(vuv_render_data *render) {
    if (render && render->vertex_buffer != NULL)
        free(render->vertex_buffer);
}

int vuv_render_create_gpu_data(vuv_render_data *render) {

    vuv_render_create_vertex_buffer(render);

    glGenVertexArrays(1, &render->gl_VA);
    glBindVertexArray(render->gl_VA);

    glGenBuffers(1, &render->gl_VB);
    glBindBuffer(GL_ARRAY_BUFFER, &render->gl_VB);

    glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(vuv_render_vertex), NULL, GL_DYNAMIC_DRAW);

    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        glDeleteBuffers(1, &render->gl_VB);
        return VUV_RENDER_GPU_DATA_FAILED;
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vuv_render_vertex),
                          (const GLvoid *) offsetof(vuv_render_vertex, position));
    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vuv_render_vertex),
                          (const GLvoid *) offsetof(vuv_render_vertex, color));
    //texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vuv_render_vertex),
                          (const GLvoid *) offsetof(vuv_render_vertex, texture_coord));
    //texture_id
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vuv_render_vertex),
                          (const GLvoid *) offsetof(vuv_render_vertex, texture_id));
    //type
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(vuv_render_vertex),
                          (const GLvoid *) offsetof(vuv_render_vertex, type_id));

    //disable for good measure
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    uint32_t indices[MAX_INDEX_COUNT];
    uint32_t offset = 0;

    for (unsigned int i = 0; i < MAX_INDEX_COUNT; i += 6) {

        indices[i + 0] = 3 + offset;
        indices[i + 1] = 2 + offset;
        indices[i + 2] = 0 + offset;

        indices[i + 3] = 0 + offset;
        indices[i + 4] = 2 + offset;
        indices[i + 5] = 1 + offset;

        offset += 4;
    }
    glGenBuffers(1, &render->gl_IB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, &render->gl_IB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Done generating
    //Batch data

    GLenum erre = glGetError();
    if (erre != GL_NO_ERROR) {
        printf("error %s", erre);
    }
    // remember to close very important or other attributes made by others is overwritting it
    glBindVertexArray(0);

    begin_batch(render);
    return VUV_OK;
}

void vuv_render_end_batch(vuv_render_data *render) {
    end_batch(render);
    flush(render);
    begin_batch(render);
}

void vuv_render_draw_quad(vuv_render_data *render) {
    check_maximum_render(render);
    vec2 position, size;
    position[0] = 10.0f;
    position[1] = 10.0f;
    size[0] = 50.0f;
    size[1] = 50.0f;
    set_render_vertices(render, position, size);
    increment_number_of_vertices(render);
}