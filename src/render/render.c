#include "vuv/render.h"
#include "SDL_log.h"
#include "SDL_video.h"
#include "vuv/datatypes.h"
#include "vuv/errors.h"
#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTEX_COUNT 1000

int checkGLError() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        break;
    }
    return err;
}

void begin_batch(vuv_render_data *render) {
    render->current_pointer_of_vertex_buffer = render->vertex_buffer;
}

void end_batch(vuv_render_data *render) {
    GLsizeiptr size = (uint8_t *) render->current_pointer_of_vertex_buffer - (uint8_t) render->vertex_buffer;
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

int read_shaders(vuv_render_data *render) {
    FILE *fp;
    char buff[255];

    fp = fopen("/home/vushu/coding/c/vuv/resources/shaders/shader.vert", "r");

    if (fp == NULL) {
        return VUV_RENDER_READ_SHADER_FAILED;
    }

    fgets(render->vertex_shader_src, 255, fp);
    fp = fopen("/home/vushu/coding/c/vuv/resources/shaders/shader.frag", "r");
    fgets(render->fragment_shader_src, 255, fp);
    fclose(fp);
    return VUV_OK;
}


void flush(vuv_render_data *render) {
    glUseProgram(render->shader_program);
//    create_gpu_data_mat4(render->shader_program, "uMvp", render->camera->view_matrix);
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

    return vuv_render_create_program(render);
}

int vuv_render_init(vuv_render_data *render) {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        return VUV_GLAD_FAILED;
    }

//    render->vertex_shader_src = "#version 300 es precision highp float;precision highp int;in vec3 aPos;in vec4 aColor;in vec2 aTexCoords;in float aTexId;in float aType;uniform mat4 uMvp;out vec4 fColor;out vec2 fTexCoords;out float fTexId;out float fType;void main() {    fColor = aColor;    fTexCoords = aTexCoords;    fTexId = aTexId;    fType = aType;    gl_Position = uMvp * vec4(aPos, 1.0);}";
//    render->fragment_shader_src = "#version 300 es precision highp float;precision highp int;in vec4 fColor;in vec2 fTexCoords;in float fTexId;in float fType;uniform sampler2D uTextures[8];out vec4 color;void main () {    //color = vec4( 0.6, 1.0, 1.0, 1.0 );    //color = fColor;    //color = texture(uTex, fTexCoords);    //color = fColor * texture(uTextures[1], fTexCoords);    vec4 tempColor = vec4(1.0,1.0,1.0,1.0);    if (fTexId == 0.0f) {        color = fColor;    }    else if (fTexId == 1.0f) {        tempColor = texture(uTextures[1], fTexCoords);        //tempColor = vec4(vec3(1.0 - texture(uTextures[1], fTexCoords)), 1.0);    }    else if (fTexId == 2.0f) {        tempColor = texture(uTextures[2], fTexCoords);        //tempColor = vec4(vec3(1.0 - texture(uTextures[1], fTexCoords)), 1.0);    }    else if (fTexId == 3.0f) {        tempColor = texture(uTextures[3], fTexCoords);    }    else if (fTexId == 4.0f) {        tempColor = texture(uTextures[4], fTexCoords);    }    else if (fTexId == 5.0f) {        tempColor = texture(uTextures[5], fTexCoords);    }    else if (fTexId == 6.0f) {        tempColor = texture(uTextures[6], fTexCoords);    }    else if (fTexId == 7.0f) {        tempColor = texture(uTextures[7], fTexCoords);    }    if (fType == 0.0f){        color = fColor * tempColor;        //color.rgb = (fColor.rgb) + (tempColor.rgb * (1.0f - fColor.a)) ;        //color.rgb = (tempColor.rgb) + (fColor.rgb * (1.0f - tempColor.a)) ;    }    //font    else if (fType == 1.0f){        color = fColor * tempColor.a;    }}";
    render->vertex_shader_src = malloc(255);
    render->fragment_shader_src = malloc(255);
    if (read_shaders(render) < 0) {
        return VUV_FAIL;
    }
    render->clear_color.r = 0.0f;
    render->clear_color.g = 0.3f;
    render->clear_color.b = 0.3f;
    render->clear_color.a = 1.0f;

    render->camera = malloc(sizeof(vuv_camera));
    return vuv_render_compile_shaders(render);
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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
            0.5f, 0.5f, 0.0f,   // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f, // bottom left
            -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {
            // note that we start from 0!
            0, 1, 3, // first Triangle
            1, 2, 3  // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    render->VBO = VBO;
    render->VAO = VAO;
    render->EBO = EBO;

    vuv_render_create_vertex_buffer(render);


    return VUV_OK;
}

void vuv_render_clear(vuv_render_data *render) {
    if (render->vsync_on) {
        SDL_GL_SetSwapInterval(1);
    }

    glClearColor(render->clear_color.r, render->clear_color.g, render->clear_color.b, render->clear_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}


void vuv_render_draw(vuv_application *app) {
    glUseProgram(app->render->shader_program);
    glBindVertexArray(app->render->VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    SDL_GL_SwapWindow(app->context->sdl_window);
}

void vuv_render_free(vuv_render_data *render) {
    glDeleteVertexArrays(1, &render->VAO);
    glDeleteBuffers(1, &render->VBO);
    glDeleteBuffers(1, &render->EBO);
    glDeleteProgram(render->shader_program);
    vuv_render_free_vertex_buffer(render->vertex_buffer);
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
    glGenVertexArrays(1, &render->gl_VA);
    glad_glBindVertexArray(render->gl_VA);

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
                          (const GLvoid *) offsetof(vuv_render_vertex, texture_coords));
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

    uint32_t indices[MAX_VERTEX_COUNT];
    uint32_t offset = 0;

    for (unsigned int i = 0; i < MAX_VERTEX_COUNT; i += 6) {

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
}

void vuv_render_end_batch(vuv_render_data *render) {
    end_batch(render);
    flush(render);
    begin_batch(render);
}