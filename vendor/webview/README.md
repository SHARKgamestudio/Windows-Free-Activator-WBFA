# webview - prebuilt static libraries

Prebuilt static libraries for the [webview](https://github.com/webview/webview)
library (`webview_core`) and the window-utility layer (`webview_ext`), plus the
headers needed to use them.

At runtime the app only needs the WebView2 Evergreen runtime (ships with
Windows 11, auto-installed on Windows 10).

## Usage (MSVC)

1. Add `include/webview/` to *AdditionalIncludeDirectories*.
2. Add `lib/` to *AdditionalLibraryDirectories*.
3. `#include "webview_core.h"` and `#include "webview_ext.h"`.
   (`WEBVIEW_STATIC` is defined automatically by the packaged
   `webview_core.h`; you do not need to define it yourself.)
4. Link (webview_ext BEFORE webview_core - ext references core symbols):

       webview_ext.lib;webview_core.lib;advapi32.lib;ole32.lib;shell32.lib;shlwapi.lib;user32.lib;version.lib

   For Debug builds use the `-d` variants:

       webview_ext-d.lib;webview_core-d.lib;advapi32.lib;ole32.lib;shell32.lib;shlwapi.lib;user32.lib;version.lib

5. Match the CRT: this package was built with the static CRT
   (`/MT` for Release, `/MTd` for Debug). Use the same runtime library
   setting in your project, or rebuild the libs with your CRT.

## License

MIT - see `LICENSE` (webview library).