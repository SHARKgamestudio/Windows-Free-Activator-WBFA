#pragma once

#include <webview/webview_core.h>

typedef struct app_config {
    const char title[32];
    const int width;
    const int height;
    const webview_hint_t hint;
} app_config_t;

typedef struct app {
    const app_config_t* config_ptr;

    webview_t webv_inst;
} app_t;

void app_initialize(app_t* app, const app_config_t* config);
int app_run(app_t* app);