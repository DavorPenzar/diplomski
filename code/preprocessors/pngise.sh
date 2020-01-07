#!/usr/bin/env bash

##  Convert SVG images to PNG images.
##
##  This file is part of Davor Penzar's master thesis programing.
##
##  This is a very primitive program which takes arguments as paths to SVG
##  images and creates PNG images using Inkscape in the same directory, even
##  with the same file name but with extension ".png" append at the end.  For
##  example, an SVG image named "test.svg" would be converted to "test.svg.png"
##  in the same directory.  Also, images' width and height are hard coded to
##  1000 x 1000 px2.
##

# Convert images from SVG to PNG.
for svg in "$@"
do
    inkscape -z -e "$svg" -w 1000 -h 1000 "$svg.png"
done
