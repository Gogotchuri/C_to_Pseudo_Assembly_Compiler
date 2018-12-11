
#include "tokenizer.h"

/* Prototypes */
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitTypesMap (hashset* typesMap);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static int StringHash (char* str, int numBuckets);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void StringFree (void* strAddr);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitTokenizer(tokenizer* t, const char * source_file);
static void InitBasicTokenizer(vector * vec, const char * source_file_name);
static void InitSmartTokenizer(tokenizer* t, int num_basic_tokens);
static bool GetTokenType(char * token, char * result, hashset * types_map);
static void InsertInTokenType(char * token_, char * type_, vector * vec);
static void SliceToken(char * token, tokenizer * t);
static void EvaluateNonIdentifier(char * token, tokenizer * t);

void TokenizerNew (tokenizer* t, const char * source_file)
{
    InitTypesMap(&(t->typesMap));
    InitTokenizer(t, source_file);
    t->current_index = -1;
}

void TokenizerDispose (tokenizer* t)
{
    HashSetDispose(&(t->typesMap));
    VectorDispose(&(t->basic_tokens));
    VectorDispose(&(t->token_to_type));
}

//---------------------------------------------Code Tokenizer functionality-----------------------------
/*Checks if tokenizer has more tokens(Public)*/
bool TokenizerHaveMore(tokenizer* t){
    if(t->current_index == (VectorLength(&(t->token_to_type)) - 1)) return false;
    return true;
}

/* Moves tokenizer counter one index forward. 
 * Returns true if tokizer moves one index forward, otherwise false
 * 
 * */
bool TokenizerNextTok(tokenizer* t){
    if(!TokenizerHaveMore(t)) return false;
    t->current_index++;
    return true;    
}

/**/
bool TokenizerPeekNth(tokenizer* t, int n, pair **dest){
    int position = t->current_index + n;
    if(position < 0 || position >= (VectorLength(&(t->token_to_type))))
        return false;
    *dest = (pair *)VectorNth(&(t->token_to_type), position);
    return true;
}

bool TokenizerPeekNext(tokenizer* t, pair **dest){
    return TokenizerPeekNth(t, 1, dest);
}

bool TokenizerPeekPrev(tokenizer* t, pair **dest){
    return TokenizerPeekNth(t, -1, dest);
}

bool TokenizerCurrent(tokenizer* t, pair **dest){
    return TokenizerPeekNth(t, 0, dest);
}

//---------------------------------------------Code Tokenizer initialization-----------------------------
/*Initializes smart tokenizer from source file.
    --fills basic tokens vector
    --matches basic tokens with types
    --fills token_to_type vector (pair vector)*/
static void InitTokenizer(tokenizer* t, const char * source_file){
    InitBasicTokenizer(&(t->basic_tokens), source_file);
    int num_basic_tokens = VectorLength(&(t->basic_tokens));
    InitSmartTokenizer(t, num_basic_tokens);
    
}

/*Chops source file into basic tokens and fill basic_token vector*/
static const int line_buffer_size = 1024;
static const char* basic_code_delimiters = " \n\t";
static const int initial_allocation_size = 40; 
static void InitBasicTokenizer(vector * vec, const char * source_file_name){
    VectorNew(vec, sizeof(char*), StringFree, initial_allocation_size);
    FILE* source_file = fopen(source_file_name, "r");
    assert(source_file != NULL);
    char line_buffer[line_buffer_size];
    char * token;
    while (fgets(line_buffer, line_buffer_size, source_file) != NULL)
    {
        token = strtok(line_buffer, basic_code_delimiters);
        if(token == NULL)
            continue;
        token = strdup(token);
        VectorAppend(vec, &token);
        token = strtok(NULL, basic_code_delimiters);
        while (token != NULL)
        {
            token = strdup(token);
            VectorAppend(vec, &token);
            token = strtok(NULL, basic_code_delimiters);
        }
    }

    fclose(source_file);
}

/*Initializes token types vector with tokens and adequate types
    @param t tokenizer struct address, which is ought to be changed
    @param num_basic_tokens is size of basic_token vector*/
static void InitSmartTokenizer(tokenizer* t, int num_basic_tokens){
    vector * token_to_type = &(t->token_to_type);
    vector * basic_tokens = &(t->basic_tokens);
    VectorNew(token_to_type, sizeof(pair), PairFreeFn, num_basic_tokens);
    int buff_size = 64;
    char *token = NULL, type_buff[buff_size];
    for(int i = 0; i < num_basic_tokens; i++){
        token = *((char**)VectorNth(basic_tokens, i));
        if(!GetTokenType(token, type_buff, &(t->typesMap))){
            SliceToken(token, t);
        }else{
            InsertInTokenType(token, type_buff, token_to_type);
        }
    }
        
}

/*After evaluating token as undefined, we assume that it is combination
    of few parts and try to slice it into sensible parts.
    
    
    @param token string to slice
    @param vec token_to_type vector to push into defined token with type

    id : short term for identifier*/
