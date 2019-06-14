#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include "Util.h"
#include "Command.h"
#include "Table.h"
#include "QueryState.h"

///
/// Allocate State_t and initialize some attributes
/// Return: ptr of new State_t
///
State_t* new_State() {
    State_t *state = (State_t*)malloc(sizeof(State_t));
    state->saved_stdout = -1;
    return state;
}

///
/// Print shell prompt
///
void print_prompt(State_t *state) {
    if (state->saved_stdout == -1) {
        printf("db > ");
    }
}

///
/// Print the user in the specific format
///
void print_user(User_t *user, SelectArgs_t *sel_args) {
    size_t idx;
    printf("(");
    for (idx = 0; idx < sel_args->fields_len; idx++) {
        if (!strncmp(sel_args->fields[idx], "*", 1)) {
            printf("%d, %s, %s, %d", user->id, user->name, user->email, user->age);
        } else {
            if (idx > 0) printf(", ");

            if (!strncmp(sel_args->fields[idx], "id", 2)) {
                printf("%d", user->id);
            } else if (!strncmp(sel_args->fields[idx], "name", 4)) {
                printf("%s", user->name);
            } else if (!strncmp(sel_args->fields[idx], "email", 5)) {
                printf("%s", user->email);
            } else if (!strncmp(sel_args->fields[idx], "age", 3)) {
                printf("%d", user->age);
            }
        }
    }
    printf(")\n");
}

/* operation begin */
int equal_to            (void *patterm, void *val) { return *((int*)patterm) == atoi((char*)val); }
int not_equal_to        (void *patterm, void *val) { return *((int*)patterm) != atoi((char*)val); }
int greater             (void *patterm, void *val) { return *((int*)patterm) >  atoi((char*)val); }
int less                (void *patterm, void *val) { return *((int*)patterm) <  atoi((char*)val); }
int greater_or_equal_to (void *patterm, void *val) { return *((int*)patterm) >= atoi((char*)val); }
int less_or_equal_to    (void *patterm, void *val) { return *((int*)patterm) <= atoi((char*)val); }
int string_equal_to     (void *patterm, void *val) { return strcmp((char*)patterm, (char*)val) == 0; }
int string_not_equal_to (void *patterm, void *val) { return strcmp((char*)patterm, (char*)val) != 0; }
/* operation  end  */

///
/// eval string condition
///
int eval(User_t *user, char *condition) {
    if (strlen(condition) == 0)
        return 1;
    void* patterm[] = {
        (void*)&user->id, 
        (void*)&user->name, 
        (void*)&user->email, 
        (void*)&user->age
    };
    size_t patterm_idx;

    int (*op[])(void* patterm, void* val) = {
        equal_to, 
        not_equal_to, 
        greater, 
        less, 
        greater_or_equal_to, 
        less_or_equal_to, 
        string_equal_to, 
        string_not_equal_to
    };
    size_t op_idx = 0;

    char *ptr = condition;
         if (!strncmp(ptr, "id"   , 2)) ptr += 2, patterm_idx = ID;
    else if (!strncmp(ptr, "name" , 4)) ptr += 4, patterm_idx = NAME;
    else if (!strncmp(ptr, "email", 5)) ptr += 5, patterm_idx = EMAIL;
    else if (!strncmp(ptr, "age"  , 3)) ptr += 3, patterm_idx = AGE;
    else {};

         if (!strncmp(ptr, "=" , 1)) ptr += 1, op_idx = ( patterm_idx == ID || patterm_idx == AGE ? EQUAL_TO : STRING_EQUAL_TO );
    else if (!strncmp(ptr, "!=", 2)) ptr += 2, op_idx = ( patterm_idx == ID || patterm_idx == AGE ? NOT_EQUAL_TO : STRING_NOT_EQUAL_TO );
    else if (!strncmp(ptr, ">=", 2)) ptr += 2, op_idx = GREATER_OR_EQUAL_TO;
    else if (!strncmp(ptr, "<=", 2)) ptr += 2, op_idx = LESS_OR_EQUAL_TO;
    else if (!strncmp(ptr, ">" , 1)) ptr += 1, op_idx = GREATER;
    else if (!strncmp(ptr, "<" , 1)) ptr += 1, op_idx = LESS;
    else {};

    return op[op_idx](patterm[patterm_idx], (void*)ptr);
}

/* logic operation begin */
int and(int a, int b) { return a && b; }
int or (int a, int b) { return a || b; }
/* logic operation  end  */

