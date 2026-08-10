#pragma once

#include <stdbool.h>
#include <windows.h>

typedef void(*shell_cmd_cb)(DWORD exit_code, shell_t* src_shell, void* usr_data);

typedef struct shell_cmd {
    const char* content;
    const char* marker;
    shell_cmd_cb callback;
} shell_cmd_t;

typedef struct shell {
    bool has_admin;
    bool show_console;

    shell_cmd_t cmds[512];
    size_t cmd_count;
} shell_t;


shell_t shell_create(bool has_admin, bool show_console);

void shell_add_command(shell_t* shell, const char* cmd, shell_cmd_cb callback);

void shell_run_commands(shell_t* shell);