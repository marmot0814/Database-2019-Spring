#ifndef DB_UTIL_H
#define DB_UTIL_H
#include "Command.h"
#include "Table.h"

typedef struct State {
    int saved_stdout;
} State_t;

State_t* new_State();
void print_prompt(State_t *state);
void print_user(User_t *user, SelectArgs_t *sel_args);
void print_result(Table_t *user_table, Table_t *like_table, Command_t *cmd);
int parse_input(char *input, Command_t *cmd);
void handle_builtin_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd, State_t *state);
void print_help_msg();
void append_string(char **string, char *targetObj);
int check_where_condition(User_t *user, Like_t *like, WhereArgs_t *where_args);

#endif
