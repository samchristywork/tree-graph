#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

void add_child(Tree *tree, const char *text) {
  int count = tree->children_count + 1;
  tree->children = realloc(tree->children, sizeof(Tree *) * count);
  tree->children[tree->children_count] = malloc(sizeof(Tree));
  tree->children[tree->children_count]->text = strdup(text);
  tree->children[tree->children_count]->children = NULL;
  tree->children[tree->children_count]->children_count = 0;
  tree->children[tree->children_count]->parent = tree;
  tree->children_count++;
}

void print_tree(Tree *tree, int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
  printf("%s\n", tree->text);
  for (unsigned int i = 0; i < tree->children_count; i++) {
    print_tree(tree->children[i], depth + 1);
  }
}
