#include <math.h>
#include <stdlib.h>

#include <draw.h>

double get_text_width(const char *text) {
  cairo_t *cr;
  cairo_surface_t *surface;

  surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
  cr = cairo_create(surface);

  cairo_text_extents_t extents;
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);

  cairo_set_font_size(cr, font_size);
  cairo_text_extents(cr, text, &extents);

  return extents.width;
}

void draw_text_rect(cairo_t *cr, double x, double y, double width) {
  if (x + width + paddingX * 2 > max_width) {
    max_width = (int)(x + width + paddingX * 2);
  }

  if (y + font_size + paddingY * 2 > max_height) {
    max_height = (int)(y + font_size + paddingY * 2);
  }

  if (cr == NULL) {
    return;
  }

  cairo_rectangle(cr, x, y, width + paddingX * 2, font_size + paddingY * 2);
  cairo_fill(cr);

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_rectangle(cr, x, y, width + paddingX * 2, font_size + paddingY * 2);
  cairo_stroke(cr);
}

void draw_text(cairo_t *cr, double x, double y, const char *text) {
  if (cr == NULL) {
    return;
  }

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_move_to(cr, x + paddingX, y + paddingY);
  cairo_show_text(cr, text);
}

double draw_node(cairo_t *cr, double x, double y, char *text) {
  double width = get_text_width(text);

  if (cr != NULL) {
    cairo_set_source_rgb(cr, 1, 1, 1);
  }
  draw_text_rect(cr, x, y, width);
  draw_text(cr, x, y + font_size, text);

  return width;
}

void draw_circle(cairo_t *cr, double x, double y, double radius) {
  if (cr == NULL) {
    return;
  }

  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_arc(cr, x, y, radius, 0, 2 * M_PI);
  cairo_stroke(cr);
}

void draw_bezier(cairo_t *cr, double x1, double y1, double x2, double y2) {
  if (cr == NULL) {
    return;
  }

  double xmid = (x1 + x2) / 2;

  cairo_set_source_rgba(cr, 0.6, 0.5, 0.7, 0.5);
  cairo_move_to(cr, x1, y1);
  cairo_curve_to(cr, xmid, y1, xmid, y2, x2, y2);
  cairo_stroke(cr);

  draw_circle(cr, x1, y1, 3);
  draw_circle(cr, x2, y2, 3);
}

double draw_tree(cairo_t *cr, Tree *tree, double x, double y) {
  double initial_x = x;
  double initial_y = y;
  double width = draw_node(cr, initial_x, initial_y, tree->text);
  x += width + paddingX * 2 + marginX;

  if (tree->children_count == 0) {
    return font_size + paddingY * 2 + marginY;
  }

  double height = 0;

  for (unsigned int i = 0; i < tree->children_count; i++) {
    draw_bezier(cr, x - marginX, initial_y + font_size, x,
                y + height + font_size);
    height += draw_tree(cr, tree->children[i], x, y + height);
  }

  draw_node(cr, initial_x, initial_y, tree->text);
  return height;
}
