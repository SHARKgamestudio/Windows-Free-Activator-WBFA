#pragma once

#define APP_NAME "WBFA"

#define WBFA_LOG_INFO(message) wbfa_log(message, WBFA_INFO, __FUNCTION__, __FILE__, __LINE__)
#define WBFA_LOG_WARNING(message) wbfa_log(message, WBFA_WARNING, __FUNCTION__, __FILE__, __LINE__)
#define WBFA_LOG_ERROR(message) wbfa_log(message, WBFA_ERROR, __FUNCTION__, __FILE__, __LINE__)

typedef enum wbfa_severity {
	WBFA_INFO,
	WBFA_WARNING,
	WBFA_ERROR
} wbfa_severity_t;

const char* wbfa_get_severity_color(wbfa_severity_t severity);
const char* wbfa_get_severity_string(wbfa_severity_t severity);

void wbfa_log(const char* message, wbfa_severity_t severity, const char* function, const char* file, int line);