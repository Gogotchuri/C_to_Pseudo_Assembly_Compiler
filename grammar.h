#ifndef gramma_syn
#define gramma_syn
/* Whole purpose of grammar is to construct syntax rules trees for general C code
 * (I'll inculde diagrams soon.) So we would be able to build AST(abstract syntax tree)
 * using C grammar rules, modify and add new rules if neccessary.
 * */
#include "node.h"
#include "vector.h"
#include "compilatorConstants.h"
//--------------------------stuctures---------------------------------
typedef struct{
    char var_type[64];
    char identifier[128];
}var_t;

typedef struct{
    char name[64];
    var_t params[40];
    int num_param;
}function_t;
//-----------------------------------------------------------------------
typedef struct{
    node * program;
    node * function; 
    node * statement; 
    node * expression;
    vector * all_nodes;
}grammar;

void GrammarNew(grammar * gr);
void GrammarDispose(grammar * gr);

#endif