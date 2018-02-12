#ifndef CALC_LIST_H
#define CALC_LIST_H

#define KIND_CMND 0
#define KIND_OPER 1
#define KIND_FILE 0

typedef struct tokn {
  int kind;
  char* text;
} tokn;

tokn* make_tokn(char* text);
tokn* copy_tokn(tokn* tt);
void  free_tokn(tokn* tt);

void  print_tokn(tokn* tt);

// A list is one of:
//  - a pointer to a cell
//  - the null pointer (0) if empty

typedef struct cell {
  tokn* head;
  struct cell* tail;
} cell;

cell* cons(tokn* hh, cell* tt);
tokn* car(cell* xs);
cell* cdr(cell* xs);

void free_list(cell* xs);

int length(cell* xs);
cell* reverse(cell* xs);

void print_list(cell* xs);

#endif
