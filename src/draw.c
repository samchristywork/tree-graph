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
