#include "tokens.h"
#include <stdio.h>
#include <stdlib.h>

void on_token_received(const Token token) {
    printf("token: %s\n", token.text);
    push_token(token);
    destroy_token(token);
}

int main() {
    TokenList list = create_token_list();
    target_token_list(&list);
    tokenize_file("data/hello.script", on_token_received);
    printf("num-tokens: %lu\n", list.token_count);
    delete_token_list(list);
    return 0;
}