#ifndef PARSE_H
#define	PARSE_H

typedef int (*cpred)(int); 

void copy_next_token(char* text, char* tokn, cpred pred);

char* copy_quote(char* text, char* buf);

cell* tokens(char* text);

#endif