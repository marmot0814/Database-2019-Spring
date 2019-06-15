#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Magic.h"

Node* newNode(int _v, int _addr) {
    Node *u = (Node*)malloc(sizeof(Node));
    u->sz = 1;
    u->v = _v;
    u->addr = _addr;
    u->l = u->r = NULL;
    return u;
}
Node *rt[6];
void init() {
    memset(rt, 0, sizeof(rt));
}
void clear(Node *u) {
    if (!u) return ;
    clear(u->l); clear(u->r);
    free(u);
}
int size(Node *u) {
    return u ? u->sz : 0;
}
void pull(Node *u) {
    u->sz = 1 + size(u->l) + size(u->r);
}
void split(Node *T, int k, Node **a, Node **b) {
    if (!T) *a = *b = NULL;
    else {
        if (size(T->l) < k) {
            *a = T;
            split(T->r, k - size(T->l) - 1, &((*a)->r), b);
            pull(*a);
        } else {
            *b = T;
            split(T->l, k, a, &((*b)->l));
            pull(*b);
        }
    }
}
Node* merge(Node *a, Node *b) {
    if (!a || !b) return a ? a : b;
    if (rand() % (size(a) + size(b)) < size(a)) {
        a->r = merge(a->r, b); pull(a);
        return a;
    } else {
        b->l = merge(a, b->l); pull(b);
        return b;
    }
}

int rank(Node *u, int v) {
    if (!u) return 0;
    return v < u->v ? rank(u->l, v) : size(u->l) + 1 + rank(u->r, v);
}
int find(Node *u, int v) {
    if (!u) return 0;
    return v == u->v ? 1 : find(u->l, v) || find(u->r, v);
}

void print(Node *u) {
    if (!u) return ;
    print(u->l);
    printf ("%d ", u->v);
    print(u->r);
}

void insert(int idx, int v, int addr) {
    Node *a, *b;
    split(rt[idx], rank(rt[idx], v), &a, &b);
    rt[idx] = merge(merge(a, newNode(v, addr)), b);
}
