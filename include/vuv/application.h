#pragma once
#include "datatypes.h"

vuv_application* vuv_application_create(char* title, int width, int height);

int vuv_application_init(vuv_application* app);

void vuv_application_game_loop(vuv_application* app);

void vuv_application_destroy(vuv_application* vuv_app);


