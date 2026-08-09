#include "app.h"

#include <Windows.h>

int main() {
    const app_config_t config = {
        "WBFA 3",
        960,
        540,
        WEBVIEW_HINT_FIXED
    };

    app_t app;
    app_initialize(&app, &config);
    return app_run(&app);
}