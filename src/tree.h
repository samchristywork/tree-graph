#ifndef TREE_H
#define TREE_H

#include <stdio.h>

typedef struct Tree {
  struct Tree *parent;
  char *text;
  struct Tree **children;
  unsigned int children_count;
} Tree;

char **readNodes(FILE *stream);
char **readExtra(FILE *f, int *length);
Tree *processNodes(char **lines, char *root_name);
void free_lines(char **lines);
void free_tree(Tree *root);

#endif
