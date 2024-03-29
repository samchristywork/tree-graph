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

char **readNodes(FILE *stream) {
  char **lines = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned int count = 0;
  while ((read = getline(&line, &len, stream)) != -1) {
    if (strstr(line, "--\n") == line) {
      break;
    }
    if (line[read - 1] == '\n') {
      line[read - 1] = '\0';
    }
    for (int i = 0; i < read; i++) {
      if (line[i] == '|') {
        line[i] = '\n';
      }
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

char **readExtra(FILE *f, int *length) {
  char **lines = NULL;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned int count = 0;
  while ((read = getline(&line, &len, f)) != -1) {
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

int getDepthFromIndentation(const char *line) {
  int depth = 0;
  while (line[depth] == ' ') {
    depth++;
  }
  return depth / 2 + 1;
}

Tree *processNodes(char **lines, char *root_name) {
  Tree *root = malloc(sizeof(Tree));
  root->text = strdup(root_name);

  Tree *currentNode = root;
  int currentDepth = 0;

  for (int i = 0; lines[i] != NULL; i++) {
    int depth = getDepthFromIndentation(lines[i]);
    char *line = lines[i] + (depth - 1) * 2;
    if (depth > currentDepth) {
      add_child(currentNode, line);
      currentNode = currentNode->children[currentNode->children_count - 1];
      currentDepth = depth;
    } else if (depth < currentDepth) {
      for (int j = 0; j < currentDepth - depth + 1; j++) {
        currentNode = currentNode->parent;
      }
      add_child(currentNode, line);
      currentNode = currentNode->children[currentNode->children_count - 1];
      currentDepth = depth;
    } else {
      currentNode = currentNode->parent;
      add_child(currentNode, line);
      currentNode = currentNode->children[currentNode->children_count - 1];
    }
  }

  return root;
}

void free_lines(char **lines) {
  for (int i = 0; lines[i] != NULL; i++) {
    free(lines[i]);
  }
  free(lines);
}

void free_tree(Tree *root) {
  if (root == NULL) {
    return;
  }

  for (int i = 0; i < root->children_count; i++) {
    free_tree(root->children[i]);
  }

  free(root->text);
  free(root);
}
