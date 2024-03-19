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

