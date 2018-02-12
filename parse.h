#ifndef PARSE_H
#define	PARSE_H

#define KIND_ARG 0
#define KIND_UNIX 1
#define KIND_SPEC 2
	
typedef struct tree {
	char* cmnd;
	int kind;
	struct tree* arg1;
	struct tree* arg2;

} tree;

tree* parse(cell** cmnd);

#endif