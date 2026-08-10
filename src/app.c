#include "app.h"

#include <webview/webview_core.h>
#include <webview/webview_ext.h>

#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "utils/logger.h"
#include "resource.h"
#include "utils/os.h"
#include "utils/shell.h"

//webview_eval(w, "functionToCall('param');");

static void get_title(const char* id, const char* req, void* arg) {
    (void)req;

    app_t* app = (app_t*)arg;
    app_config_t* config = app->config_ptr;

    char buf[32];
    snprintf(buf, sizeof(buf), "\"%s\"", config->title);
    webview_return((webview_t)app->webv_inst, id, 0, buf);
}

static void get_content(const char* id, const char* req, void* arg) {
    (void)req;

    char buf[512] = "[";
    int first = 1;
    for (int i = 1; os_distribution_names[i] != NULL; i++) {
        if (!first)
            strcat(buf, ",");
        first = 0;
        strcat(buf, "\"");
        strcat(buf, os_distribution_names[i]);
        strcat(buf, "\"");
    }
    strcat(buf, "]");
    webview_return((webview_t)arg, id, 0, buf);
}

static void run_action(const char* id, const char* req, void* arg) {
    char selected[128] = "?";
    const char* first = strchr(req, '"');
    if (first != NULL) {
        size_t len = strcspn(first + 1, "\"");
        if (len > sizeof(selected) - 1)
            len = sizeof(selected) - 1;
        memcpy(selected, first + 1, len);
        selected[len] = '\0';
    }

    shell_run_command("echo Hello", true, false);

    //ShellExecute(NULL,
    //    "runas",
    //    "echo",
    //    "HelloWorld",
    //    NULL,                        // default dir 
    //    SW_SHOWNORMAL
    //);

    printf("Run Selected Action: %s\n", selected);
    webview_return((webview_t)arg, id, 0, "true");
}

void app_initialize(app_t* app, const app_config_t* config) {
    app->config_ptr = config;

    os_info_t infos = os_get_info();


    WBFA_LOG_INFO("Creating WebView instance...");

    app->webv_inst = webview_create(0, NULL);
    if (app->webv_inst == NULL)
        WBFA_LOG_ERROR("Could not create the WebView instance!");

    WBFA_LOG_INFO("Binding functions to javascript...");


    webview_bind_minimize(app->webv_inst, "appMinimize");
    webview_bind_close(app->webv_inst, "appClose");
    webview_bind_drag(app->webv_inst, "appStartDrag");
    webview_bind(app->webv_inst, "getWindowTitle", get_title, app);
    webview_bind(app->webv_inst, "getDropdownContent", get_content, app->webv_inst);
    webview_bind(app->webv_inst, "runAction", run_action, app->webv_inst);

    WBFA_LOG_INFO("Initializing the window surface...");

    webview_window_options_t options;
    webview_window_options_init(&options);
    options.borderless = 1;
    options.rounded_corners = 1;
    options.icon_resource_id = IDI_APP_ICON;

    if (!webview_init_window(app->webv_inst, config->title, config->width, config->height, config->hint, &options))
        WBFA_LOG_ERROR("Could not initialize the window!");

    char* html = webview_load_resource_text(IDR_BASIC_APP_HTML, RT_HTML);
    if (html == NULL) {
        WBFA_LOG_ERROR("Could not load HTML resource!");
    }

    webview_set_html(app->webv_inst, html);
    webview_free_resource_text(html);
}

int app_run(app_t* app) {
    WBFA_LOG_INFO("Running WebView...");
    webview_error_t err = webview_run(app->webv_inst);
    if (err != WEBVIEW_ERROR_OK) {
        WBFA_LOG_ERROR("Could not run WebView!");
        return 1;
    }

    WBFA_LOG_INFO("Shutting down WebView...");
    err = webview_destroy(app->webv_inst);
    if (err != WEBVIEW_ERROR_OK) {
        WBFA_LOG_ERROR("Could not shutdown WebView!");
        return 1;
    }

    return 0;
}