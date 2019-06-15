#include <stdio.h>
#include <string.h>

#include "Hash.h"
#include "Table.h"

#define HASH_SIZE 400000
Hash HT[HASH_SIZE];
void hash_init() {
    memset(HT, -1, sizeof(HT));
}
int hash_func(long long key, long long i) {
    return ((long long)(key % HASH_SIZE + 0.5 * i + 0.5 * i * i)) % HASH_SIZE;
}
void hash_insert(int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (~HT[j].key)
            continue;
        HT[j].key = key;
        return ;
    }
}
int hash_find(int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (HT[j].key == key)
            return 1;
        if (!~HT[j].key)
            return 0;
    }
    return 0;
}
void hash_maintain(Table_t *user_table) {
    hash_init();
    for (size_t idx = 0; idx < user_table->len; idx++) {
        User_t *user = get_User(user_table, idx);
        hash_insert(user->id);
    }
}
