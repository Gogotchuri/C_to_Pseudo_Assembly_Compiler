#include "node.h"

void NodeNew(node * n, void * value, unsigned long value_s, 
    int init_num_children, NodeFreeFunction free_fn){
    n->free_fn = free_fn;
    n->elem_size = value_s;
    VectorNew(&(n->children), sizeof(node *), NULL, init_num_children);
    n->value = malloc(value_s);
    memcpy(n->value, value, value_s);
}

void NodeDispose(node * n){
    VectorDispose(&(n->children));
    if(n->free_fn != NULL)
        n->free_fn(n->value);
    free(n->value);
}

void NodeGetChildren(node * n, vector ** dest){
    *dest = &(n->children);
}

void NodeAddChild(node * n, node * child){
    VectorAppend(&(n->children), &child);
}

void NodeSetValue(node * n, void * elem){
    memcpy(n->value, elem, n->elem_size);
}

void NodeGetValue(node * n, void * addr){
    memcpy(addr, n->value, n->elem_size);
}

int NodeNumChildren(node * n){
    return VectorLength(&(n->children));
}