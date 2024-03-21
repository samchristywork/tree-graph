#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command_line.h"
#include "draw.h"
#include "tree.h"

int max(int a, int b) {
  if (a > b) {
    return a;
  }
  return b;
}

int main(int argc, char *argv[]) {
  add_arg('f', "font-size", "Set the size of the font (default 12).");
  add_arg('m', "margin", "Set the margin around the tree (default 10,10).");
  add_arg('p', "pad", "Set the padding around the text (default 5,5).");
  add_arg('g', "gap", "Set the gap between nodes (default 40,5).");
  add_arg('i', "infile", "Set the filename of the output image (default stdin).");
  add_arg('o', "outfile", "Set the filename of the output image.");

  parse_opts(argc, argv);

  Context ctx= {
    .font_size = 12,
    .margin = {10, 10},
    .pad = {5, 5},
    .gap = {40, 5},
    .max_width = 0,
    .max_height = 0,
    .filename = "tree.png"
  };

  char **lines = readNodes();
  Tree *root = processNodes(lines);

  // First pass to calculate the width and height of the tree
  Vec2 pos = {0, 0};
  draw_tree(&ctx, NULL, root, pos);
  ctx.max_width += ctx.margin.x*2;
  ctx.max_height += ctx.margin.y*2;

  // Read the extra text and recalculate the height
  int length = 0;
  char **lines2 = readExtra(&length);

  int extraHeight = (length + 2) * ctx.font_size + ctx.pad.y * 4;
  ctx.max_height = max(extraHeight, ctx.max_height);

  // Second pass to draw the tree
  cairo_t *cr = init_cairo(&ctx, ctx.max_width, ctx.max_height);
  draw_background(&ctx, cr);
  Vec2 treePos = {ctx.margin.x, ctx.margin.y};
  draw_tree(&ctx, cr, root, treePos);

  // Draw the extra text
  for (int i = length - 1; i >= 0; i--) {
    Vec2 textPos = {0, ctx.max_height - (length - i) * ctx.font_size};
    draw_text(&ctx, cr, textPos, lines2[i]);
  }

  // Write to file
  cairo_surface_t *surface = cairo_get_target(cr);
  cairo_status_t r = cairo_surface_write_to_png(surface, ctx.filename);
  if (r != CAIRO_STATUS_SUCCESS) {
    printf("Error: could not write to file\n");
    return 1;
  }

  // Clean up
  cairo_destroy(cr);
  cairo_surface_destroy(surface);
  free_tree(root);
  free_lines(lines);
  free_lines(lines2);
}
