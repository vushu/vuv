#pragma once
#include "datatypes.h"
#include <stdbool.h>

void vuv_input_listen(vuv_application* app);

bool vuv_input_key_up(vuv_application* app);

bool vuv_input_key_down(vuv_application* app);

bool vuv_input_key_escape(vuv_application* app);
