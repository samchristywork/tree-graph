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

char **readNodes() {
  char **lines = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned int count = 0;
  while ((read = getline(&line, &len, stdin)) != -1) {
    if (strstr(line, "--\n") == line) {
      break;
    }
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    lines = realloc(lines, sizeof(char *) * (count + 1));
    lines[count] = strdup(line);
    count++;
  }
  free(line);

  lines = realloc(lines, sizeof(char *) * (count + 1));
  lines[count] = NULL;

  return lines;
}

char **readExtra(int *length) {
  char **lines = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned int count = 0;
  while ((read = getline(&line, &len, stdin)) != -1) {
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    lines = realloc(lines, sizeof(char *) * (count + 1));
    lines[count] = strdup(line);
    count++;
  }
  free(line);

  lines = realloc(lines, sizeof(char *) * (count + 1));
  lines[count] = NULL;

  *length = count;
  return lines;
}
