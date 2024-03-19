#ifndef TREE_H
#define TREE_H

typedef struct Tree {
  struct Tree *parent;
  char *text;
  struct Tree **children;
  unsigned int children_count;
} Tree;

#endif
