#ifndef QUERY_STATE_H
#define QUERY_STATE_H

#include "Command.h"
#include "Table.h"

int handle_query_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd);

/* select begin */
int handle_select_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd);
void select_field_handler(Command_t *cmd, size_t arg_idx);

/* select  end  */

/* insert begin */
int handle_insert_cmd(Table_t *user_table, Table_t *like_table, Command_t *cmd);
void update_field_state_handler(Command_t *cmd, size_t arg_idx);
void set_state_handler(Command_t *cmd, size_t arg_idx);
/* insert  end  */

/* update begin */
int handle_update_cmd(Table_t *table, Command_t *cmd);

/* update  end  */

/* delete begin */
int handle_delete_cmd(Table_t *table, Command_t *cmd);
void delete_state_handler(Command_t *cmd, size_t arg_idx);

/* delete  end  */

void table_state_handler(Command_t *cmd, size_t arg_idx);
void join_state_handler(Command_t *cmd, size_t arg_idx);
void where_condition1_state_handler(Command_t *cmd, size_t arg_idx);
void where_condition2_state_handler(Command_t *cmd, size_t arg_idx);
void offset_state_handler(Command_t *cmd, size_t arg_idx);
void limit_state_handler(Command_t *cmd, size_t arg_idx);


#endif
