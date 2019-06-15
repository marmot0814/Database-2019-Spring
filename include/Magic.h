#ifndef MAGIC_H
#define MAGIC_H

typedef struct Node_t {
    struct Node_t *l, *r;
    int sz, v, addr;
} Node;

extern Node *rt[];

void init();
void clear(Node *u);
int size(Node *u);
void pull(Node *u);
void split(Node *T, int k, Node **a, Node **b);
Node* merge(Node *a, Node *b);
int rank(Node *u, int v);
int find(Node *u, int v);
void print(Node *u);
void insert(int idx, int v, int addr);

#endif
