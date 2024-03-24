# mklabimg

This particular program can reproduce the `labwc3-with-text.png` file
used for the official [labwc website](https://labwc.github.io).

### build

```
make
```

and optionally, as root, install the resulting `mklabimg` binary
to `/usr/local/bin` :

```
make install
```

### reproducer

Here are the exact commands that reproduce the current website image.
Note that `Nunito` font must be installed on your system.

```
$ mklabimg -f nunito -s 116 -v 34 -h 290 -n labwc3-with-text
$ rsvg-convert -w 199 -h 61 labwc3-with-text.svg > labwc3-with-text.png
```

Files are saved to your `$HOME` directory.
In the above case `$HOME/labwc3-with-text.svg` is produced.
The `labwc3-with-text.svg` produced is in the current directory
of this repository.

### usage

Run `mklabimg` image with no arguments and a brief usage message
is printed.

You can change to any font on your system and there is small scope
for font size and placement.

The background is hard coded to 600 x 184 which scales down perfectly
to the website dimensions of 199 x 61. By default the background color
is transparent but this can be changed with the `-b "float,float,float"`
option.

The main `labwc` icon is hard coded along with the "labwc" text.
