#pragma once
#include "cglm/call/vec3.h"
#include "cglm/types.h"
void create_gpu_data_mat4(int program_id, char *var_name, mat4 matrix);

void create_gpu_data_mat3(int program_id, char *var_name, mat3 matrix);

void create_gpu_data_vec3(int program_id, char *var_name, vec3 vec);

void create_gpu_data_vec2(int program_id, char *var_name, vec2 vec);

void create_gpu_data_float(int program_id, char *var_name, float value);
