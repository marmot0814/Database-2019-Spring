#include <stdio.h>
#include <string.h>
#include <unordered_map>

#include "Hash.h"
#include "Table.h"

std::unordered_map<long long, long long> HT[6];

void hash_init(int idx) {
    HT[idx].clear();
}

void hash_insert(int idx, int key) {
    HT[idx][key] = 1;
}

int hash_find(int idx, int key) {
    return HT[idx].count(key);
}

void hash_maintain(Table_t *user_table) {
    hash_init(ID);
    for (size_t idx = 0; idx < user_table->len; idx++) {
        User_t *user = get_User(user_table, idx);
        hash_insert(ID, user->id);
    }
}

void hash_add(int idx, int key) {
    HT[idx][key]++;
}

int hash_get_value(int idx, int key) {
    return HT[idx].count(key) ? HT[idx][key] : 0;
}
