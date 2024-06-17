#include "lang.tab.h"
#include "ast.h"
#include "stdio.h"
#include "ir.h"

extern FILE *yyin;

int main() {
    yyin = fopen("test", "r");
    Node *result;
    printf("start\n");
    yyparse(&result);
    printf("\n");
    print_node(stdout, result, 0);
    printf("before ir\n");
    IR *ir = ir_build(result);
    printf("ir\n");
    ir_print(stdout, ir);
    printf("after ir\n");
    //printf("%lu\n", result);
}

void yyerror(const char* s) {
	fprintf(stderr, "Parse error: %s\n", s);
	exit(1);
}