#ifndef MUPDF_FITZ_OUTPUT_PCL_H
#define MUPDF_FITZ_OUTPUT_PCL_H

#include "system.h"
#include "context.h"
#include "output.h"
#include "pixmap.h"
#include "bitmap.h"

/*
	PCL output
*/
typedef struct fz_pcl_options_s fz_pcl_options;

struct fz_pcl_options_s
{
	/* Features of a particular printer */
	int features;
	const char *odd_page_init;
	const char *even_page_init;

	/* Options for this job */
	int tumble;
	int duplex_set;
	int duplex;
	int paper_size;
	int manual_feed_set;
	int manual_feed;
	int media_position_set;
	int media_position;
	int orientation;

	/* Updated as we move through the job */
	int page_count;
};

/*
	 fz_pcl_preset: Retrieve a set of fz_pcl_options suitable for a given
	 preset.

	 opts: pointer to options structure to populate.

	 preset: Preset to fetch. Currently defined presets include:
		ljet4	HP DeskJet
		dj500	HP DeskJet 500
		fs600	Kyocera FS-600
		lj	HP LaserJet, HP LaserJet Plus
		lj2	HP LaserJet IIp, HP LaserJet IId
		lj3	HP LaserJet III
		lj3d	HP LaserJet IIId
		lj4	HP LaserJet 4
		lj4pl	HP LaserJet 4 PL
		lj4d	HP LaserJet 4d
		lp2563b	HP 2563B line printer
		oce9050	Oce 9050 Line printer

	Throws exception on unknown preset.
*/
void fz_pcl_preset(fz_context *ctx, fz_pcl_options *opts, const char *preset);

/*
	fz_pcl_option: Set a given PCL option to a given value in the supplied
	options structure.

	opts: The option structure to modify,

	option: The option to change.

	val: The value that the option should be set to. Acceptable ranges of
	values depend on the option in question.

	Throws an exception on attempt to set an unknown option, or an illegal
	value.

	Currently defined options/values are as follows:

		spacing,0		No vertical spacing capability
		spacing,1		PCL 3 spacing (<ESC>*p+<n>Y)
		spacing,2		PCL 4 spacing (<ESC>*b<n>Y)
		spacing,3		PCL 5 spacing (<ESC>*b<n>Y and clear seed row)
		mode2,0 or 1		Disable/Enable mode 2 graphics compression
		mode3,0 or 1		Disable/Enable mode 3 graphics compression
		mode3,0 or 1		Disable/Enable mode 3 graphics compression
		eog_reset,0 or 1	End of graphics (<ESC>*rB) resets all parameters
		has_duplex,0 or 1	Duplex supported (<ESC>&l<duplex>S)
		has_papersize,0 or 1	Papersize setting supported (<ESC>&l<sizecode>A)
		has_copies,0 or 1	Number of copies supported (<ESC>&l<copies>X)
		is_ljet4pjl,0 or 1	Disable/Enable HP 4PJL model-specific output
		is_oce9050,0 or 1	Disable/Enable Oce 9050 model-specific output
*/
void fz_pcl_option(fz_context *ctx, fz_pcl_options *opts, const char *option, int val);

typedef struct fz_mono_pcl_output_context_s fz_mono_pcl_output_context;

fz_mono_pcl_output_context *fz_write_mono_pcl_header(fz_context *ctx, fz_output *out, int w, int h, int xres, int yres, int pagenum, const fz_pcl_options *options);

void fz_write_mono_pcl_band(fz_context *ctx, fz_output *out, fz_mono_pcl_output_context *poc, const fz_bitmap *bitmap);

void fz_write_mono_pcl_trailer(fz_context *ctx, fz_output *out, fz_mono_pcl_output_context *pcoc);

void fz_write_bitmap_as_pcl(fz_context *ctx, fz_output *out, const fz_bitmap *bitmap, const fz_pcl_options *pcl);

void fz_save_bitmap_as_pcl(fz_context *ctx, fz_bitmap *bitmap, char *filename, int append, const fz_pcl_options *pcl);


typedef struct fz_color_pcl_output_context_s fz_color_pcl_output_context;

fz_color_pcl_output_context *fz_write_color_pcl_header(fz_context *ctx, fz_output *out, int w, int h, int n, int xres, int yres, int pagenum, const fz_pcl_options *options);

void fz_write_color_pcl_band(fz_context *ctx, fz_output *out, fz_color_pcl_output_context *poc, int w, int h, int n, int band, int bandheight, unsigned char *samples);

void fz_write_color_pcl_trailer(fz_context *ctx, fz_output *out, fz_color_pcl_output_context *pcoc);

void fz_write_pixmap_as_pcl(fz_context *ctx, fz_output *out, const fz_pixmap *pixmap, const fz_pcl_options *pcl);

void fz_save_pixmap_as_pcl(fz_context *ctx, fz_pixmap *pixmap, char *filename, int append, const fz_pcl_options *pcl);

#endif
