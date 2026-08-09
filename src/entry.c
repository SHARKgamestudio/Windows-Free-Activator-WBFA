/*
 * Sample app: webview library + webview_ext helpers.
 *
 * Data flows "pull": JavaScript calls a bound function (e.g.
 * window.getWindowTitle()) and the C callback replies through
 * webview_return(), resolving the JS Promise with a JSON string.
 * Nothing is pushed from C to JS.
 */

#include "webview/webview_core.h"
#include "webview/webview_ext.h"
#include <stdio.h>
#include <string.h>

#include <windows.h>
#include "resource.h"

 /* Window title as a JSON string, set from WinMain before the window exists. */
static const char* g_title = NULL;

static void get_title(const char* id, const char* req, void* arg) {
	(void)req; /* payload of the (parameterless) JS call, unused here */

	char buf[512];
	/* webview_return expects a JSON value, hence the surrounding quotes. */
	snprintf(buf, sizeof(buf), "\"%s\"", g_title ? g_title : "");
	webview_return((webview_t)arg, id, 0, buf);
}

/* Environment list for the dropdown; NULL-terminated. */
static const char* g_content[] =
{ "Option 1", "Option 2", "Option 3", NULL };

static void get_content(const char* id, const char* req, void* arg) {
	(void)req;

	/* Assemble a JSON array by hand: ["Option 1","Option 2","Option 3"]. */
	char buf[512] = "[";
	int first = 1;
	for (int i = 0; g_content[i] != NULL; i++) {
		if (!first)
			strcat(buf, ",");
		first = 0;
		strcat(buf, "\"");
		strcat(buf, g_content[i]);
		strcat(buf, "\"");
	}
	strcat(buf, "]");
	webview_return((webview_t)arg, id, 0, buf);
}

/* Handles the "Run Selected Action" button. JS passes the selected value as
 * an argument; the bridge delivers it as the JSON array of params, e.g.
 * ["Development"]. We pull the string out and log it to the console. */
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

	printf("Run Selected Action: %s\n", selected);
	webview_return((webview_t)arg, id, 0, "true");
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	(void)hInst;
	(void)hPrevInst;
	(void)lpCmdLine;
	(void)nCmdShow;

	const char* app_title = "Basic Example";
	webview_t w = webview_create(0, NULL);
	if (w == NULL)
		return 1;

	g_title = app_title;

	/* Make functions callable from JS. The webview_bind_* helpers expose
	 * native window actions; webview_bind wires arbitrary callbacks. */
	webview_bind_minimize(w, "appMinimize");
	webview_bind_close(w, "appClose");
	webview_bind_drag(w, "appStartDrag");
	webview_bind(w, "getWindowTitle", get_title, w);
	webview_bind(w, "getDropdownContent", get_content, w);
	webview_bind(w, "runAction", run_action, w);

	/* Defaults are sane; override only what differs. */
	webview_window_options_t options;
	webview_window_options_init(&options);
	options.borderless = 1;
	options.rounded_corners = 1;
	options.icon_resource_id = IDI_APP_ICON;

	if (!webview_init_window(w, app_title, 960, 540, WEBVIEW_HINT_FIXED, &options))
		return 1;

	/* the ui is compiled into the exe as an rcdata resource, so the app is a
	 * single self-contained binary. webview_load_resource_text returns a copy
	 * we own, hence the matching char* (webview_set_html only reads it). */
	char* html = webview_load_resource_text(IDR_BASIC_APP_HTML, RT_HTML);
	if (html != NULL) {
		webview_set_html(w, html);
		webview_free_resource_text(html);
	}

	webview_run(w); /* blocks until the window is closed */
	webview_destroy(w);
	return 0;
}