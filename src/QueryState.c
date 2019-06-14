#include <string.h>
#include <stdlib.h>
#include "Command.h"
#include "QueryState.h"
#include "Util.h"
#include "Table.h"

int handle_query_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    if        (!strncmp(cmd->args[0], "select", 6)) {
        handle_select_cmd(user_table, like_table, cmd);
    } else if (!strncmp(cmd->args[0], "insert", 6)) {
        handle_insert_cmd(user_table, like_table, cmd);
    } else if (!strncmp(cmd->args[0], "update", 6)) {

    } else if (!strncmp(cmd->args[0], "delete", 6)) {

    }
    return 1;
}


/* select begin */
int handle_select_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    cmd->type = SELECT_CMD;
    cmd->where_args.where_condition1 = strdup("");
    cmd->where_args.where_condition2 = strdup("");
    select_field_handler(cmd, 1);
    /*
    puts("");
    printf ("args_len: %ld\n", cmd->args_len);
    printf ("field_len: %ld\n", cmd->cmd_args.sel_args.fields_len);
    printf ("table1: %d\n", cmd->cmd_args.sel_args.table1);
    printf ("table2: %d\n", cmd->cmd_args.sel_args.table2);
    printf ("table1_field: %d\n", cmd->cmd_args.sel_args.field_in_table1);
    printf ("table2_field: %d\n", cmd->cmd_args.sel_args.field_in_table2);
    printf ("where_condition1: %s\n", cmd->where_args.where_condition1);
    printf ("where_condition2: %s\n", cmd->where_args.where_condition2);
    printf ("where_op: %d\n", cmd->where_args.where_logic_op);
    printf ("offset: %d\n", cmd->cmd_args.sel_args.offset);
    printf ("limit: %d\n", cmd->cmd_args.sel_args.limit);
    puts("");
    */
    print_result(user_table, like_table, cmd);
    return 1;
}

void select_field_handler(Command_t *cmd, size_t arg_idx) {
    cmd->cmd_args.sel_args.fields = NULL;
    cmd->cmd_args.sel_args.fields_len = 0;
    cmd->cmd_args.sel_args.limit = -1;
    cmd->cmd_args.sel_args.offset = 0;
    char* field_list[] = { "*", "id", "name", "email", "age", "" };
    while (arg_idx < cmd->args_len) {
        for (size_t idx = 0; strlen(field_list[idx]); idx++) {
            if (!strncmp(cmd->args[arg_idx], field_list[idx], strlen(field_list[idx]))) {
                add_select_field(cmd, cmd->args[arg_idx]);
                break;
            } else if (!strncmp(cmd->args[arg_idx], "from", 4))
                return table_state_handler(cmd, arg_idx + 1);
        }
        arg_idx++;
    }
}
/* select  end  */

/* insert begin */
int handle_insert_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    int ret = 0;
    if (cmd->args_len < 3)
        return 0;
    else if (!strncmp(cmd->args[2], "user", 4)) {
        cmd->cmd_args.ins_args.table = USER;
        User_t *user = command_to_User(cmd);
        if (user) {
            ret = add_User(user_table, user);
            if (ret > 0)
                cmd->type = INSERT_CMD;
        }
    } else if (!strncmp(cmd->args[2], "like", 4)) {
        cmd->cmd_args.ins_args.table = LIKE;
        Like_t *like = command_to_Like(cmd);
        if (like) {
            ret = add_Like(like_table, like);
            if (ret > 0)
                cmd->type = INSERT_CMD;
        }
    }
    return ret;
}

/* insert  end  */

/* update begin */
int handle_update_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    cmd->type = UPDATE_CMD;
    return 1;
}

/* update  end  */

/* delete begin */
int handle_delete_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd) {
    cmd->type = DELETE_CMD;
    return 1;
}

/* delete  end  */

