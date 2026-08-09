#include "os.h"

#include <windows.h>

typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

os_info_t os_get_info() {
    os_info_t info = { OS_WIN_UNKNOWN, 0, OS_DIST_UNKNOWN, OS_LANG_UNKNOWN, OS_THEME_UNKNOWN };

	// Get version and build number //
    HMODULE hMod = GetModuleHandleW(L"ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fRtlGetVersion = (RtlGetVersionPtr)GetProcAddress(hMod, "RtlGetVersion");
        if (fRtlGetVersion) {
            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);
            if (fRtlGetVersion(&rovi) == 0) {
                DWORD major = rovi.dwMajorVersion;
                DWORD minor = rovi.dwMinorVersion;
                info.build = rovi.dwBuildNumber;

                if (major == 5 && minor == 1) info.version = OS_WIN_XP;
                else if (major == 6 && minor == 0) info.version = OS_WIN_VISTA;
                else if (major == 6 && minor == 1) info.version = OS_WIN_7;
                else if (major == 6 && minor == 2) info.version = OS_WIN_8;
                else if (major == 6 && minor == 3) info.version = OS_WIN_8_1;
                else if (major == 10) {
                    if (info.build < 22000) {
                        // Windows 10 family
                        if (info.build == 10240) info.version = OS_WIN_10_LTSB_2015;
                        else if (info.build == 14393) info.version = OS_WIN_10_LTSB_2016;
                        else if (info.build == 17763) info.version = OS_WIN_10_LTSC_2019;
                        else if (info.build == 19044) info.version = OS_WIN_10_LTSC_2021;
                        else info.version = OS_WIN_10;
                    }
                    else {
                        info.version = OS_WIN_11;
                    }
                }
            }
        }
    }

	// Get distribution //
    DWORD prodType = 0;
    if (GetProductInfo(10, 0, 0, 0, &prodType)) {
        switch (prodType) {
        case PRODUCT_CORE: info.distribution = OS_DIST_HOME; break;
        case PRODUCT_CORE_N: info.distribution = OS_DIST_HOME_N; break;
        case PRODUCT_CORE_SINGLELANGUAGE: info.distribution = OS_DIST_HOME_SINGLE_LANG; break;
        case PRODUCT_CORE_COUNTRYSPECIFIC: info.distribution = OS_DIST_HOME_COUNTRY_SPECIFIC; break;
        case PRODUCT_PROFESSIONAL: info.distribution = OS_DIST_PRO; break;
        case PRODUCT_PROFESSIONAL_N: info.distribution = OS_DIST_PRO_N; break;
        case PRODUCT_EDUCATION: info.distribution = OS_DIST_EDUCATION; break;
        case PRODUCT_EDUCATION_N: info.distribution = OS_DIST_EDUCATION_N; break;
        case PRODUCT_ENTERPRISE: info.distribution = OS_DIST_BUSINESS; break;
        case PRODUCT_ENTERPRISE_N: info.distribution = OS_DIST_BUSINESS_N; break;
        case PRODUCT_ENTERPRISE_S: // Enterprise LTSB
            info.distribution = OS_DIST_BUSINESS_LTSB; break;
        case PRODUCT_ENTERPRISE_S_N:
            info.distribution = OS_DIST_BUSINESS_LTSB_N; break;
        default:
            info.distribution = OS_DIST_UNKNOWN; break;
        }
    }
    
	// Get system language //
    LANGID langId = GetSystemDefaultLangID();  // System-wide language
    WORD primaryLang = PRIMARYLANGID(langId);
    switch (primaryLang) {
	case LANG_ENGLISH:   info.language = OS_LANG_ENGLISH; break;
	case LANG_FRENCH:    info.language = OS_LANG_FRENCH; break;
	case LANG_GERMAN:    info.language = OS_LANG_GERMAN; break;
	case LANG_SPANISH:   info.language = OS_LANG_SPANISH; break;
	case LANG_JAPANESE:  info.language = OS_LANG_JAPANESE; break;
    case LANG_CHINESE:
        // Distinguish between Simplified vs Traditional
        switch (SUBLANGID(langId)) {
		case SUBLANG_CHINESE_SIMPLIFIED: info.language = OS_LANG_CHINESE_SIMPLIFIED; break;
		case SUBLANG_CHINESE_TRADITIONAL: info.language = OS_LANG_CHINESE_TRADITIONAL; break;
        default:
            info.language = OS_LANG_CHINESE_SIMPLIFIED;
			break;
        }
    default:
        info.language = OS_LANG_UNKNOWN;
		break;
    }

    // Get theme //
    DWORD value = 0;
    DWORD dataSize = sizeof(value);
    HKEY hKey;
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0,
        KEY_READ,
        &hKey
    );
    if (result != ERROR_SUCCESS) {
        info.theme = OS_THEME_UNKNOWN;
    }
    result = RegGetValueW(
        hKey,
        NULL,
        L"AppsUseLightTheme",
        RRF_RT_REG_DWORD,
        NULL,
        &value,
        &dataSize
    );
    RegCloseKey(hKey);
    if (result != ERROR_SUCCESS) {
        info.theme = OS_THEME_UNKNOWN;
    }
    info.theme = (value == 0) ? OS_THEME_DARK : OS_THEME_LIGHT;
    
    return info;
}