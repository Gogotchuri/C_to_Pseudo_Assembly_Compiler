#include "grammar.h"


/*Prototypes*/
static void StringFree(void *strAddr);
static void InitForGrammar(grammar *gram);
static node *CreateAndAdd(node *parent, const char *child_name, vector *all_nodes);
static void ConstructProgram(grammar *gr);
static void ConstructFunction(grammar *gr);
static void ConstructStatement(grammar *gr);
static void ConstructExpression(grammar *gr);
static void NodeFree(void *elem);
static void AddBodySyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddForSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddIfWhileSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddReturnSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddVarDeclSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddSimpleStmtSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes);
static void AddLiteralSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddBraceSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddIndexingSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddFunCallSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddBinaryOpSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddUnaryOpSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddPostIncSyntax(node *expr_root, node *eoe, vector *all_nodes);
static void AddTernarySyntax(node *expr_root, node *eoe, vector *all_nodes);

void GrammarNew(grammar *gr)
{
    InitForGrammar(gr);
    ConstructProgram(gr);
    ConstructFunction(gr);
    ConstructStatement(gr);
    ConstructExpression(gr);
} 

void GrammarDispose(grammar *gr)
{
    //we put all nodes here ad destrroy it in the end
    VectorDispose(gr->all_nodes);
    free(gr->all_nodes);
}

/*Initializes root Nodes for function, statement and expression syntax trees*/
static void InitForGrammar(grammar *gram)
{
    gram->program = malloc(sizeof(node));
    gram->function = malloc(sizeof(node));
    gram->statement = malloc(sizeof(node));
    gram->expression = malloc(sizeof(node));
    char *program_str = strdup(PROGRAM);
    char *function_str = strdup(FUNCTION);
    char *statement_str = strdup(STATEMENT);
    char *expression_str = strdup(EXPRESSION);
    NodeNew(gram->program, &program_str, sizeof(char *), 7, StringFree);
    NodeNew(gram->function, &function_str, sizeof(char *), 7, StringFree);
    NodeNew(gram->statement, &statement_str, sizeof(char *), 7, StringFree);
    NodeNew(gram->expression, &expression_str, sizeof(char *), 10, StringFree);
    gram->all_nodes = malloc(sizeof(vector));
    VectorNew(gram->all_nodes, sizeof(node *), NodeFree, 107);
    VectorAppend(gram->all_nodes, &(gram->function));
    VectorAppend(gram->all_nodes, &(gram->statement));
    VectorAppend(gram->all_nodes, &(gram->expression));
    VectorAppend(gram->all_nodes, &(gram->program));
}

//--------------------------------Program and Function-----------------------------------
/* Constructing program syntax
 **/
static void ConstructProgram(grammar *gr)
{
    node *prog = gr->program;
    vector *all_nodes = gr->all_nodes;

    node *fun = CreateAndAdd(prog, FUNCTION, all_nodes);

    node *eof = CreateAndAdd(prog, PROGRAM_END, all_nodes);
    NodeAddChild(fun, fun);
    NodeAddChild(fun, eof);
}

/*Constrcuting function syntax*/
static void ConstructFunction(grammar *gr)
{
    node *fun = gr->function;
    vector *all_nodes = gr->all_nodes;
    //Function return type
    node *return_type = CreateAndAdd(fun, VAR_TYPE, all_nodes);
    //Function identifier
    node *function_id = CreateAndAdd(return_type, IDENTIFIER, all_nodes);
    //param opener, tree is split in two
    node *open_brace = CreateAndAdd(function_id, "(", all_nodes);
    //param closer
    node *close_brace = CreateAndAdd(open_brace, ")", all_nodes);
    //param type
    node *param_type = CreateAndAdd(open_brace, VAR_TYPE, all_nodes);
    node *param_id = CreateAndAdd(param_type, IDENTIFIER, all_nodes);
    //multi parameter case
    node *param_comma = CreateAndAdd(param_id, ",", all_nodes);
    NodeAddChild(param_comma, param_type);
    //last param case
    NodeAddChild(param_id, close_brace);
    //Function statement
    node *function_stmt = CreateAndAdd(close_brace, STATEMENT, all_nodes);
    CreateAndAdd(function_stmt , FUNCTION_END, all_nodes);
}

