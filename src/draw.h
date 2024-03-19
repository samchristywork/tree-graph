#ifndef DRAW_H
#define DRAW_H

#include <cairo/cairo.h>
#include "tree.h"

typedef struct Vec2 {
  double x;
  double y;
} Vec2;

typedef struct Context {
  double font_size;
  Vec2 margin; // Space around the tree
  Vec2 pad; // Space around the text
  Vec2 gap; // Space between nodes
  int max_width;
  int max_height;
  char *filename;
} Context;

cairo_t *init_cairo(Context *ctx, int width, int height);
void draw_background(Context *ctx, cairo_t *cr);
double draw_tree(Context *ctx, cairo_t *cr, Tree *tree, Vec2 pos);
void draw_text(Context *ctx, cairo_t *cr, Vec2 pos, const char *text);

#endif