static void SliceToken(char * token, tokenizer * t){
    //base case
    int tok_len = strlen(token);
    if(tok_len == 0) return;

    vector * vec = &(t->token_to_type);
    char type_buff[64];

    //first index of identifier char and 
    int first_id_char, last_id_char;

    for(first_id_char = 0; first_id_char < tok_len; first_id_char++)
        if(is_id_char(token[first_id_char], true)) break;
    
    if(first_id_char == tok_len){
        EvaluateNonIdentifier(token, t);
        return;
    }

    //looking for last occurance of identifier character after first
    for(last_id_char = first_id_char; last_id_char < tok_len; last_id_char++)
        if(!is_id_char(token[last_id_char], false)) break;
    
    last_id_char -= 1;
    
    char *first_part, *pos_id_part, *last_part;

    //as we know we don't have identifier before first id char index
    //example: ++k
    if(first_id_char != 0){
        first_part = GetStringBetween(0, first_id_char - 1, token);
        EvaluateNonIdentifier(first_part, t);
        free(first_part);
    }

    pos_id_part = GetStringBetween(first_id_char, last_id_char, token);
    GetTokenType(pos_id_part, type_buff, &(t->typesMap));
    InsertInTokenType(pos_id_part, type_buff, vec);
    free(pos_id_part);
    if(last_id_char == tok_len - 1) return;
    last_part = GetStringBetween(last_id_char + 1, tok_len - 1, token);
    SliceToken(last_part, t);
    free(last_part);
}

/*After deducing that this token can't be an identifier, we try to chop this up in a way
    that fits any identifier, if not successful token is marked with "undefined" flag (nothing we can do)
    !!!Known bugs, can't deduce grouped strings and strings need to fix later
    !!!Also need to evaluate as large part at once as possible*/
static void EvaluateNonIdentifier(char * token, tokenizer * t){
    //base case 1
    int tok_len = strlen(token);
    if(tok_len == 0) return;

    char one_sized[2], type_buff[64], *first_part, *number_part, *last_part;
    vector * vec = &(t->token_to_type);

    /*Base case: Token is already a type*/
    if(GetTokenType(token, type_buff, &(t->typesMap))){
        GetTokenType(token, type_buff, &(t->typesMap));
        InsertInTokenType(token, type_buff, vec);
        return;
    }

    /*Token might contain literal (checking for number literal)*/
    int first_digit, last_digit;
    for(first_digit = 0; first_digit < tok_len; first_digit++)
        if(is_digit(token[first_digit])) break;
    
    if(first_digit != tok_len){
    //looking for last occurance of digit after first
        for(last_digit = first_digit; last_digit < tok_len; last_digit++)
            if(!is_digit(token[last_digit])) break;
        last_digit -= 1;

        //Case when token doesn't start with number, we take first part and reevaluate
        if(first_digit != 0){
            first_part = GetStringBetween(0, first_digit - 1, token);
            EvaluateNonIdentifier(first_part, t);
            free(first_part);
        }

        number_part = GetStringBetween(first_digit, last_digit, token);
        GetTokenType(number_part, type_buff, &(t->typesMap));
        InsertInTokenType(number_part, type_buff, vec);
        if(last_digit == tok_len - 1) return;
        last_part = GetStringBetween(last_digit + 1, tok_len - 1, token);
        EvaluateNonIdentifier(last_part, t);
        free(number_part);
        free(last_part);
        return;
    }

    for(int i = 0; i < tok_len; i++){
        one_sized[0] = token[i];
        one_sized[1] = '\0';
        GetTokenType(one_sized, type_buff, &(t->typesMap));
        InsertInTokenType(one_sized, type_buff, vec);
    }

    
}
/*Creates pair struct, initializes it with passed token and type and pushes it in vector
    @param token_ string for pair.first
    @param type_ string for pair.second
    @param vec already initialized vector which takes elems with size sizeof(pair)*/
static void InsertInTokenType(char * token_, char * type_, vector * vec){
    char *token, *type;
    token = strdup(token_);
    type = strdup(type_);
    pair to_append;
    PairNew(&to_append, &token, &type, sizeof(char*), sizeof(char*),
        StringFree, StringFree);
    VectorAppend(vec, &to_append);

}

/*Deduces type of passed token
    @param token is string to find type of
    @param result already allocated or buffered string, where result will be copied
    @param types_map already constructed map (hashset of pairs) with basic type naming in it
    
    Function checks token for type in following order:
    1-keyword / var_type / space separator({, (, ;, ...)) / operator (binary or unary)
    2-literal (int, char, string)
    3-identifier
    
    If token can't be evaluated in those simple checks, function returns false and type:
    "undefined". so we assume that token is recombination of other tokens*/
