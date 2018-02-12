
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "list.h"
#include "parse.h"

int
streq(const char* aa, const char* bb)
{
    return strcmp(aa, bb) == 0;
}

char*
pop(cell** xs)
{
    char* text = car(*xs)->text;
    *xs = cdr(*xs);
    return text;
}

char*
peek(cell** xs)
{
    return car(*xs)->text;
}

int
is_special(char* cmnd) {
	return 	streq(cmnd, ">") ||
			streq(cmnd, "|") ||
			streq(cmnd, "<") ||
			streq(cmnd, "&") ||
			streq(cmnd, ";");

}

int
arg_type(char* cmnd) {
	if(is_special(cmnd)) {
		return KIND_SPEC;
	}		
	else {
		return KIND_UNIX;
	}
}

char**
arg_array(tree* x) {
	tree* current = x;
	int length = 0;
	
	while(current != 0) {
		length++;
		current = current->arg1;
	}
	current = x;
	//printf("%d\n", length);

	char** args = malloc(length + 1);

	int i;
	for(i = 0; i < length; i++) {
		args[i] = current->cmnd;
		current = current->arg1;
	}
	args[length] = 0;
	//printf("current command: %s\n", args[0]);
	return args;
}



tree*
parse_until_cp(cell** toks)
{
	//pop next token,
	tree* tt = 0;
	
	char* next = peek(toks);
	if(streq(next, ")")) {
		//printf("found closed paren and returning %s\n", next);
		return tt;
	}
	else if(streq(next, "(")) {
		//printf("found open paren and returning to parse new tree\n");
		return tt;
	}
	else {
		tt = malloc(sizeof(tree));
		char* action = pop(toks);
		//printf("getting argument %s\n", action);
		tt->cmnd = action;
		tt->kind = KIND_ARG;
		//printf("parsing until closed paren\n");
		tt->arg1 = parse_until_cp(toks);
	}
	
	return tt;
}

tree*
parse(cell** toks)
{
    tree* tt = malloc(sizeof(tree));

    char* oparen = pop(toks);
	char* cparen;
    assert(streq(oparen, "("));

//if next token is oparen, oparen = pop, 
//parse rest into arg1 until cparen

    char* action = pop(toks);
	//printf("getting action of new tree %s\n", action);
	tt->cmnd = action;
	tt->kind = arg_type(action);

	char* next = peek(toks);
	if(streq(next, "(")) {
		//printf("next is open paren\n");
		tt->arg1 = parse(toks);
	}
	else {
		//printf("parsing until closed paren\n");
		tt->arg1 = parse_until_cp(toks);
		cparen = pop(toks);
		//printf("cparen found, ending tree: %s\n", cparen);
		return tt;
	}

	next = peek(toks);
	//printf("next token is %s\n", next);
	if(streq(next, "(")) {
		//printf("next is open paren, parsing 2nd arg for %s\n", action);
		tt->arg2 = parse(toks);
	}
	else {
		//printf("parsing until closed paren for 2nd arg for %s\n", action);
		tt->arg2 = parse_until_cp(toks);
	}	

	//printf("finished parsing into tree and returning\n");
    return tt;
}

// ("A" "B" "C")

