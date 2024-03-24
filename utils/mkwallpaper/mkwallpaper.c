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
 * (c)2015 - 2024 Michael Amadio. Gold Coast QLD, Australia 01micko@gmail.com
 */
 
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

#include <cairo-svg.h>
#include <pango/pangocairo.h>

#define PROG "mkwallpaper"
#define THIS_VERSION "0.12"

void usage(){
	printf("%s-%s\n\n", PROG , THIS_VERSION);
	printf("\tGenerate SVG or PNG Linux wallpapers. SVG is default.\n\n");
	printf("Usage :\n");
	printf("%s [-l,-n,-f,-p,-s,-x,-y,-r,-d,-j,-k,-z,-o,-a,-b,-h,-e,-w]\n", PROG);
	printf("\t-n [string] image file name\n");
	printf("\t-l [string] label for an image, up to 36 chars\n");
	printf("\t-f [string] a TTF font family\n");
	printf("\t-i [\"0|1|2\"] integer from 0 - 2 to align text left, centre or right\n");
	printf("\t-p [\"png|svg\"] \"png\" or \"svg\" format\n");
	printf("\t-x [int] width of image in pixels\n");
	printf("\t-y [int] height of image in pixels\n");
	printf("\t-s [int] font size in pixels\n");
	printf("\t-k \"yes\" : embossed effect on font\n");
	printf("\t-g \"yes\" : background gradient (default). \"no\" to turn off\n");
	printf("\t-e [string] : '/path/to/icon.png x y' - embed a png image at position\n\t(optional)\n");
	printf("\t-w [string] : '/path/to/background.png' - embed a png wall at pos 0,0\n\t(optional)\n");
	printf("\t Requires original dimensions of incoming png image for '-x' and '-y'\n");
	printf("\t-j [x y] : font position in px\n");
	printf("\t-z [\"float float float\"] floating point RGB, quoted,\n"
					"\tspace delimited values for colour\n"
					"\t(mandatory!) eg: -z \"0.1 0.2 0.3\"\n");
	printf("\t-o [float] offset: floating point value from 0.0 to 1.0\n"
								"\tfor the gradient offset\n");
	printf("\t-a [int] angle: integer value from 0 to 20 for the gradient angle\n");
	printf("\t-d [/path/to/directory] destination directory: (default: $HOME)\n");
	printf("\t-b [string] font-style: accepted values are \"n\" (normal) [the default],\n"
					"\t\"b\" (bold), \"i\" (italic), \"o\" (bold-italic).\n");
	printf("\t-u \"yes\" : use transparent background (not good for wallpapers)\n");
	printf("\t-h : show this help and exit.\n");
	printf("\t-v : show version and exit.\n");
	exit (EXIT_SUCCESS);
}

struct { 
	/* allows an icon */
	cairo_surface_t *image;
	/* allows a background - png only */
	cairo_surface_t *background;
}glob;

static const char *get_user_out_file(char *destination){
	static char out_file[PATH_MAX];
	if (destination != NULL) {
		snprintf(out_file, sizeof(out_file), "%s", destination);
	} else {
		fprintf(stderr, "Failed to recognise directory\n");
		exit (EXIT_FAILURE);
	}
	mkdir(out_file, 0755);
	if (access(out_file, W_OK) == -1) {
		fprintf(stderr, "Failed to access directory %s\n", out_file);
		exit (EXIT_FAILURE);
	}
	return out_file;
}

