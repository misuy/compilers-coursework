#include "stdio.h"
#include "ast.h"
#include "string.h"

static int line = 1;
static int first_column = 1;
//static int last_line = 0;
static int last_column = 1;

void yyerror(struct Node** node, const char* s) {
    fprintf(stderr, "Error at line: %d; columns: %d:%d\n",
            first_column, last_column, line);
    print_node(stderr, *node, 0);
}

void handle_lexem(const char* buff, int rules_matched) {
    first_column = last_column;
    for(int i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == '\n') {
            line++;
            last_column = 1;
        } else {
            last_column++;
        }
    }
}