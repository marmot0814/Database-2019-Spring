#include <stdio.h>
#include <string.h>

#include "Hash.h"
#include "Table.h"

#define HASH_SIZE 400000
Hash HT[6][HASH_SIZE];

void hash_init(int idx) {
    for (int i = 0 ; i < HASH_SIZE ; i++)
        HT[idx][i].key = -1, HT[idx][i].value = 0;
}
int hash_func(long long key, long long i) {
    return ((long long)(key % HASH_SIZE + 0.5 * i + 0.5 * i * i)) % HASH_SIZE;
}
void hash_insert(int idx, int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (~HT[idx][j].key)
            continue;
        HT[idx][j].key = key;
        return ;
    }
}
int hash_find(int idx, int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (HT[idx][j].key == key)
            return 1;
        if (!~HT[idx][j].key)
            return 0;
    }
    return 0;
}
void hash_maintain(Table_t *user_table) {
    hash_init(ID);
    for (size_t idx = 0; idx < user_table->len; idx++) {
        User_t *user = get_User(user_table, idx);
        hash_insert(ID, user->id);
    }
}

void hash_add(int idx, int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (HT[idx][j].key == key) {
            HT[idx][j].value++;
            return ;
        }
        if (!~HT[idx][j].key) {
            HT[idx][j].key = key;
            HT[idx][j].value++;
            return ;
        }
    }
}

int hash_get_value(int idx, int key) {
    for (int i = 0 ; i < HASH_SIZE ; i++) {
        int j = hash_func(key, i);
        if (HT[idx][j].key == key)
            return HT[idx][j].value;
        if (!~HT[idx][j].key)
            return 0;
    }
    return 0;
}
