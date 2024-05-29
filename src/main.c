#include "lang.tab.h"
#include "ast.h"
#include "stdio.h"

extern FILE *yyin;

int main() {
    yyin = fopen("test", "r");
    Node *result;
    printf("start\n");
    yyparse(&result);
    printf("\n");
    FILE *f = fopen("out", "w");
    print_node(stdout, result, 0);
    //printf("%lu\n", result);
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}