///
/// check where condition
///
int check_where_condition(User_t *user, WhereArgs_t *where_args) {
    int (*op[])(int a, int b) = {and, or};
    return op[where_args->where_logic_op](eval(user, where_args->where_condition1), eval(user, where_args->where_condition2));
}


void print_result(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    for (size_t idx = 0; idx < user_table->len; idx++) {
        if (cmd->cmd_args.sel_args.offset) {
            cmd->cmd_args.sel_args.offset--;
            continue;
        }
        if (!cmd->cmd_args.sel_args.limit)
            break;
        User_t *user = get_User(user_table, idx);
        if (check_where_condition(user, &cmd->where_args))
            print_user(user, &cmd->cmd_args.sel_args), cmd->cmd_args.sel_args.limit--;
    }
}
///
/// This function received an output argument
/// Return: category of the command
///
int parse_input(char *input, Command_t *cmd) {
    char *token;
    int idx;
    token = strtok(input, " ,\n");
    if (!token)
        return -1;
    for (idx = 0; strlen(cmd_list[idx].name) != 0; idx++) {
        if (!strncmp(token, cmd_list[idx].name, cmd_list[idx].len)) {
            cmd->type = cmd_list[idx].type;
        }
    }
    while (token != NULL) {
        add_Arg(cmd, token);
        token = strtok(NULL, " ,\n");
    }
    return cmd->type;
}

///
/// Handle built-in commands
/// Return: command type
///
void handle_builtin_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd, State_t *state) {
    if (!strncmp(cmd->args[0], ".exit", 5)) {
        archive_table(user_table);
        exit(0);
    } else if (!strncmp(cmd->args[0], ".output", 7)) {
        if (cmd->args_len == 2) {
            if (!strncmp(cmd->args[1], "stdout", 6)) {
                close(1);
                dup2(state->saved_stdout, 1);
                state->saved_stdout = -1;
            } else if (state->saved_stdout == -1) {
                int fd = creat(cmd->args[1], 0644);
                state->saved_stdout = dup(1);
                if (dup2(fd, 1) == -1) {
                    state->saved_stdout = -1;
                }
                __fpurge(stdout); //This is used to clear the stdout buffer
            }
        }
    } else if (!strncmp(cmd->args[0], ".load", 5)) {
        if (cmd->args_len == 2) {
            load_table(user_table, cmd->args[1]);
        }
    } else if (!strncmp(cmd->args[0], ".help", 5)) {
        print_help_msg();
    }
}

void append_string(char **oriString, char *targetObj) {
    size_t oriString_len = strlen(*oriString);
    size_t targetObj_len = strlen( targetObj);
    char *buf = (char*)malloc(sizeof(char) * (oriString_len + targetObj_len + 1));
    memcpy(buf, *oriString, sizeof(char) * oriString_len);
    free(*oriString);
    memcpy(buf + oriString_len, targetObj, sizeof(char) * (targetObj_len + 1));
    *oriString = buf;
}


///
/// Show the help messages
///
void print_help_msg() {
    const char msg[] = "# Supported Commands\n"
    "\n"
    "## Built-in Commands\n"
    "\n"
    "  * .exit\n"
    "\tThis cmd archives the table, if the db file is specified, then exit.\n"
    "\n"
    "  * .output\n"
    "\tThis cmd change the output strategy, default is stdout.\n"
    "\n"
    "\tUsage:\n"
    "\t    .output (<file>|stdout)\n\n"
    "\tThe results will be redirected to <file> if specified, otherwise they will display to stdout.\n"
    "\n"
    "  * .load\n"
    "\tThis command loads records stored in <DB file>.\n"
    "\n"
    "\t*** Warning: This command will overwrite the records already stored in current table. ***\n"
    "\n"
    "\tUsage:\n"
    "\t    .load <DB file>\n\n"
    "\n"
    "  * .help\n"
    "\tThis cmd displays the help messages.\n"
    "\n"
    "## Query Commands\n"
    "\n"
    "  * insert\n"
    "\tThis cmd inserts one user record into table.\n"
    "\n"
    "\tUsage:\n"
    "\t    insert <id> <name> <email> <age>\n"
    "\n"
    "\t** Notice: The <name> & <email> are string without any whitespace character, and maximum length of them is 255. **\n"
    "\n"
    "  * select\n"
    "\tThis cmd will display all user records in the table.\n"
    "\n";
    printf("%s", msg);
}

