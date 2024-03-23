#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "draw.h"

cairo_t *init_cairo(Context *ctx, int width, int height) {
  int format = CAIRO_FORMAT_ARGB32;
  cairo_surface_t *surface = cairo_image_surface_create(format, width, height);
  cairo_t *cr = cairo_create(surface);

  cairo_select_font_face(cr, ctx->font, ctx->slant, ctx->weight);
  cairo_set_font_size(cr, ctx->font_size);
  return cr;
}

void draw_background(Context *ctx, cairo_t *cr) {
  cairo_set_source_rgb(cr, 0.9, 0.9, 0.9);
  cairo_rectangle(cr, 0, 0, ctx->max_width, ctx->max_height);
  cairo_fill(cr);
}

void draw_text_rect(Context *ctx, cairo_t *cr, Vec2 pos, Vec2 size) {
  cairo_set_line_width(cr, 2);
  ctx->max_width = fmax(ctx->max_width, pos.x + size.x + ctx->pad.x * 2);

  ctx->max_height = fmax(ctx->max_height, pos.y + size.y + ctx->pad.y * 2);

  {
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_rectangle(cr, pos.x, pos.y, size.x, size.y);
    cairo_fill(cr);
  }

  {
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_rectangle(cr, pos.x, pos.y, size.x, size.y);
    cairo_stroke(cr);
  }
}

char **split_text(const char *text, char delim, int *n) {
  int count = 1;
  for (int i = 0; text[i] != '\0'; i++) {
    if (text[i] == delim) {
      count++;
    }
  }

  char **lines = malloc(sizeof(char *) * count);
  int j = 0;
  int start = 0;
  for (int i = 0; text[i] != '\0'; i++) {
    if (text[i] == delim) {
      int len = i - start;
      lines[j] = malloc(len + 1);
      strncpy(lines[j], text + start, len);
      lines[j][len] = '\0';
      start = i + 1;
      j++;
    }
  }

  int len = strlen(text) - start;
  lines[j] = malloc(len + 1);
  strncpy(lines[j], text + start, len);
  lines[j][len] = '\0';

  *n = count;
  return lines;
}

void draw_text(Context *ctx, cairo_t *cr, Vec2 pos, const char *text) {
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, pos.x + ctx->pad.x, pos.y + ctx->pad.y);
  cairo_show_text(cr, text);
}

Vec2 draw_node(Context *ctx, cairo_t *cr, Vec2 pos, char *text) {
  Vec2 rectSize = {0, 0};
  int n = 0;
  char **lines = split_text(text, '|', &n);

  cairo_set_source_rgb(cr, 1, 1, 1);

  for (int i = 0; i < n; i++) {
    char *line = lines[i];

    cairo_text_extents_t extents;
    cairo_text_extents(cr, line, &extents);

    rectSize.x = fmax(rectSize.x, extents.width + ctx->pad.x * 2);
    rectSize.y += ctx->font_size + ctx->pad.y;

  }
  rectSize.y += ctx->pad.y;

  draw_text_rect(ctx, cr, pos, rectSize);

  for (int i = 0; i < n; i++) {
    char *line = lines[i];
    Vec2 textPos = {pos.x,
                    pos.y + ctx->font_size + (ctx->font_size + ctx->pad.y) * i};

    draw_text(ctx, cr, textPos, line);
  }

  return rectSize;
}

void draw_circle(cairo_t *cr, Vec2 pos, double radius) {
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, pos.x, pos.y, radius, 0, 2 * M_PI);
  cairo_fill(cr);
}

void draw_bezier(cairo_t *cr, Vec2 p1, Vec2 p2) {
  double xmid = (p1.x + p2.x) / 2;
  cairo_set_source_rgba(cr, 0.6, 0.5, 0.7, 0.5);
  cairo_move_to(cr, p1.x, p1.y);
  cairo_curve_to(cr, xmid, p1.y, xmid, p2.y, p2.x, p2.y);
  cairo_stroke(cr);

  draw_circle(cr, p1, 3);
  draw_circle(cr, p2, 3);
}

double draw_tree(Context *ctx, cairo_t *cr, Tree *tree, Vec2 pos) {
  Vec2 initial = {pos.x, pos.y};
  double width = draw_node(ctx, cr, initial, tree->text);
  pos.x += width + ctx->pad.x * 2 + ctx->gap.x;

  if (tree->children_count == 0) {
    return ctx->font_size + ctx->pad.y * 2 + ctx->gap.y;
  }

  double height = 0;

  for (unsigned int i = 0; i < tree->children_count; i++) {
    Vec2 p1 = {pos.x - ctx->gap.x, initial.y + ctx->font_size};
    Vec2 p2 = {pos.x, pos.y + height + ctx->font_size};
    draw_bezier(cr, p1, p2);

    Vec2 treePos = {pos.x, pos.y + height};
    height += draw_tree(ctx, cr, tree->children[i], treePos);
  }

  draw_node(ctx, cr, initial, tree->text);
  return height;
}
