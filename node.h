#ifndef tree_node
#define tree_node

#include "vector.h"
#include <stdlib.h>
#include <string.h>

typedef void (*NodeFreeFunction) (void* elemAddr);

typedef struct node_{
    void * value;
    unsigned long elem_size;
    NodeFreeFunction free_fn;
    vector children;
} node;

/*Initializes Node object with value which is written on the addr value,
    value should have size of "value_s",
    @param int_num_children is just initial vector allocation*/
void NodeNew(node * n, void * value, unsigned long value_s, 
    int init_num_children, NodeFreeFunction free_fn);

/*Writes children vector address to provided variable*/
void NodeGetChildren(node * n, vector ** dest);

/*Adds child to the node, as the left-most child.
    @param child child to add*/
void NodeAddChild(node * n, node * child);

/*Sets/changes value of the node*/
void NodeSetValue(node * n, void * elem);

void NodeGetValue(node * n, void * addr);

/*Returns  children count*/
int NodeNumChildren(node * n);

/*Disposes Node
    Bare in mind, that this function does free only memory allocated for it's value
    And calls value free function beforehand.
    So Disposing the root before children may cause in memory leaks!*/
void NodeDispose(node * n);

#endif