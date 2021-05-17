#pragma once
#include "datatypes.h"

vuv_application* vuv_application_create(char* title, int width, int height);

int vuv_application_init(vuv_application* app);

void vuv_application_destroy(vuv_application* vuv_app);

void vuv_application_run(vuv_application* vuv_app);

