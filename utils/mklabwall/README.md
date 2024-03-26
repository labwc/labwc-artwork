# mklabwall

This is a CLI program to generate wallpapers specifically for `labwc`.
It will generate by default `svg` wallpapers with `#0d1117` background
color with the `labwc` logo and text at `1366 x 768` resolution. Colors,
font family and dimensions can be overridden with the options. `labwc`
icons and text are hard coded.

### build

```
make

# optional as root

make install
```

The binary installs to `/usr/local/bin`. Edit the `Makefile` if you want
to install to an alternate location.

### usage

```
mklabwall
	-f font[str]
	-s fontsize[int]
	-v font_vertical_position[int]
	-h font_horizontal_position[int]
	-n filename[str]
	-z fancy xD [no arg] (an easter egg!)
	-b background color["float float float"]
	-p save as png [no arg - default is svg]
	-l logo only [no arg]
	-r resolution ["int int"]

at least 1 option is required
file is saved to "HOME/labwcwall" or "HOME/myfile" with -n [arg] option
Version 0.0.1
```

Images are saved to your `$HOME` directory or any subdir of `$HOME` as
long as that subdir already exists. If it doesn't an error is produced.

The demonstration script `demo.sh` will produce 8 images in `$HOME/demo'

### inspiration

See [mkwallpaper](https://github.com/01micko/mkwallpaper)
