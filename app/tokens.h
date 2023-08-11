#pragma once

#include <stddef.h>

// Abstract representation of a source code element
typedef struct TOKEN {
    // Heap allocated string, needs to be de-allocated
    char*   text;
    size_t  text_length;
} Token;

typedef struct TOKEN_LIST {
    size_t token_count;
    size_t tokens_capacity;
    Token* tokens;
} TokenList;

// This global variable is the target for pushing tokens
extern TokenList* g_current_token_list;

Token create_token(const char* text);
void  destroy_token(const Token token);

typedef void (*OnTokenReceivedFn)(
    const Token token
);

void tokenize_file(
    const char* filename, 
    OnTokenReceivedFn on_token_received
);

TokenList create_token_list();
void      push_token(Token token);
void      delete_token_list(TokenList list);
void      target_token_list(TokenList* list);



