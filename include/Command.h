#ifndef COMMAND_H
#define COMMAND_H

/* enum begin */
enum { 
    UNRECOG_CMD,
    BUILT_IN_CMD,
    QUERY_CMD,
};

enum {
    INSERT_CMD = 100,
    SELECT_CMD,
    UPDATE_CMD,
    DELETE_CMD
};

enum {
    AND,
    OR
};

enum {
    ID,
    NAME,
    EMAIL,
    AGE,
    ID1,
    ID2
};

enum {
    SUM,
    COUNT,
    AVG
};

enum {
    EQUAL_TO,
    NOT_EQUAL_TO,
    GREATER,
    LESS,
    GREATER_OR_EQUAL_TO,
    LESS_OR_EQUAL_TO,
    STRING_EQUAL_TO,
    STRING_NOT_EQUAL_TO,
};

/* enum end */

/* cmd begin */
typedef struct {
    char name[256];
    int len;
    unsigned char type;
} CMD_t;

extern CMD_t cmd_list[];
/* cmd end */

typedef struct InsertArgs {
    int table;
} InsertArgs_t;

typedef struct SelectArgs {
    char **fields;
    size_t fields_len;
    int table1;
    int table2;
    int field_in_table1;
    int field_in_table2;
    int offset;
    int limit;
} SelectArgs_t;

typedef struct UpdateArgs {
    char *set_condition;
} UpdateArgs_t;

typedef struct DeleteArgs {
    int table;
} DeleteArgs_t;

typedef struct WhereArgs {
    char *where_condition1, *where_condition2;
    int where_logic_op;
} WhereArgs_t;

typedef union {
    InsertArgs_t ins_args;
    SelectArgs_t sel_args;
    UpdateArgs_t upd_args;
    DeleteArgs_t del_args;
} CmdArg_t;

typedef struct Command {
    unsigned char type;
    char **args;
    size_t args_len;
    size_t args_cap;
    CmdArg_t cmd_args;
    WhereArgs_t where_args;
} Command_t;

Command_t* new_Command();
int add_Arg(Command_t *cmd, const char *arg);
int add_select_field(Command_t *cmd, const char *argument);
void cleanup_Command(Command_t *cmd);

#endif
