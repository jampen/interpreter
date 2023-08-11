#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

size_t num_tokens = 0;

void on_token_received(const Token token) {
    ++num_tokens;
    printf("token: %s\n", token.text);
    destroy_token(token);
}

int main() {
    tokenize_file("data/hello.script", on_token_received);
    printf("num-tokens: %lu\n", num_tokens);
    return 0;
}