#ifndef HASH_H
#define HASH_H

#include "Table.h"

typedef struct Hash_t {
    int key, value;
} Hash;
#define HASH_SIZE 400000
extern Hash HT[][HASH_SIZE];
void hash_init(int idx);
int hash_func(long long key, long long i);
void hash_insert(int idx, int key);
int hash_find(int idx, int key);
void hash_maintain(Table_t *user_table);
void hash_add(int idx, int key);
int hash_get_value(int idx, int key);


#endif