static void paint_img (const char *label,
						const char *name,
						const char *font,
						const char *form,
						const int wdth,
						int hght,
						const char *fp_color,
						int f_size,
						double offset,
						int angle,
						const char *kfont,
						const char *grad,
						char *jposi,
						int fontcol,
						char *dest,
						int align,
						char *style,
						char *eicon,
						char *wall,
						char *trans) {
	
	char icon[PATH_MAX];
	char icon_pre[PATH_MAX];
	char posx[8];
	char posy[8];
	int icon_x, icon_y;
	/* icon */
	if (eicon != NULL) {
		int icon_res = sscanf(eicon, "%s %s %s", icon_pre, posx, posy);
		if (icon_res < 3) {
			fprintf(stderr,"ERROR: path, x and y positions are required\n");
			exit (EXIT_FAILURE);
		}
		if (icon_res > 3) {
			fprintf(stderr,"ERROR: path spaces are not allowed\n");
			exit (EXIT_FAILURE);
		}
		snprintf(icon, sizeof(icon), "%s", icon_pre);
		if (access(icon, R_OK) == -1) {
			fprintf(stderr, "Failed to access icon %s\n", icon);
			exit (EXIT_FAILURE);
		}
		icon_x = atoi(posx);
		icon_y = atoi(posy);
	}
	/* background */
	if (wall != NULL) {
		if (access(wall, R_OK) == -1) {
			fprintf(stderr, "Failed to access image %s\n", wall);
			exit (EXIT_FAILURE);
		}
	}
	
	char destimg[PATH_MAX];
	
	if ((align < 0) || (align > 2))
		align = 0; /* counter silly input */
	if (!fp_color) exit (EXIT_FAILURE);

	int msg_len = strlen(label);
	if (msg_len > 36) {
		fprintf(stderr,"\"%s\" is too long!\n", label);
		exit (EXIT_FAILURE);
	}
	
	/* error check angle/offset */
	if ((angle < 0) || (angle > 20)) {
		fprintf(stderr, "%d is out of range. Must be 0 - 20 inclusive\n", angle);
		exit(EXIT_FAILURE);
	}
	if ((offset < 0) || (offset > 1)) {
		fprintf(stderr, "%f is out of range. Must be 0.00 - 1.00 inclusive\n", offset);
		exit(EXIT_FAILURE);
	}
	
	/*font style*/
	int boldness; int styleness;
	if (strncmp(style, "b", 1) == 0) { boldness = PANGO_WEIGHT_BOLD; styleness = PANGO_STYLE_NORMAL;}
	else if (strncmp(style, "i", 1) == 0) { boldness = PANGO_WEIGHT_NORMAL; styleness = PANGO_STYLE_OBLIQUE;}
	else if (strncmp(style, "o", 1) == 0) { boldness = PANGO_WEIGHT_BOLD; styleness = PANGO_STYLE_OBLIQUE;}
	else { boldness = PANGO_WEIGHT_NORMAL; styleness = PANGO_STYLE_NORMAL;} /*catch garbage and default to 'normal'*/

	float r, g , b;
	char red[8];
	char green[8];
	char blue[8];
	int len = strlen(fp_color);
	if (len > 27 ) {
		fprintf(stderr,"ERROR: colour argument too long\n");
		exit (EXIT_FAILURE);
	}
	int result = sscanf(fp_color, "%s %s %s", red, green, blue);
	if (result < 3) {
		fprintf(stderr,"ERROR: less than 3 colour aguments!\n");
		exit (EXIT_FAILURE);
	}
	r = atof(red);
	g = atof(green);
	b = atof(blue);
	if ((r > 1.0) || (g > 1.0) || (b > 1.0) || 
		(r < 0.0) || (g < 0.0) || (b <0.0))  {
		fprintf(stderr, "Color values are out of range\n");
		exit (EXIT_FAILURE);
	}
	/* gradient */
	float r1, g1, b1, r2, g2, b2;
	if (strncmp(grad, "yes", 3) == 0) {
		r1 = r; g1 = g; b1 = b;
		if ((r > 0.701) || (g > 0.701) || (b > 0.701)) {
			r2 = r + 0.3;
			g2 = g + 0.3;
			b2 = b + 0.3;
		} else {
			r2 = r - 0.3;
			g2 = g - 0.3;
			b2 = b - 0.3;		
		}
	}else {
		r1 = r; g1 = g; b1 = b; r2 = r; g2 = g; b2 = b;
	}
	
	/* font color */
	float rf;
	if ((r > 0.5) || (g > 0.5) || (b > 0.5))
		rf = 0.1;
	else
		rf = 0.9;
	
	/* offset font for effect option */	
	float or, og, ob;
	int fc = 0;
	if (strncmp(kfont, "yes", 3) == 0) {
		or = (r1 + r2) / 2;
		og = (g1 + g2) /2 ;
		ob = (b1 + b2) / 2;
		fc = 1;
	}
	
	cairo_surface_t *cs;

	if (strcmp(form, "svg") == 0) {
		snprintf(destimg, sizeof(destimg), "%s/%s.svg", get_user_out_file(dest), name);
		cs = cairo_svg_surface_create(destimg, wdth, hght);

	} else {
		cs = cairo_image_surface_create
							(CAIRO_FORMAT_ARGB32, wdth, hght);
		snprintf(destimg, sizeof(destimg), "%s/%s.png", get_user_out_file(dest), name);
	}
	cairo_t *c;
	c = cairo_create(cs);
	
	/* background and position */
	if (wall != NULL) {
		cairo_rectangle(c, 0.0, 0.0, wdth, hght);
		glob.background = cairo_image_surface_create_from_png(wall);
		cairo_set_source_surface(c, glob.background, 0, 0);
		cairo_paint(c);
	} else if (strncmp(trans, "yes", 3) == 0) {
		cairo_set_source_rgba(c, 0, 0, 0, 0);
		cairo_rectangle(c, 0.0, 0.0, wdth, hght);
		cairo_fill(c);
	} else {
		cairo_rectangle(c, 0.0, 0.0, wdth, hght);
		cairo_pattern_t *linear = cairo_pattern_create_linear(angle * wdth / 20, 0, wdth / 2, hght);
		cairo_pattern_add_color_stop_rgb(linear, 0.1, r1, g1, b1);
		cairo_pattern_add_color_stop_rgb(linear, offset, r2, g2, b2);
		cairo_pattern_add_color_stop_rgb(linear, 0.9, r1, g1, b1);
		cairo_set_source(c, linear);
		cairo_fill(c);
	}
	
	/* font */
	PangoLayout *layout;
	PangoFontDescription *font_description;
	
	font_description = pango_font_description_new ();
	pango_font_description_set_family (font_description, font);
	pango_font_description_set_style (font_description, styleness ); /*PANGO_STYLE_NORMAL = 0, PANGO_STYLE_OBLIQUE = 1*/
	pango_font_description_set_weight (font_description, boldness); /*PANGO_WEIGHT_NORMAL = 400, PANGO_WEIGHT_BOLD = 700*/
	pango_font_description_set_absolute_size (font_description, f_size * PANGO_SCALE);
	layout = pango_cairo_create_layout (c);
	pango_layout_set_font_description (layout, font_description);
	pango_layout_set_width (layout, 9 * wdth / 20 * PANGO_SCALE);
	pango_layout_set_alignment (layout, align); /*PANGO_ALIGN_LEFT, PANGO_ALIGN_CENTER, PANGO_ALIGN_RIGHT)*/
	pango_layout_set_wrap (layout, PANGO_WRAP_WORD);
	pango_layout_set_text (layout, label, -1);
	
	/* icon and position */
	if (eicon != NULL) {
		glob.image = cairo_image_surface_create_from_png(icon);
		cairo_set_source_surface(c, glob.image, icon_x, icon_y);
		cairo_paint(c);
	}
	/* position of text */
	int xposi, yposi;
	if (jposi != NULL) {	
		char prex[8];
		char prey[8];
		int font_pos = sscanf(jposi, "%s %s", prex, prey);
		if (font_pos < 2) {
			fprintf(stderr,"ERROR: x and y positions are required\n");
			exit (EXIT_FAILURE);
		}
		if (font_pos > 2) {
			fprintf(stderr,"ERROR: too many args\n");
			exit (EXIT_FAILURE);
		}
		xposi = atoi(prex);
		yposi = atoi(prey);
	} else { /* fallback */
		xposi = wdth / 2;
		yposi = 3 * hght / 7;
	}
	
	
	//float xposi = wdth / 2;
	//float yposi = 4 * hght / 7;
	//if ((strcmp(jposi, "tl") == 0) || (strcmp(jposi, "bl") == 0) || (strcmp(jposi, "cl") == 0))
		//xposi = wdth / 6;
	//if ((strcmp(jposi, "tl") == 0) || (strcmp(jposi, "tr") == 0))
		//yposi = hght / 6;
	//if ((strcmp(jposi, "cl") == 0) || (strcmp(jposi, "cr") == 0))
		//yposi = 3 * hght / 7;
	
	cairo_move_to(c, xposi , 1 * yposi);
	if (fontcol == 1)
		cairo_set_source_rgba(c, rf, rf, rf, 0.6);
	else
		cairo_set_source_rgba(c, rf, rf, rf, 0.55);
		
	pango_cairo_show_layout (c, layout);
	
	if (fc == 1) {
		cairo_move_to(c, xposi - 1.5 , (1 * yposi) - 1.2);
		cairo_set_source_rgba(c, or, og, ob, 0.65);
		pango_cairo_show_layout (c, layout);
	}
	cairo_status_t res = cairo_surface_status(cs);
	const char *ret;
	ret = cairo_status_to_string (res);
	if (res != CAIRO_STATUS_SUCCESS) {
		g_object_unref (layout);
		pango_font_description_free (font_description);
		cairo_surface_destroy(cs);
		cairo_destroy(c);
		fprintf(stderr, "Cairo : %s\n", ret);
		exit (EXIT_FAILURE);
	}
	/* cleanup */
	g_object_unref (layout);
	pango_font_description_free (font_description);
	if (strcmp(form, "png") == 0) {
		cairo_surface_write_to_png (cs, destimg);
	}
	if (eicon != NULL)
		cairo_surface_destroy(glob.image);
	if (wall != NULL)
		cairo_surface_destroy(glob.background);
	cairo_surface_destroy(cs);
	cairo_destroy(c);
	fprintf(stdout, "image saved to %s\n", destimg);
}