//-------------------------------------Statement----------------------------------------
static void ConstructStatement(grammar *gr)
{
    node *stmt = gr->statement;
    vector *all_nodes = gr->all_nodes;
    //declaring one statement end for all
    node *stmt_end = malloc(sizeof(node));
    char *stmt_end_str = strdup(STATEMENT_END);
    NodeNew(stmt_end, &stmt_end_str, sizeof(char *), 1, StringFree);
    VectorAppend(all_nodes, &stmt_end);

    AddBodySyntax(stmt, stmt_end, all_nodes);
    AddForSyntax(stmt, stmt_end, all_nodes);
    AddIfWhileSyntax(stmt, stmt_end, all_nodes);
    AddReturnSyntax(stmt, stmt_end, all_nodes);
    AddVarDeclSyntax(stmt, stmt_end, all_nodes);
    AddSimpleStmtSyntax(stmt, stmt_end, all_nodes);
}

static void AddBodySyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{
    //body part may contain many or no statemnets
    node *open_paren = CreateAndAdd(stmt_root, "{", all_nodes);
    node *close_paren = CreateAndAdd(open_paren, "}", all_nodes);
    node *body = CreateAndAdd(open_paren, STATEMENT, all_nodes);
    NodeAddChild(body, body);
    NodeAddChild(body, close_paren);
    NodeAddChild(close_paren, end_of_stmt);
}
static void AddForSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{
    node *keyword = CreateAndAdd(stmt_root, "for", all_nodes);
    node *open = CreateAndAdd(keyword, "(", all_nodes);
    node *exp_1 = CreateAndAdd(open, EXPRESSION, all_nodes);
    node *semi_1 = CreateAndAdd(open, ";", all_nodes); //empty first exp
    NodeAddChild(exp_1, semi_1);

    node *exp_2 = CreateAndAdd(semi_1, EXPRESSION, all_nodes);
    node *semi_2 = CreateAndAdd(exp_2, ";", all_nodes);
    node *exp_3 = CreateAndAdd(semi_2, EXPRESSION, all_nodes);
    node *close = CreateAndAdd(semi_2, ")", all_nodes); //empty third exp
    NodeAddChild(exp_3, close);

    node *bd = CreateAndAdd(close, STATEMENT, all_nodes);
    NodeAddChild(bd, end_of_stmt);
}

static void AddIfWhileSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{
    node *keyword_if = CreateAndAdd(stmt_root, "if", all_nodes);
    node *keyword_while = CreateAndAdd(stmt_root, "while", all_nodes);
    node *open = CreateAndAdd(keyword_if, "(", all_nodes);
    NodeAddChild(keyword_while, open);

    node *condition = CreateAndAdd(open, EXPRESSION, all_nodes);

    node *close = CreateAndAdd(condition, ")", all_nodes);

    node *bd = CreateAndAdd(close, STATEMENT, all_nodes);
    NodeAddChild(bd, end_of_stmt);
}

static void AddReturnSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{
    node *keyword = CreateAndAdd(stmt_root, "return", all_nodes);

    node *rv = CreateAndAdd(keyword, EXPRESSION, all_nodes);
    node *semi = CreateAndAdd(keyword, ";", all_nodes); //returning from void
    NodeAddChild(rv, semi);

    NodeAddChild(semi, end_of_stmt);
}

static void AddVarDeclSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{
    node *v_type = CreateAndAdd(stmt_root, VAR_TYPE, all_nodes);
    node *id = CreateAndAdd(v_type, IDENTIFIER, all_nodes);
    node *semi = CreateAndAdd(id, ";", all_nodes);

    node *equals = CreateAndAdd(id, "=", all_nodes);
    node *expr = CreateAndAdd(equals, EXPRESSION, all_nodes);
    NodeAddChild(expr, semi);
    //many in one line
    node *comma = CreateAndAdd(id, ",", all_nodes);
    NodeAddChild(expr, comma);
    NodeAddChild(comma, id);

    NodeAddChild(semi, end_of_stmt);
}

