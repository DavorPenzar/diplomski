#!/usr/bin/env python

# -*- coding: utf-8 -*-

"""
Generate SVG images of polygons.

This file is part of Davor Penzar's master thesis programing.

Usage:
    ./svgise.py in_table out_directory
where
    in_table        is the path to the input table (TSV file; actually single
                    tab separated values),
    out_directory   is the path to the output directory.

All input polygons (polygons in the input table) must have the same number of
vertice.  Each polygon must be formated in the input table as
    x_0	y_0	x_1	y_1	...	x_n_minus_1	y_n_minus_1
where x_i denotes the x-coordinate of the i-th vertex and y_i denotes the
y-coordinate of the i-th vertex, and n is the number of vertices (at least 3).

For each polygon in the input file a single SVG image is generated in the output
directory.  SVG files are named after the input table (minus the extension),
then an underscore, and then the index of the polygon in the input table (zero
padded).  For example, if the input file is "triangles.tsv" with 750 rows,
output directory is "images", then for the 12th polygon (index 11) in the
input table a file "images/triangles_011.svg" is generated.  The boundary box of
each SVG file is [-0.5, 0.5] x [-0.5, 0.5] px2.

The code was inspired by ewcz's answer on
https://stackoverflow.com/questions/49147707/how-can-i-convert-a-shapely-polygon-to-an-svg.

"""

# Import standard library modules.
import inspect
import os
import sys

# Import SciPy packages.
import numpy as np
import pandas as pd

# Import Shapely.
import shapely as sh
import shapely.geometry as gm

# Check if the variable `__name__` is in the right format.
if not (isinstance(__name__, str) and __name__.__class__ is str):
    raise RuntimeError('`__name__` must be an instance of class `str`.')

# Check if the module is loaded as the main script.
if not (__name__ == '__main__'):
    raise RuntimeError('Module must be run as a Python script.')

# Check if the variable `sys.argv` is in the right format.
if not (isinstance(sys.argv, list) and sys.argv.__class__ is list):
    raise RuntimeError('`sys.argv` must be an instance of class `list`.')

# Check if all the command line arguments are in the right format.
for arg in sys.argv:
    if not (isinstance(arg, str) and arg.__class__ is str):
        raise RuntimeError(
            'Command line arguments must be instances of class `str`'
        )
try:
    del arg
except (NameError, UnboundLocalError):
    pass

# If no command line arguments were given, print the documentation and exit.
if len(sys.argv) != 3:
    # Print the documentation.
    print(inspect.cleandoc(__doc__))

    # Exit.
    sys.exit(0)

if len(sys.argv[1]) < 4:
    raise ValueError('Input table must be TSV, CSV or TXT file.')
if sys.argv[1][-4:].lower() not in {'.csv', '.tsv', '.txt'}:
    raise ValueError('Input table must be TSV, CSV or TXT file.')

# Try to open the input file.  If an exception occurs, raise an exception of
# type `RuntimeError`.
df = None
try:
    df = pd.read_csv(
        sys.argv[1],
        sep = "\t",
        header = None,
        index_col = None,
        dtype = float
    )
except (TypeError, ValueError, AttributeError, IndexError, KeyError):
    raise RuntimeError(
        "Cannot open file \"{path:s}\".".format(path = sys.argv[1])
    )

# Check the shape of the input table: it must be non-empty with an even number
# (at least 6) of columns.
if not df.size:
    raise RuntimeError('Input table must be non-empty.')
if (df.shape[1] & 1) or df.shape < 6:
    raise RuntimeError(
        'Input table must have an even number of columns greater than 5.'
    )

# Extract the number of polygons.
N = int(df.shape[0])

# Extract the number of vertices of each polygon.
n = int(df.shape[1]) >> 1

# Set the bounding box.
bbox = np.array([-0.5, -0.5, 0.5, 0.5], dtype = float, order = 'F')

# Set the scale of the output images.
scale = 10.0

# Compute the width and the height of the bounding box.
width = bbox[2] - bbox[0]
height = bbox[3] - bbox[1]

# Set the format string for the paths of the output images.
path_str = os.path.join(
    sys.argv[2] if sys.argv[2] else '.',
    "{table:s}_{{ind:0{nw:d}d}}.svg".format(
        table = os.path.basename(sys.argv[1])[:-4],
        nw = len(str(max(N - 1, 1)))
    )
)

# Set the format string for the output SVG data.
svg_str = (
    "<?xml version=\"1.0\" encoding=\"utf-8\" ?>\n"
        "<"
            "svg version=\"1.1\" "
            "baseProfile=\"full\" "
            "width=\"{ws:.0f}px\" "
            "height=\"{hs:.0f}px\" "
            "viewBox=\"{xmin:.1f},{ymin:.1f},{w:.1f},{h:.1f}\" "
            "xmlns=\"http://www.w3.org/2000/svg\" "
            "xmlns:ev=\"http://www.w3.org/2001/xml-events\" "
            "xmlns:xlink=\"http://www.w3.org/1999/xlink\""
        ">\n"
        "{{data:s}}\n"
    "</svg>\n"
).format(
    ws = width * scale,
    hs = height * scale,
    xmin = bbox[0],
    ymin = bbox[1],
    w = width,
    h = height
)

# Generate images of the polygons.  If an exception of `FileNotFoundError`
# occurs, raise an exception of type `RuntimeError`.
try:
    for i in df.index:
        # Open the file for the current polygon.
        with open(path_str.format(ind = i), 'wt') as svg:
            # Write SVG data of the current polygon to file `svg`.
            svg.write(
                svg_str.format(
                    data = gm.Polygon(
                        [
                            (df.loc[i, 2 * j], df.loc[i, 2 * j + 1])
                                for j in range(n)
                        ]
                    ).svg(scale_factor = 0.0, fill_color = '#000000')
                )
            )
        try:
            del svg
        except (NameError, UnboundLocalError):
            pass
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
except FileNotFoundError:
    raise RuntimeError(
        "Cannot open output files (check if directory \"{dir:s}\" "
        "exists).".format(dir = sys.argv[2] if sys.argv[2] else '.')
    )
