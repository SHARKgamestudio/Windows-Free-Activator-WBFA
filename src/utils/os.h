#pragma once

/**********************************************************************************************
*
*   OS 0.1 - A high-level abstraction layer for interacting with the Windows OS, developed for WBFA.
*
*   FEATURES:
*       - Get system version information
*       - Get system language information
*       - Get system theme information
*
*   NOTES:
*       - We do not recommend using this library in your projects, as the API is subject to frequent breaking changes.
*
*   DEPENDENCIES (system only):
*       - Windows headers: <windows.h>
*
*   LICENSE: SHARK_LICENSE_ONR
*       - OS is licensed under the same terms as the software it is part of: WBFA
*
**********************************************************************************************/


/*
 * Define DWORD for better readability of the 'OSBuild' type.
 * On Windows, DWORD is typically defined as "unsigned long".
 * It is redefined here to avoid including <Windows.h> in this header.
 */
typedef unsigned long DWORD;


//----------------------------------------------------------------------------------
// Version
//----------------------------------------------------------------------------------

/*
 * Enum for major Windows versions supported by WBFA.
 */
typedef enum os_version {
    OS_WIN_UNKNOWN,
    OS_WIN_XP,
    OS_WIN_VISTA,
    OS_WIN_7,
    OS_WIN_8,
    OS_WIN_8_1,
    OS_WIN_10,
    OS_WIN_10_LTSB_2015,
    OS_WIN_10_LTSB_2016,
    OS_WIN_10_LTSC_2019,
    OS_WIN_10_LTSC_2021,
    OS_WIN_11
} os_version_t;

static const char* const os_version_names[] = {
    [OS_WIN_UNKNOWN]        = "Unknown Version",
    [OS_WIN_XP]             = "Windows XP",
    [OS_WIN_VISTA]          = "Windows Vista",
    [OS_WIN_7]              = "Windows 7",
    [OS_WIN_8]              = "Windows 8",
    [OS_WIN_8_1]            = "Windows 8.1",
    [OS_WIN_10]             = "Windows 10",
    [OS_WIN_10_LTSB_2015]   = "Windows 10 - LTSB 2015",
    [OS_WIN_10_LTSB_2016]   = "Windows 10 - LTSB 2016",
    [OS_WIN_10_LTSC_2019]   = "Windows 10 - LTSC 2019",
    [OS_WIN_10_LTSC_2021]   = "Windows 10 - LTSC 2021",
    [OS_WIN_11]             = "Windows 11"
};

/*
 * Define OSBuild as an alias for DWORD.
 * Provides abstraction and improves readability.
 */
typedef DWORD os_build_t;

/*
 * Enum for Windows distributions/editions supported by WBFA.
 */
typedef enum os_distribution {
    OS_DIST_UNKNOWN,
    OS_DIST_HOME,
    OS_DIST_HOME_N,
    OS_DIST_HOME_SINGLE_LANG,
    OS_DIST_HOME_COUNTRY_SPECIFIC,
    OS_DIST_PRO,
    OS_DIST_PRO_N,
    OS_DIST_EDUCATION,
    OS_DIST_EDUCATION_N,
    OS_DIST_BUSINESS,
    OS_DIST_BUSINESS_N,
    OS_DIST_BUSINESS_LTSB,
    OS_DIST_BUSINESS_LTSB_N
} os_distribution_t;

static const char* const os_distribution_names[] = {
    [OS_DIST_UNKNOWN]                   = "Unknown Distribution",
    [OS_DIST_HOME]                      = "Windows Home",
    [OS_DIST_HOME_N]                    = "Windows Home N",
    [OS_DIST_HOME_SINGLE_LANG]          = "Windows Home, Single Language",
    [OS_DIST_HOME_COUNTRY_SPECIFIC]     = "Windows Home, Country Specific",
    [OS_DIST_PRO]                       = "Windows Professional",
    [OS_DIST_PRO_N]                     = "Windows Professional N",
    [OS_DIST_EDUCATION]                 = "Windows Education",
    [OS_DIST_EDUCATION_N]               = "Windows Education N",
    [OS_DIST_BUSINESS]                  = "Windows Business",
    [OS_DIST_BUSINESS_N]                = "Windows Business N",
    [OS_DIST_BUSINESS_LTSB]             = "Windows Business - LTSB",
    [OS_DIST_BUSINESS_LTSB_N]           = "Windows Business N - LTSB"
};


//----------------------------------------------------------------------------------
// Language
//----------------------------------------------------------------------------------

/*
 * Enum for system languages supported by WBFA.
 */
typedef enum os_language {
    OS_LANG_UNKNOWN,
    OS_LANG_ENGLISH,
    OS_LANG_FRENCH,
    OS_LANG_GERMAN,
    OS_LANG_SPANISH,
    OS_LANG_JAPANESE,
    OS_LANG_CHINESE_SIMPLIFIED,
    OS_LANG_CHINESE_TRADITIONAL,
} os_language_t;
static const char* const os_language_names[] = {
    [OS_LANG_UNKNOWN]               = "Unknown Language",
    [OS_LANG_ENGLISH]               = "English",
    [OS_LANG_FRENCH]                = "French",
    [OS_LANG_GERMAN]                = "German",
    [OS_LANG_SPANISH]               = "Spanish",
    [OS_LANG_JAPANESE]              = "Japanese",
    [OS_LANG_CHINESE_SIMPLIFIED]    = "Simplified Chinese",
    [OS_LANG_CHINESE_TRADITIONAL]   = "Traditional Chinese"
};


//----------------------------------------------------------------------------------
// Theme
//----------------------------------------------------------------------------------

typedef enum os_theme {
    OS_THEME_UNKNOWN,
    OS_THEME_LIGHT,
    OS_THEME_DARK
} os_theme_t;

static const char* const os_theme_names[] = {
    [OS_THEME_UNKNOWN] = "Unknown Theme",
    [OS_THEME_LIGHT] = "Dark",
    [OS_THEME_DARK] = "Light",
};


//----------------------------------------------------------------------------------
// Get OS Info
//----------------------------------------------------------------------------------

/*
 * Structure representing all necessary OS information required for WBFA to function properly.
 * Includes version, build number, and distribution.
 * As well as main system language, and theme.
 */
typedef struct os_info {
    os_version_t version;
    os_build_t build;
    os_distribution_t distribution;

    os_language_t language;
	os_theme_t theme;
} os_info_t;

os_info_t os_get_info();