int main(int argc, char **argv) {
	if (argc < 2) {
		usage();
		return 0;
	}
	int hflag = 0;
	int vflag = 0;
	char *lvalue = "hello wallpaper!"; /* the cli string that appears in image */
	char *nvalue = "foo"; /* image name */
	char *fvalue = "Sans"; /* font */
	char *pvalue = "svg"; /* default */
	char *zvalue = NULL; /* fp colour */
	char *evalue = NULL; /* embedded icon */
	char *wvalue = NULL; /* embedded background */
	char *kvalue = "no";
	char *grvalue = "yes";
	char *jvalue = NULL;
	int gvalue = 0;
	char *dvalue = getenv("HOME");
	double ovalue = 0.65;
	int avalue = 10;
	int width = 200; int height = 60;
	int font_size = 20;
	unsigned int ivalue = 0;
	char *bvalue = "n";
	char *uvalue = "no";
	int c;
	while ((c = getopt (argc, argv, "l:n:f:p:x:y:d:z:o:a:i:k:g:j:s:b:e:w:u:hv")) != -1) {
		switch (c)
		{
			case 'l':
				lvalue = optarg;
				break;
			case 'n':
				nvalue = optarg;
				break;
			case 'f':
				fvalue = optarg;
				break;
			case 'p':
				pvalue = optarg;
				break;
			case 'x':
				width = atoi(optarg);
				break;
			case 'y':
				height = atoi(optarg);
				break;
			case 'z':
				zvalue = optarg;
				break;
			case 'o':
				ovalue = atof(optarg);
				break;
			case 'a':
				avalue = atoi(optarg);
				break;
			case 'i':
				ivalue = atoi(optarg);
				break;
			case 'd':
				dvalue = optarg;
				break;
			case 'k':
				kvalue = optarg;
				break;
			case 'g':
				grvalue = optarg;
				break;
			case 'j':
				jvalue = optarg;
				break;
			case 's':
				font_size = atoi(optarg);
				break;
			case 'b':
				bvalue = optarg;
				break;
			case 'e':
				evalue = optarg;
				break;
			case 'w':
				wvalue = optarg;
				gvalue = 1;
				break;
			case 'u':
				uvalue = optarg;
				break;
			case 'h':
				hflag = 1;
				if (hflag == 1) usage();
				break;
			case 'v':
				vflag = 1;
				if (vflag == 1) fprintf(stdout, "%s\n", THIS_VERSION);
				return 0;
			default:
				usage();
		}
	}
	paint_img(lvalue, nvalue, fvalue, pvalue,
					width, height, zvalue, font_size, ovalue, avalue,
					 kvalue, grvalue, jvalue, gvalue, dvalue, ivalue, bvalue, evalue, wvalue, uvalue);
	return 0;
}

