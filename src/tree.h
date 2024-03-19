#ifndef TREE_H
#define TREE_H

typedef struct Tree {
  struct Tree *parent;
  char *text;
  struct Tree **children;
  unsigned int children_count;
} Tree;

char **readNodes();
char **readExtra(int *length);
Tree *processNodes(char **lines);
void free_lines(char **lines);
void free_tree(Tree *root);

#endif
