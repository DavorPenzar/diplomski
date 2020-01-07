#!/usr/bin/env bash

##  Convert SVG images to PNG images.
##
##  This file is part of Davor Penzar's master thesis programing.
##
##  Usage:
##      ./pngise.sh in_directory out_directory
##  where:
##      in_directory    is the path to the directory containing origina SVD
##                      images,
##      out_directory   is the path to the directory to which to export
##                      the converted PNG images.
##
##  All files ending in ".svg" in in_directory (and only ".svd"; ".SVD" files
##  are ignored) will be converted and their conversions will be saved to
##  out_directory with the same names but different extensions (extension ".png"
##  is used).  It is assumed the files are of square shape; to change the
##  dimensions of the resulting PNG images, change the code below.
##
##  The pogram prints to the console the time elapsed during the conversion of
##  images.
##

# Variable to save the value returned by the `slashise_directory` function.
dirpath=""

# Ensure a directory path ends with the '/' character.  The resulting path is
# saved to the `$dirpath` variable.
function slashise_directory {
    dirpath=""
    case $1 in
       */)
          dirpath=$1
          ;;
       *)
          dirpath="$1/"
          ;;
    esac
}

# Get the input directory with the '/' character at the end.
slashise_directory $1
in=$dirpath

# Get the output directory with the '/' character at the end.
slashise_directory $2
out=$dirpath

# Initialise time points.
t0=0
t1=0

# Get the current time in nanoseconds.
t0=$(date "+%s%N")

# Convert images from SVG to PNG.
for svg in "$in"*.svg
do
    # Get the path to the resulting PNG image.
    png=${svg#$in}
    png=${png%.svg}
    png="$out$png.png"

    # Convert image.
    inkscape -z $svg -w 64 -h 64 -e $png
done

# Get the current time in nanoseconds.
t1=$(date "+%s%N")

# Print the time elapsed during the conversion of images.
printf "Time elapsed: %.6f s.\n" $(echo "0.000000001 * ($t1 - $t0)" | bc)
