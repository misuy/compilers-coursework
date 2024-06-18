//
// Created by vlad on 18.06.24.
//

#ifndef COMPILERS_COURSEWORK_ERROR_H
#define COMPILERS_COURSEWORK_ERROR_H

void yyerror(struct Node** node, const char* s);
void handle_lexem(const char* buff, int rules_matched);

#endif //COMPILERS_COURSEWORK_ERROR_H
