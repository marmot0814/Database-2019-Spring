#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "InputBuffer.h"
#include "QueryState.h"
#include "Util.h"
#include "Table.h"
#include "Hash.h"

int main(int argc, char **argv) {
    InputBuffer_t *input_buffer = new_InputBuffer();
    Command_t *cmd = new_Command();
    State_t *state = new_State();
    Table_t *user_table = NULL, *like_table = NULL;
    hash_init(ID); hash_init(ID1); hash_init(ID2);
    int cmd_type;
    if (argc != 2) {
        user_table = new_Table(NULL, USER);
        like_table = new_Table(NULL, LIKE);
    } else {
        user_table = new_Table(argv[1], USER);
        like_table = new_Table(NULL, LIKE);
    }
    if (!user_table || !like_table) {
        return 1;
    }
    for (;;) {
        print_prompt(state);
        read_input(input_buffer);
        cmd_type = parse_input(input_buffer->buffer, cmd);
        if (cmd_type == BUILT_IN_CMD) {
            handle_builtin_cmd(user_table, like_table, cmd, state);
        } else if (cmd_type == QUERY_CMD) {
            handle_query_cmd(user_table, like_table, cmd);
        } else if (cmd_type == UNRECOG_CMD) {
            printf("Unrecognized command '%s'.\n", input_buffer->buffer);
        }
        cleanup_Command(cmd);
        clean_InputBuffer(input_buffer);
    }
    return 0;
}
