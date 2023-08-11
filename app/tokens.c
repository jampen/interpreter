#include "tokens.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

// definition of global variable in tokens_h
TokenList* g_current_token_list = NULL;

enum CharacterClassification {
    CHCLASS_UNKNOWN,
    CHCLASS_ALPHANUMERIC,
    CHCLASS_WHITESPACE,
};

enum CharacterClassification classify_character(FILE* file) {
    int character = fgetc(file);
    ungetc(character, file);

    if (isalpha(character)) {
        return CHCLASS_ALPHANUMERIC;
    }

    if (isspace(character)) {
        return CHCLASS_WHITESPACE;
    }

    return CHCLASS_UNKNOWN;
}

Token create_token(const char* text) {
    // Copy 'text' into a new string for our resulting token
    size_t text_length = strlen(text);
    size_t num_bytes = text_length + 1;
    char* text_copy = malloc(num_bytes);
    strcpy(text_copy, text);

    // Zero-terminate the string
    text_copy[text_length] = 0;

    Token result_token;
    result_token.text = text_copy;
    result_token.text_length = text_length;
    return result_token;
}

void destroy_token(const Token token) {
    free(token.text);
}


void consume_identifier(FILE* file, OnTokenReceivedFn on_identifier_recieved) {
    fpos_t begin;
    fgetpos(file, &begin);

    size_t string_length = 0;

    while (!feof(file)) {
        enum CharacterClassification classification = classify_character(file);
        int character = fgetc(file);

        if (classification != CHCLASS_ALPHANUMERIC) {
            ungetc(character, file);
            break;
        }

        ++string_length;
    }

    fsetpos(file, &begin);

    // Copy the string from the file
    size_t num_bytes = string_length + 1;
    char* identifier_string = malloc(num_bytes);
    identifier_string[string_length] = 0;
    fread(identifier_string, 1, string_length, file);


    // Create a token using our identifier string, the token should be deallocated by the caller
    Token token = create_token(identifier_string);
    free(identifier_string);

    on_identifier_recieved(token);
}

void ignore_whitespace(FILE* file) {
    // Skip over any whitespace

    while (!feof(file)) {
        int character = fgetc(file);
        if (!isspace(character)) {
            ungetc(character, file);
            return;
        }
    }
}

void tokenize_file(
    const char* filename,
    OnTokenReceivedFn on_token_received
) {
    if (filename == NULL) {
        errno = EINVAL;
        perror("error: filename is NULL");
        return;
    }

    if (on_token_received == NULL) {
        errno = EINVAL;
        perror("error: on_token_received is NULL");
        return;
    }

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("error: could not open file \"%s\"\n", filename);
        fflush(stdout);
        perror("reason");
        return;
    }

    while (1) {
        enum CharacterClassification char_class 
            = classify_character(file);

        if (feof(file)) {
            break;
        }

        if (char_class == CHCLASS_UNKNOWN) {
            printf("error: invalid character %c\n", fgetc(file));
            break;
        }

        switch (char_class) {
            case CHCLASS_ALPHANUMERIC:
                consume_identifier(file, on_token_received);
                break;
            case CHCLASS_WHITESPACE:
                ignore_whitespace(file);
                break;
        }
    }

    fclose(file);
}

void target_token_list(TokenList* list) {
    g_current_token_list = list;
}

TokenList create_token_list() {
    TokenList list;
    list.token_count = 0;
    list.tokens_capacity = 10;
    list.tokens = malloc(sizeof(Token) * 10);
    return list;
}

void delete_token_list(TokenList list) {
    free(list.tokens);
}

void push_token(const Token token) {
    // When the list is full, we will extend the capacity
    if (g_current_token_list->token_count >= g_current_token_list->tokens_capacity) {
        g_current_token_list->token_count *= 2;
        Token* old_tokens = g_current_token_list->tokens;
        g_current_token_list->tokens =
            realloc(
                g_current_token_list->tokens,
                g_current_token_list->token_count * sizeof(Token)
            );
        free(old_tokens);
    }

    // Insert the token
    g_current_token_list->tokens[g_current_token_list->token_count] = token;
    ++g_current_token_list->token_count;
}
