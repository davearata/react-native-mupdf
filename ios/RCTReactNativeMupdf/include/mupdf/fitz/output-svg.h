#ifndef MUPDF_FITZ_OUTPUT_SVG_H
#define MUPDF_FITZ_OUTPUT_SVG_H

#include "system.h"
#include "context.h"
#include "device.h"
#include "output.h"

fz_device *fz_new_svg_device(fz_context *ctx, fz_output *out, float page_width, float page_height);

#endif
