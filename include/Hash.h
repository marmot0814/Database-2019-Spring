#ifndef HASH_H
#define HASH_H

#include "Table.h"

typedef struct Hash_t {
    int key, value;
} Hash;
#define HASH_SIZE 400000
extern Hash HT[];
void hash_init();
int hash_func(long long key, long long i);
void hash_insert(int key);
int hash_find(int key);
void hash_maintain(Table_t *user_table);

#endif
