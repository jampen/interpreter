#pragma once

#include <stddef.h>

// Abstract representation of a source code element
typedef struct {
    // Heap allocated string, needs to be de-allocated
    char*   text;
    size_t  text_length;
} Token;

Token create_token(const char* text);
void  destroy_token(const Token token);

typedef void (*OnTokenReceivedFn)(
    const Token token
);

void tokenize_file(
    const char* filename, 
    OnTokenReceivedFn on_token_received
);