static bool GetTokenType(char * token, char * result, hashset * types_map){
    pair p;
    PairNew(&p, &token, &result, sizeof(char*), sizeof(char*), 
        NULL, NULL);
    char * substitute;
    //is defined in type map
    pair * addr = HashSetLookup(types_map, &p);
    PairDispose(&p);  
    if(addr != NULL){
        PairGetSecond(addr, &substitute);
        strcpy(result, substitute);
        return true;
    }
    //token can be evluated with value
    int have_value = is_literal(token);
    switch(have_value){
        case 1:
            strcpy(result, NUM_LITERAL);
            break;
        case 2:
            strcpy(result, CHAR_LITERAL);
            break;
        case 3:
            strcpy(result, STR_LITERAL);
            break;
        default:
            break;
    }

    if(have_value != 0) return true;

    //token is identifier
    if(is_identifier(token)){
        strcpy(result, IDENTIFIER);
        return true;
    }

    //base case: can't evaluate token with simple rules.
    //token needs to be sliced
    strcpy(result, UNDEFINED);
    return false;
}

//---------------------------------------------Tokenizer initialization End-----------------------------

//--------------------------------------psd JSON parsing----------------------------
/**
 * (out of procedural paradigms, need change)
 * Initializes typesMap as
 * set< pair< string, string > >
 * from typesFileName file.
 * Every row of typesFileName file MUST BE type of:
 * "type" : ["x", "y", ... "z"]
 * Result of upper example looks like that:
 * set of (x->type), (y->type), (x->type)
 */
static const char* typesFileName = "types.txt";
static const char* typesFileDelimiters = " \n";
static void InitTypesMap (hashset* typesMap)
{
   FILE* typesFile;
    typesFile = fopen(typesFileName, "r");
    assert(typesFile != NULL);

    HashSetNew(typesMap, sizeof(pair), 10007,
               PairHashFnByFirstString, PairCmpFnByFirstString, PairFreeFn);

    char lineBuffer[line_buffer_size];
    char *generalType, *newGeneralType, *nextType;
    pair p;

    while (fgets(lineBuffer, line_buffer_size, typesFile) != NULL)
    {
        generalType = strtok(lineBuffer, typesFileDelimiters);
        generalType = GetStringBetween(1, strlen(generalType) - 2, generalType);

        strtok(NULL, typesFileDelimiters); // skip ":"
        
        nextType = strtok(NULL, typesFileDelimiters);
        nextType = GetStringBetween(2, strlen(nextType) - 3, nextType);

        while (true)
        {
            newGeneralType = strdup(generalType); // create new general type for pair
            assert(newGeneralType != NULL);
            PairNew(&p, &nextType, &newGeneralType,
                    sizeof(char*), sizeof(char*),
                    StringFree, StringFree);

            HashSetEnter(typesMap, &p);

            nextType = strtok(NULL, typesFileDelimiters);
            if (nextType == NULL)
                break;
            nextType = GetStringBetween(1, strlen(nextType) - 3, nextType);
        }

        free(generalType);
    }

    fclose(typesFile);
}


/**
 * Hash function for typesMap hashset.
 * Hashes by first string of pair.
 * Uses StringHash helper function.
 */
int PairHashFnByFirstString (const void* vpairAddr, int numBuckets)
{
    pair* pairAddr = (pair*)vpairAddr;
    char* first; PairGetFirst(pairAddr, &first);
    return StringHash(first, numBuckets);
}

/**
 * String hash function.
 */
static const signed long kHashMultiplier = -1664117991L;
static int StringHash (char* str, int numBuckets)
{
  unsigned long hashcode = 0;
  for (int i = 0; i < strlen(str); i++)
    hashcode = hashcode * kHashMultiplier + tolower(str[i]);
  return hashcode % numBuckets;                                
}

/**
 * Comparator function for pairs.
 * Compares pairs by first string.
 */
int PairCmpFnByFirstString (const void *vpairAddr1, const void *vpairAddr2)
{
    pair* pairAddr1 = (pair*)vpairAddr1;
    pair* pairAddr2 = (pair*)vpairAddr2;

    char* first1; PairGetFirst(pairAddr1, &first1);
    char* first2; PairGetFirst(pairAddr2, &first2);

    return strcmp(first1, first2);
}

/**
 * Just PairDispose.
 */
void PairFreeFn (void *vpairAddr)
{
    pair* pairAddr = (pair*)vpairAddr;
    PairDispose(pairAddr);
}

/**
 * Example:
 * GetStringBetween(1, 4, "abcdef") -> "bcde"
 * Allocates space in heap for result and returns pointer to that memory.
 */
char* GetStringBetween (int startIndex, int finishIndex, char* str)
{
    int numChars = finishIndex - startIndex + 1;
    char* result = malloc(numChars + 1); // + 1 for '\0'
    memcpy(result, str + startIndex, numChars);
    result[numChars] = '\0'; // last character
    return result;
}

/**
 * Free function for strings.
 */
static void StringFree (void* strAddr)
{
  free(*(char**)strAddr);
}
//-------------------------------------END of JSON Parser----------------------------