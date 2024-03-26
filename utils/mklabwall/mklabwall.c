/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */
#include <stdio.h>
#include <unistd.h>

#include <cairo-svg.h>
#include <pango/pangocairo.h>

#define THIS_VERSION "0.0.1"

void usage() {
	printf("mklabwall\n"
		"\t-f font[str]\n"
		"\t-s fontsize[int]\n"
		"\t-v font_vertical_position[int]\n"
		"\t-h font_horizontal_position[int]\n"
		"\t-n filename[str]\n"
		"\t-z fancy xD [no arg] (an easter egg!)\n"
		"\t-b background color[\"float float float\"]\n"
		"\t-p save as png [no arg - default is svg]\n"
		"\t-l logo only [no arg]\n"
		"\t-r resolution [\"int int\"]\n\n"
		"at least 1 option is required\n"
		"file is saved to \"HOME/labwcwall\" or \"HOME/myfile\" with -n [arg] option\n"
		"Version %s\n", THIS_VERSION);
	exit (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		usage();
		return 0;
	}
	char *res = NULL;
	char *font = "Sans";
	char *bg = NULL;
	int f_size = 116;
	char *name = "labwcwall";
	int fnt_vert = 32;
	int fnt_hor = 290;
	int zflag = 0;
	int pflag = 0;
	int lflag = 0;
	int d;
	while ((d = getopt (argc, argv, "r:f:b:s:n:v:h:zpl")) != -1) {
		switch (d)
		{
			case 'r':
				res = optarg;
				break;
			case 'f':
				font = optarg;
				break;
			case 'b':
				bg = optarg;
				break;
			case 's':
				f_size = atoi(optarg);
				break;
			case 'n':
				name = optarg;
				break;
			case 'v':
				fnt_vert = atoi(optarg);
				break;
			case 'h':
				fnt_hor = atoi(optarg);
				break;
			case 'z':
				zflag = 1;
				break;
			case 'p':
				pflag = 1;
				break;
			case 'l':
				lflag = 1;
				break;
			default:
				usage();
			
		}
	}
	
	/* resolution */
	
	float w, h;
	if (!res) {
		w = 1366; 
		h = 768;
	} else {
		char pre_w[8];
		char pre_h[8];
		int res_result = sscanf(res, "%s %s", pre_w, pre_h);
		if (res_result < 2) {
			fprintf(stderr,"ERROR: less than 2 x y resolution aguments!\n");
			exit (EXIT_FAILURE);
		}
		w = atof(pre_w);
		h = atof(pre_h);
	}
	/* /resolution */
	
	/* logo and text dims*/
	int img_wdth = 600;
	int img_hght = 184;
	int xh = 0;
	if (zflag == 1) {
		xh = 52;
		img_hght = img_hght + xh;
	}
	if (lflag == 1) {
		img_wdth = 250;
	}
	/* /logo and text dims*/
	
	/* file save name/fmt */
	char destimg[PATH_MAX];
	char *dest = getenv("HOME");
	cairo_surface_t *cs;
	if (pflag == 0) {
		snprintf(destimg, sizeof(destimg), "%s/%s.svg", dest, name);
		cs = cairo_svg_surface_create(destimg, w, h);
	} else {
		cs = cairo_image_surface_create
							(CAIRO_FORMAT_ARGB32, w, h);
		snprintf(destimg, sizeof(destimg), "%s/%s.png", dest, name);
	}
	/* /file save name/fmt */
	
	cairo_t *c;
	c = cairo_create(cs);
	
	/* icon and position */
	if (!bg) {
		cairo_set_source_rgb (c, 0.05, 0.06, 0.09); /* default #0d1117 */
	} else {
		float r, g , b;
		char red[8];
		char green[8];
		char blue[8];
		int result = sscanf(bg, "%s %s %s", red, green, blue);
		if (result < 3) {
			fprintf(stderr,"ERROR: less than 3 colour aguments!\n");
			exit (EXIT_FAILURE);
		}
		r = atof(red);
		g = atof(green);
		b = atof(blue);
		cairo_set_source_rgb (c, r, g, b);
	}
	cairo_rectangle(c, 0.0, 0.0, w, h);
	cairo_fill(c);
	
	/** The image and text size are hard coded to 600 * (184 + xh)
	 * and the image only is hard coded to 250 * (184 + xh)
	 *  so just need to scale that to suit the resolution with a scale
	 *  factor. We'll use 1366 x 768 as 1:1, well 1366 anyway as to
	 *  account for different aspect ratios.
	 *  Then work out where to place the image and text in the centre
	 *  of our wallpaper
	 */
	 
	float sf = w / 1366;
	printf("scale factor: %f\n", sf);
	float x_pos = w / 2;
	float y_pos = h / 2;
	float new_wdth = (float)img_wdth * sf;
	float new_hght = (float)img_hght * sf;
	x_pos = x_pos - (new_wdth / 2);
	y_pos = y_pos - (new_hght / 2);
	xh = (float)xh * sf;
	
	/* fancy */ 
	if (zflag == 1) {
		cairo_set_source_rgba (c, 0.81, 0.18, 0.56, 0.3);
		cairo_move_to (c, x_pos + (102 * sf), y_pos + (24 * sf));
		cairo_line_to (c, x_pos + (26 * sf), y_pos + (45 * sf));
		cairo_curve_to (c, x_pos + (19 * sf), y_pos + (46 * sf), x_pos + (19 * sf), y_pos + (51 * sf), x_pos + (20 * sf), y_pos + (57 * sf));
		cairo_line_to (c, x_pos + (31 * sf), y_pos + (146 * sf));
		cairo_line_to (c, x_pos + (112 * sf),y_pos +  (114 * sf));
		cairo_line_to (c, x_pos + (112 * sf), y_pos + (28 * sf));
		cairo_curve_to (c, x_pos + (111 * sf), y_pos + (24 * sf), x_pos + (107 * sf), y_pos + (23 * sf), x_pos + (102 * sf), y_pos + (24 * sf));
		cairo_close_path (c);
		cairo_fill (c);
		
		cairo_set_source_rgba (c, 0.94, 0.84, 0.05, 0.3);
		cairo_move_to (c, x_pos + (131 * sf), y_pos + (24 * sf));
		cairo_line_to (c, x_pos + (207 * sf), y_pos + (45 * sf));
		cairo_curve_to (c, x_pos + (214 * sf), y_pos + (46 * sf), x_pos + (214 * sf), y_pos + (51 * sf), x_pos + (213 * sf), y_pos + (57 * sf));
		cairo_line_to (c, x_pos + (201 * sf), y_pos + (146 * sf));
		cairo_line_to (c, x_pos + (121 * sf), y_pos + (114 * sf));
		cairo_line_to (c, x_pos + (121 * sf), y_pos + (28 * sf));
		cairo_curve_to (c, x_pos + (122 * sf), y_pos + (24 * sf), x_pos + (126 * sf), y_pos + (23 * sf), x_pos + (131 * sf), y_pos + (24 * sf));
		cairo_close_path (c);
		cairo_fill (c);
	} /* /fancy */
	
	cairo_set_source_rgb (c, 0.94, 0.84, 0.05);
	cairo_set_line_cap (c, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_join (c, CAIRO_LINE_JOIN_ROUND);
	
	cairo_move_to (c, x_pos + (7 * sf), y_pos + (6 * sf) + xh);
	cairo_line_to (c, x_pos + (106 * sf), y_pos + (64 * sf) + xh);
	cairo_line_to (c, x_pos + (106 * sf), y_pos + (178 * sf) + xh);
	cairo_line_to (c, x_pos + (24 * sf), y_pos + (113 * sf) + xh);
	cairo_line_to (c, x_pos + (7 * sf), y_pos + (6 * sf) + xh);
	cairo_close_path (c);
	cairo_fill(c);
	
	cairo_set_line_width (c, 12.0 * sf);
	cairo_move_to (c, x_pos + (7 * sf), y_pos + (6 * sf) + xh);
	cairo_line_to (c, x_pos + (106 * sf), y_pos + (64 * sf) + xh);
	cairo_line_to (c, x_pos + (106 * sf), y_pos + (178 * sf) + xh);
	cairo_line_to (c, x_pos + (24 * sf), y_pos + (113 * sf) + xh);
	cairo_line_to (c, x_pos + (7 * sf), y_pos + (6 * sf) + xh);
	cairo_close_path (c);	
	cairo_stroke (c);
	
	cairo_set_source_rgb (c, 0.81, 0.18, 0.56);
	cairo_move_to (c, x_pos + (226 * sf), y_pos + (6 * sf) + xh);
	cairo_line_to (c, x_pos + (127 * sf), y_pos + (64 * sf) + xh);
	cairo_line_to (c, x_pos + (127 * sf), y_pos + (178 * sf) + xh);
	cairo_line_to (c, x_pos + (209 * sf), y_pos + (113 * sf) + xh);
	cairo_line_to (c, x_pos + (226 * sf), y_pos + (6 * sf) + xh);
	cairo_close_path (c);
	cairo_fill (c);
	
	cairo_move_to (c, x_pos + (226 * sf), y_pos + (6 * sf) + xh);
	cairo_line_to (c, x_pos + (127 * sf), y_pos + (64 * sf) + xh);
	cairo_line_to (c, x_pos + (127 * sf), y_pos + (178 * sf) + xh);
	cairo_line_to (c, x_pos + (209 * sf), y_pos + (113 * sf) + xh);
	cairo_line_to (c, x_pos + (226 * sf), y_pos + (6 * sf) + xh);
	cairo_close_path (c);
	cairo_stroke (c);
	
	if (lflag == 0) {
		/* font */
		float rgb = 0.97; /* font color */
		char *label = "labwc";
		
		PangoLayout *layout;
		PangoFontDescription *font_description;
	
		font_description = pango_font_description_new ();
		pango_font_description_set_family (font_description, font);
		pango_font_description_set_style (font_description, PANGO_STYLE_NORMAL ); /*PANGO_STYLE_NORMAL = 0, PANGO_STYLE_OBLIQUE = 1*/
		pango_font_description_set_weight (font_description, PANGO_WEIGHT_NORMAL); /*PANGO_WEIGHT_NORMAL = 400, PANGO_WEIGHT_BOLD = 700*/
		pango_font_description_set_absolute_size (font_description, f_size * sf * PANGO_SCALE);
		layout = pango_cairo_create_layout (c);
		pango_layout_set_font_description (layout, font_description);
		pango_layout_set_alignment (layout, PANGO_ALIGN_LEFT); /*PANGO_ALIGN_LEFT, PANGO_ALIGN_CENTER, PANGO_ALIGN_RIGHT)*/
		pango_layout_set_wrap (layout, PANGO_WRAP_WORD);
		pango_layout_set_text (layout, label, -1);
		
		/* position of text */
		int xposi = x_pos + (fnt_hor * sf);
		int yposi = y_pos + (fnt_vert * sf) + xh;
		cairo_move_to (c, xposi , 1 * yposi);
		cairo_set_source_rgb (c, rgb, rgb, rgb);
		pango_cairo_show_layout (c, layout);
		pango_font_description_free (font_description);
		g_object_unref (layout);
	}
	cairo_status_t rest = cairo_surface_status(cs);
	const char *ret;
	ret = cairo_status_to_string (rest);
	if (rest != CAIRO_STATUS_SUCCESS) {
		cairo_surface_destroy (cs);
		cairo_destroy (c);
		fprintf (stderr, "Cairo : %s\n", ret);
		exit (EXIT_FAILURE);
	}
	/* cleanup */
	cairo_surface_destroy (cs);
	cairo_destroy (c);
	fprintf (stdout, "image saved to %s\n", destimg);
	
	return 0;
}
