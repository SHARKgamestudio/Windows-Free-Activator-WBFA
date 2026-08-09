#ifndef WEBVIEW_EXT_H
#define WEBVIEW_EXT_H

#include "webview_core.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Window creation options.
 *
 * These flags and identifiers control the appearance and behaviour of the
 * native window created by @c webview_init_window(). Members that are not
 * explicitly set receive sensible defaults via @c webview_window_options_init().
 *
 * @see webview_window_options_init
 */
typedef struct {
  /**
   * Whether to remove the standard window caption (title bar) and borders.
   * Set to non-zero to create a borderless/popup-style window. Default 1.
   */
  int borderless;

  /**
   * Whether to apply rounded corners to the window.
   * When enabled, DWM (Desktop Window Manager) rounded corners are used on
   * Windows 11. The feature is silently ignored on older Windows versions.
   * Default 1.
   */
  int rounded_corners;

  /**
   * Resource identifier of the icon to set on the window.
   * The icon is loaded via @c LoadImage with 32&times;32 and 16&times;16 sizes.
   * Pass 0 to skip setting an icon. Default is @c IDI_APP_ICON.
   */
  int icon_resource_id;

  /**
   * Whether to centre the window on the screen after creation.
   * The window is centred within the monitor that contains most of the
   * window area at the time of the call. Default 1.
   */
  int center_on_screen;
} webview_window_options_t;

/**
 * Initialises a @c webview_window_options_t structure with default values.
 *
 * Call this once before setting any custom fields. After calling this
 * function only the fields you wish to override need to be assigned.
 *
 * @param options Pointer to the options structure to initialise.
 *        Must not be @c NULL.
 *
 * @see webview_init_window
 */
void webview_window_options_init(webview_window_options_t *options);

/**
 * Attaches a console window to the application and redirects the standard
 * streams to it.
 *
 * This is a debug-only convenience for logging from C (e.g. via @c printf).
 * In debug builds (@c _DEBUG) a console is allocated with @c AllocConsole
 * and @c stdout/@c stderr/@c stdin are redirected to it. In release builds
 * this function is a no-op, so no console ever appears in distributed apps.
 *
 * @see webview_init_window
 */
void webview_attach_console(void);

/**
 * Creates and configures the native window for a webview instance.
 *
 * This function combines several common window-setup steps into one call:
 * setting the title, sizing the window, applying window options (borderless,
 * rounded corners, icon, centering), and preparing the webview for use.
 * In debug builds it also attaches a console for logging
 * (see @c webview_attach_console); this is skipped in release builds.
 *
 * @param w The webview instance. Must not be @c NULL.
 * @param title The window title displayed in the caption (if visible)
 *        or used internally by the window manager. Pass @c NULL to keep
 *        the current title.
 * @param width The initial width of the window in pixels.
 *        Pass 0 or a negative value to keep the default size.
 * @param height The initial height of the window in pixels.
 *        Pass 0 or a negative value to keep the default size.
 * @param hint Sizing hint (@c WEBVIEW_HINT_NONE, @c WEBVIEW_HINT_MIN,
 *        @c WEBVIEW_HINT_MAX or @c WEBVIEW_HINT_FIXED).
 * @param options Pointer to a @c webview_window_options_t structure, or
 *        @c NULL to use defaults for all options.
 *
 * @return Non-zero on success, zero on failure.
 *
 * @see webview_window_options_init
 * @see webview_window_options_t
 */
int webview_init_window(webview_t w,
                        const char *title,
                        int width,
                        int height,
                        int hint,
                        const webview_window_options_t *options);

/**
 * Binds a native function to a JavaScript function that minimises the window.
 *
 * When the JS function named @p js_name is called from the webview, the
 * native window is minimised via @c SC_MINIMIZE.
 *
 * @param w The webview instance. Must not be @c NULL.
 * @param js_name The name of the JS function to create (e.g. "appMinimize").
 *
 * @see webview_bind_close
 * @see webview_bind_drag
 */
void webview_bind_minimize(webview_t w, const char *js_name);

/**
 * Binds a native function to a JavaScript function that closes the window.
 *
 * When the JS function named @p js_name is called from the webview, the
 * webview event loop is terminated via @c webview_terminate().
 *
 * @param w The webview instance. Must not be @c NULL.
 * @param js_name The name of the JS function to create (e.g. "appClose").
 *
 * @see webview_bind_minimize
 * @see webview_bind_drag
 */
void webview_bind_close(webview_t w, const char *js_name);

/**
 * Binds a native function to a JavaScript function that starts a window drag.
 *
 * When the JS function named @p js_name is called from the webview, the
 * window is put into a drag-to-move operation by posting a
 * @c WM_NCLBUTTONDOWN message with @c HTCAPTION. This is useful for
 * implementing custom title bars in borderless windows.
 *
 * @param w The webview instance. Must not be @c NULL.
 * @param js_name The name of the JS function to create (e.g. "appStartDrag").
 *
 * @see webview_bind_minimize
 * @see webview_bind_close
 */
void webview_bind_drag(webview_t w, const char *js_name);

/**
 * Binds an arbitrary native callback to a JavaScript function.
 *
 * When the JS function named @p js_name is called from the webview, the
 * provided @p action callback is invoked. The callback receives the webview
 * instance as its sole argument. This is a low-level building block used
 * by the other @c webview_bind_* helpers.
 *
 * Internally the binding is created via @c webview_bind() and a bridge
 * callback that unpacks the user action. Only one action can be dispatched
 * at a time because the bridge uses an internal module-global handle.
 *
 * @param w The webview instance. Must not be @c NULL.
 * @param js_name The name of the JS function to create.
 * @param action The callback to invoke when the JS function is called.
 *        Must not be @c NULL.
 *
 * @see webview_bind_minimize
 * @see webview_bind_close
 * @see webview_bind_drag
 */
void webview_bind_action(webview_t w, const char *js_name,
                         void (*action)(webview_t));

/**
 * Convenience function that binds minimise, close and drag in one call.
 *
 * Equivalent to calling each of @c webview_bind_minimize(),
 * @c webview_bind_close() and @c webview_bind_drag() with the JS function
 * names "appMinimize", "appClose" and "appStartDrag" respectively.
 *
 * @param w The webview instance. Must not be @c NULL.
 *
 * @see webview_bind_minimize
 * @see webview_bind_close
 * @see webview_bind_drag
 */
void webview_bind_window_actions(webview_t w);

/**
 * Loads the content of a Windows resource as a null-terminated string.
 *
 * The resource is looked up in the module of the calling executable via
 * @c FindResourceA / @c LoadResource. The returned buffer is a copy of the
 * resource data with an additional null terminator appended, and must be
 * freed with @c webview_free_resource_text() when no longer needed.
 *
 * @param resource_id The integer identifier of the resource (e.g.
 *        @c IDR_BASIC_APP_HTML).
 * @param resource_type The resource type string (e.g. @c RT_RCDATA).
 *
 * @return A newly allocated null-terminated string on success, or
 *         @c NULL if the resource could not be found or loaded.
 *
 * @see webview_free_resource_text
 */
char *webview_load_resource_text(int resource_id, const char *resource_type);

/**
 * Frees a string previously returned by @c webview_load_resource_text().
 *
 * @param text The string to free. May be @c NULL (the call is a no-op).
 *
 * @see webview_load_resource_text
 */
void webview_free_resource_text(char *text);

#ifdef __cplusplus
}
#endif

#endif
