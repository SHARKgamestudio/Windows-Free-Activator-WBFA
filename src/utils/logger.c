#include "logger.h"

#include <stdio.h>

const char* wbfa_get_severity_color(wbfa_severity_t severity) {
	switch (severity) {
	case WBFA_INFO:
		return "\033[32m";
	case WBFA_WARNING:
		return "\033[33m";
	case WBFA_ERROR:
		return "\033[31m";
	default:
		return "\033[0m";
	}
}
const char* wbfa_get_severity_string(wbfa_severity_t severity) {
	switch (severity) {
	case WBFA_INFO:
		return "INFO";
		break;
	case WBFA_WARNING:
		return "WARNING";
		break;
	case WBFA_ERROR:
		return "ERROR";
		break;
	default:
		return "UNKNOWN";
		break;
	}
}

void wbfa_log(const char* message, wbfa_severity_t severity, const char* function, const char* file, int line) {
	const char* color = wbfa_get_severity_color(severity);
	const char* str = wbfa_get_severity_string(severity);
	// std::cout << color << "[OpenGL] " << str << " : " << message << " in " << function << " [" << file << " : line " << line << "] \033[0m" << std::endl;

	printf("%s[%s] %s : %s\n>   In %s [%s : line %i]\033[0m\n", color, APP_NAME, str, message, function, file, line);
}