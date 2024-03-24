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

void usage() {
	printf("mklabimg -f font[str] -s fontsize[int] -v font_vertical_position[int]\n"
		"-h font_horizontal_position[int] -n filename[str] -z fancy xD [no arg]\n"
		"-b background color[\"float float float\"]\n\n"
		"at least 1 option is required\n"
		"file is saved to \"HOME/labwcimg\" or \"HOME/myfile\" with -n [arg] option\n");
	exit (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		usage();
		return 0;
	}
	int f_size = 80;
	char *font = "Sans";
	char *bg = NULL;
	char *name = "labwcimg";
	int fnt_vert = 30;
	int fnt_hor = 300;
	int zflag;
	int d;
	while ((d = getopt (argc, argv, "f:b:s:n:v:h:z")) != -1) {
		switch (d)
		{
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
			default:
				usage();
			
		}
	}
	
	int wdth = 600;
	int hght = 184;
	int xh = 0;
	if (zflag == 1) {
		xh = 52;
		hght = hght + xh;
	}
	char destimg[PATH_MAX];
	char *dest = getenv("HOME");
	cairo_surface_t *cs;
	snprintf(destimg, sizeof(destimg), "%s/%s.svg", dest, name);
	cs = cairo_svg_surface_create(destimg, wdth, hght);
	
	cairo_t *c;
	c = cairo_create(cs);
	
	/* labwc icon and position */
	if (!bg) {
		cairo_set_source_rgba (c, 0, 0, 0, 0);
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
	cairo_rectangle(c, 0.0, 0.0, wdth, hght);
	cairo_fill(c);
	
	/* fancy */
	if (zflag == 1) {
		cairo_set_source_rgba (c, 0.81, 0.18, 0.56, 0.3);
		cairo_move_to (c, 102, 24);
		cairo_line_to (c, 26, 45);
		cairo_curve_to (c, 19, 46, 19, 51, 20, 57);
		cairo_line_to (c, 31, 146);
		cairo_line_to (c, 112, 114);
		cairo_line_to (c, 112, 28);
		cairo_curve_to (c, 111, 24, 107, 23, 102, 24);
		cairo_close_path (c);
		cairo_fill (c);
		
		cairo_set_source_rgba (c, 0.94, 0.84, 0.05, 0.3);
		cairo_move_to (c, 131, 24);
		cairo_line_to (c, 207, 45);
		cairo_curve_to (c, 214, 46, 214, 51, 213, 57);
		cairo_line_to (c, 201, 146);
		cairo_line_to (c, 121, 114);
		cairo_line_to (c, 121, 28);
		cairo_curve_to (c, 122, 24, 126, 23, 131, 24);
		cairo_close_path (c);
		cairo_fill (c);
	} /* /fancy */
	
	cairo_set_source_rgb (c, 0.94, 0.84, 0.05);
	cairo_set_line_cap (c, CAIRO_LINE_CAP_ROUND);
	cairo_set_line_join (c, CAIRO_LINE_JOIN_ROUND);
	
	cairo_move_to (c, 7, 6 + xh);
	cairo_line_to (c, 106, 64 + xh);
	cairo_line_to (c, 106, 178 + xh);
	cairo_line_to (c, 24, 113 + xh);
	cairo_line_to (c, 7, 6 + xh);
	cairo_close_path (c);
	cairo_fill(c);
	
	cairo_set_line_width (c, 12.0);
	cairo_move_to (c, 7, 6 + xh);
	cairo_line_to (c, 106, 64 + xh);
	cairo_line_to (c, 106, 178 + xh);
	cairo_line_to (c, 24, 113 + xh);
	cairo_line_to (c, 7, 6 + xh);
	cairo_close_path (c);	
	cairo_stroke (c);
	
	cairo_set_source_rgb (c, 0.81, 0.18, 0.56);
	cairo_move_to (c, 226, 6 + xh);
	cairo_line_to (c, 127, 64 + xh);
	cairo_line_to (c, 127, 178 + xh);
	cairo_line_to (c, 209, 113 + xh);
	cairo_line_to (c, 226, 6 + xh);
	cairo_close_path (c);
	cairo_fill (c);
	
	cairo_move_to (c, 226, 6 + xh);
	cairo_line_to (c, 127, 64 + xh);
	cairo_line_to (c, 127, 178 + xh);
	cairo_line_to (c, 209, 113 + xh);
	cairo_line_to (c, 226, 6 + xh);
	cairo_close_path (c);
	cairo_stroke (c);
	
	/* font */
	float rgb = 0.97; /* font color */
	char *label = "labwc";
	
	PangoLayout *layout;
	PangoFontDescription *font_description;

	font_description = pango_font_description_new ();
	pango_font_description_set_family (font_description, font);
	pango_font_description_set_style (font_description, PANGO_STYLE_NORMAL ); /*PANGO_STYLE_NORMAL = 0, PANGO_STYLE_OBLIQUE = 1*/
	pango_font_description_set_weight (font_description, PANGO_WEIGHT_NORMAL); /*PANGO_WEIGHT_NORMAL = 400, PANGO_WEIGHT_BOLD = 700*/
	pango_font_description_set_absolute_size (font_description, f_size * PANGO_SCALE);
	layout = pango_cairo_create_layout (c);
	pango_layout_set_font_description (layout, font_description);
	pango_layout_set_width (layout, 9 * wdth / 20 * PANGO_SCALE);
	pango_layout_set_alignment (layout, PANGO_ALIGN_LEFT); /*PANGO_ALIGN_LEFT, PANGO_ALIGN_CENTER, PANGO_ALIGN_RIGHT)*/
	pango_layout_set_wrap (layout, PANGO_WRAP_WORD);
	pango_layout_set_text (layout, label, -1);
	
	/* position of text */
	int xposi = fnt_hor;
	int yposi = fnt_vert + xh;
	cairo_move_to (c, xposi , 1 * yposi);
	cairo_set_source_rgb (c, rgb, rgb, rgb);
	pango_cairo_show_layout (c, layout);
	
	cairo_status_t res = cairo_surface_status(cs);
	const char *ret;
	ret = cairo_status_to_string (res);
	if (res != CAIRO_STATUS_SUCCESS) {
		g_object_unref (layout);
		pango_font_description_free (font_description);
		cairo_surface_destroy (cs);
		cairo_destroy (c);
		fprintf (stderr, "Cairo : %s\n", ret);
		exit (EXIT_FAILURE);
	}
	/* cleanup */
	g_object_unref (layout);
	pango_font_description_free (font_description);
	cairo_surface_destroy (cs);
	cairo_destroy (c);
	fprintf (stdout, "image saved to %s\n", destimg);
	
	return 0;
}
