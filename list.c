
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "list.h"

/*
#define KIND_NUMB 0
#define KIND_OPER 1

typedef struct numb {
  int kind;
  int numb;
} numb;

typedef struct oper {
  int kind;
  int oper;
} oper;

typedef struct tokn {
  int kind;
} tokn;
*/

tokn*
make_tokn(char* txt)
{
    tokn* tt = malloc(sizeof(tokn));
    tt->kind = KIND_CMND;
    tt->text = strdup(txt);
    return tt;
}

tokn*
copy_tokn(tokn* tt)
{
    return make_tokn(tt->text);
}

void
free_tokn(tokn* tt)
{
    free(tt->text);
    free(tt);
}

void
print_tokn(tokn* tt)
{
    printf("\"%s\"", tt->text);
}

/*
// A list is one of:
//  - a pointer to a cell
//  - the null pointer (0) if empty

typedef struct cell {
  tokn* head;
  struct cell* tail;
} cell;
*/

cell*
cons(tokn* hh, cell* tt)
{
    cell* ys = malloc(sizeof(cell));
    ys->head = hh;
    ys->tail = tt;
    return ys;
}

tokn*
car(cell* xs)
{
    return xs->head;
}

cell*
cdr(cell* xs)
{
    return xs->tail;
}

void
free_list(cell* xs)
{
    if (xs == 0) {
        // do nothing
    }
    else {
        free_tokn(car(xs));
        free_list(cdr(xs));
        free(xs);
    }
}

int
length(cell* xs)
{
    if (xs == 0) {
        return 0;
    }
    else {
        return 1 + length(cdr(xs));
    }
}

static
cell*
reverse1(cell* xs, cell* acc)
{
    if (xs == 0) {
        return acc;
    }
    else {
        return reverse1(cdr(xs), cons(copy_tokn(car(xs)), acc));
    }
}

cell*
reverse(cell* xs)
{
    return reverse1(xs, 0);
}

void
print_list(cell* xs)
{
    for (cell* it = xs; it != 0; it = cdr(it)) {
        print_tokn(car(it));
        printf(" ");
    }
    printf("\n");
}