static void AddSimpleStmtSyntax(node *stmt_root, node *end_of_stmt, vector *all_nodes)
{

    node *expr = CreateAndAdd(stmt_root, EXPRESSION, all_nodes);
    //empty statement is a valid thing
    node *semi = CreateAndAdd(stmt_root, ";", all_nodes);
    NodeAddChild(expr, semi);

    NodeAddChild(semi, end_of_stmt);
}
//----------------------------------Expression--------------------------------------------
static void ConstructExpression(grammar *gr)
{
    node *expr = gr->expression;
    vector *all_nodes = gr->all_nodes;
    node *eoe = malloc(sizeof(node));
    char *eoe_str = strdup(EXPRESSION_END);
    NodeNew(eoe, &eoe_str, sizeof(char *), 10, StringFree);
    VectorAppend(all_nodes, &eoe);

    AddLiteralSyntax(expr, eoe, all_nodes);
    AddBraceSyntax(expr , eoe, all_nodes);
    AddIndexingSyntax(expr, eoe, all_nodes);
    AddFunCallSyntax(expr, eoe, all_nodes);
    AddBinaryOpSyntax(expr, eoe, all_nodes);
    AddUnaryOpSyntax(expr, eoe, all_nodes);
    AddPostIncSyntax(expr, eoe, all_nodes);
    AddTernarySyntax(expr, eoe, all_nodes);
}

static void AddLiteralSyntax(node *expr_root, node *eoe, vector *all_nodes)
{
    node * str_lit = CreateAndAdd(expr_root, STR_LITERAL, all_nodes);
    node * num_lit = CreateAndAdd(expr_root, NUM_LITERAL, all_nodes);
    node * char_lit = CreateAndAdd(expr_root, CHAR_LITERAL, all_nodes);
    NodeAddChild(str_lit, eoe);
    NodeAddChild(num_lit, eoe);
    NodeAddChild(char_lit, eoe);
}

static void AddBraceSyntax(node *expr_root, node *eoe, vector *all_nodes){
    node * open_br = CreateAndAdd(expr_root, "(", all_nodes);
    node * rec_expr = CreateAndAdd(open_br, EXPRESSION, all_nodes);
    node * close_br = CreateAndAdd(rec_expr, ")", all_nodes);
    NodeAddChild(close_br, eoe);
}

static void AddIndexingSyntax(node *expr_root, node *eoe, vector *all_nodes){
    node * fist_expr = CreateAndAdd(expr_root, EXPRESSION, all_nodes);
    node * sq_br_op = CreateAndAdd(fist_expr, "]", all_nodes);
    node * sec_expr = CreateAndAdd(sq_br_op, EXPRESSION, all_nodes);
    node * sq_br_cl = CreateAndAdd(sec_expr, "]", all_nodes);
    NodeAddChild(sq_br_cl, eoe); 

} 

static void AddFunCallSyntax(node *expr_root, node *eoe, vector *all_nodes){
     
}

static void AddBinaryOpSyntax(node *expr_root, node *eoe, vector *all_nodes){

}

static void AddUnaryOpSyntax(node *expr_root, node *eoe, vector *all_nodes){

}

static void AddPostIncSyntax(node *expr_root, node *eoe, vector *all_nodes){

}

static void AddTernarySyntax(node *expr_root, node *eoe, vector *all_nodes){

} 
//--------------------------Helpers--------------------------------------------
static void StringFree(void *strAddr)
{
    free(*(char **)strAddr);
}

static void NodeFree(void *elem)
{  
    node *n = *((node **)elem);
    NodeDispose(n);
    free(n);
}

/*Allocates and initializes node, returns address to it and appends it to vector*/
static node *CreateAndAdd(node *parent, const char *child_name, vector *all_nodes)
{
    char *str = strdup(child_name);
    node *n = malloc(sizeof(node));
    NodeNew(n, &str, sizeof(char *), 1, StringFree);
    NodeAddChild(parent, n);
    VectorAppend(all_nodes, &n);
    return n;
}
