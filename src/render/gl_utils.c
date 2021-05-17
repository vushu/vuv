#include "vuv/gl_utils.h"
#include <glad/glad.h>

void create_gpu_data_mat4(int program_id, char *var_name, mat4 matrix) {
    unsigned int id = glGetUniformLocation(program_id, var_name);
    glUniformMatrix4fv(id, 1, GL_FALSE, matrix[0]);
}

void create_gpu_data_mat3(int program_id, char *var_name, mat3 matrix) {
    glUniformMatrix3fv(glGetUniformLocation(program_id, var_name), 1, GL_FALSE, matrix[0]);
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


