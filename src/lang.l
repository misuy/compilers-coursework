%{
    #include "ast.h"
    #include "lang.tab.h"
    #include <string.h>
%}

%option noyywrap

%%

[\s\n] ;

"i32" { yylval.data_type = DATA_TYPE_I32; return TOK_DATA_TYPE; }
"bool" { yylval.data_type = DATA_TYPE_BOOL; return TOK_DATA_TYPE; }

"true" { yylval.bool_value = 1; return TOK_BOOL_VALUE; }
"false" { yylval.bool_value = 0; return TOK_BOOL_VALUE; }

"{" |
"}" |
"(" |
")" |
"<" |
">" |
"=" |
"+" |
"-" |
"*" |
"/" |
"&" |
"|" |
"^" |
"!" |
"," |
";" { return yytext[0]; }
"==" { return TOK_EQUAL; }
"!=" { return TOK_NOT_EQUAL; }

"if" { return TOK_IF_KEYWORD; }
"elif" { return TOK_ELIF_KEYWORD; }
"else" { return TOK_ELSE_KEYWORD; }
"for" { return TOK_FOR_KEYWORD; }

"-"?[0-9]+  { yylval.i32_value = atoi(yytext); return TOK_I32_VALUE; }

[a-zA-Z][a-zA-Z0-9]* { yylval.string = strdup(yytext); return TOK_VAR_NAME; }



%%