void table_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        if        (!strncmp(cmd->args[arg_idx], "user", 4)) {
            cmd->cmd_args.sel_args.table1 = USER;
        } else if (!strncmp(cmd->args[arg_idx], "like", 4)) {
            cmd->cmd_args.sel_args.table1 = LIKE;
            cmd->cmd_args.sel_args.table2 = -1;
        }
        arg_idx++;
        if (arg_idx == cmd->args_len)
            return ;
        else if (!strncmp(cmd->args[arg_idx], "join", 4))
            return join_state_handler(cmd, arg_idx + 1);
        else if (!strncmp(cmd->args[arg_idx], "where", 5))
            return where_condition1_state_handler(cmd, arg_idx + 1);
        else if (!strncmp(cmd->args[arg_idx], "offset", 6))
            return offset_state_handler(cmd, arg_idx + 1);
        else if (!strncmp(cmd->args[arg_idx], "limit", 5))
            return limit_state_handler(cmd, arg_idx + 1);
        cmd->type = UNRECOG_CMD;
        return ;
    }
}

int getIdx(char *s) {
    if (!strncmp(s, "id1", 3))
        return ID1;
    else if (!strncmp(s, "id2", 3))
        return ID2;
    else if (!strncmp(s, "id", 2))
        return ID;
    else if (!strncmp(s, "name", 4))
        return NAME;
    else if (!strncmp(s, "email", 5))
        return EMAIL;
    else if (!strncmp(s, "age", 4))
        return AGE;
    return -1;
}

void join_state_handler(Command_t *cmd, size_t arg_idx) {
    cmd->cmd_args.sel_args.table2 = LIKE;
    arg_idx += 2;
    cmd->cmd_args.sel_args.field_in_table1 = getIdx(cmd->args[arg_idx]);
    arg_idx += 2;
    cmd->cmd_args.sel_args.field_in_table2 = getIdx(cmd->args[arg_idx]);
    arg_idx++;
    if (arg_idx == cmd->args_len)
        return ;
    else if (!strncmp(cmd->args[arg_idx], "where", 5))
        return where_condition1_state_handler(cmd, arg_idx + 1);
    else if (!strncmp(cmd->args[arg_idx], "offset", 6))
        return offset_state_handler(cmd, arg_idx + 1);
    else if (!strncmp(cmd->args[arg_idx], "limit", 5))
        return limit_state_handler(cmd, arg_idx + 1);
    cmd->type = UNRECOG_CMD;
    return ;
}

void where_condition1_state_handler(Command_t *cmd, size_t arg_idx) {
    while (arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "and", 3)) {
            cmd->where_args.where_logic_op = AND;
            return where_condition2_state_handler(cmd, arg_idx + 1);
        } else if (!strncmp(cmd->args[arg_idx], "or", 2)) {
            cmd->where_args.where_logic_op = OR;
            return where_condition2_state_handler(cmd, arg_idx + 1);
        } else if (!strncmp(cmd->args[arg_idx], "offset", 6)) {
            return offset_state_handler(cmd, arg_idx+1);
        } else if (!strncmp(cmd->args[arg_idx], "limit", 5)) {
            return limit_state_handler(cmd, arg_idx+1);
        } else {
            append_string(&cmd->where_args.where_condition1, cmd->args[arg_idx]);
        }
        arg_idx++;
    }
    cmd->type = UNRECOG_CMD;
    return ;
}

void where_condition2_state_handler(Command_t *cmd, size_t arg_idx) {
    while (arg_idx < cmd->args_len) {
        if (!strncmp(cmd->args[arg_idx], "offset", 6)) {
            offset_state_handler(cmd, arg_idx+1);
            return;
        } else if (!strncmp(cmd->args[arg_idx], "limit", 5)) {
            limit_state_handler(cmd, arg_idx+1);
            return;
        } else {
            append_string(&cmd->where_args.where_condition2, cmd->args[arg_idx]);
        }
        arg_idx++;
    }
    cmd->type = UNRECOG_CMD;
    return ;
}

void offset_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.offset = atoi(cmd->args[arg_idx]);
        arg_idx++;
        if (arg_idx == cmd->args_len)
            return;
        else if (arg_idx < cmd->args_len && !strncmp(cmd->args[arg_idx], "limit", 5))
            return limit_state_handler(cmd, arg_idx+1);
    }
    cmd->type = UNRECOG_CMD;
    return;
}

void limit_state_handler(Command_t *cmd, size_t arg_idx) {
    if (arg_idx < cmd->args_len) {
        cmd->cmd_args.sel_args.limit = atoi(cmd->args[arg_idx]);
        arg_idx++;
        if (arg_idx == cmd->args_len)
            return;
    }
    cmd->type = UNRECOG_CMD;
    return;
}
