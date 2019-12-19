#!/usr/bin/env python

# -*- coding: utf-8 -*-

"""
Python script for displaying polygons.

This file is part of Davor Penzar's master thesis programing.

Usage:
    ./display.py [x_1 y_1 x_2 y_2 ... x_n y_n]
where (x_i, y_i) are coordinates of the i-th vertex of the polygon.  The
coordinates are given in decimal format.  The vertices are given in a cyclical
order.  There must be at least three distinct vertices and no coordinate may be
NaN or infinite.

To plot multiple polygons, seperate their coordinates by a plus, i. e. the
character `'+'`.  For example, to plot two triangles, use
    ./display x_11 y_11 x_12 y_12 x_13 y_13 + x_21 y_21 x_22 y_22 x_23 y_23
where (x_ij, y_ij) are coordinates of the j-th vertex of the i-th triangle.
Do not put a plus at the beginning of the command line arguments or at the end.

"""

# Import standard library modules.
import copy
import sys

# Import SciPy packages.
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import AutoMinorLocator

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

# Extract the indices of pluses (characters, actually strings `'+'`) among
# command line arguments.  Assume pluses are also on indices 0 and
# `len(sys.argv)`.
pluses = [0]
pluses += [i for i, arg in enumerate(sys.argv) if arg == '+']
try:
    del i
except (NameError, UnboundLocalError):
    pass
try:
    del arg
except (NameError, UnboundLocalError):
    pass
pluses.append(len(sys.argv))
pluses = copy.deepcopy(pluses)

# Set the style for Matplotlib.
plt.style.use('classic')

# Create a Matplotlib figure and an axis in the figure.
fig = plt.figure(num = 'Polygon', figsize = (8, 4.5))
ax = fig.add_subplot(1, 1, 1)

# Load the additional command line arguments as numeric values.  If an exception
# is raised during the conversion, raise an exception of type `ValueError`
# informing the user that the arguments were not decimal representations of
# real numbers.
P = None
try:
    P = [
        np.array(
            sys.argv[pluses[i] + 1:pluses[i + 1]],
            dtype = float,
            copy = True,
            order = 'F',
            subok = False,
            ndmin = 1
        ) for i in range(len(pluses) - 1)
    ]
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
except (TypeError, ValueError, AttributeError):
    raise ValueError(
        'Command line arguments must be decimal representations of real '
        'numbers.'
    )

# Free memory.
del pluses

# Initialise the array of colours to zeros.  The array is of shape (`len(P)`, 3)
# so that `col[i]` defines the colour in the RGB format (in the range [0, 1]) of
# the `i`-th polygon.
col = np.zeros((len(P), 3), dtype = float, order = 'F')

# Compute colours of polygons.

# Get the number of polygons increased by 1 in the `float` type.
C = float(len(P) + 1)

# Iterate over colours.
for i in range(len(P)):
    # Get the position of the current colour in spectrum.
    c = 6.0 * float(i) / C

    # Set the RGB values of the current colour according to the position `c`.
    if c < 1.0:
        col[i, 0] = 1.0
        col[i, 1] = c - 0.0
        col[i, 2] = 0.0
    elif c < 2.0:
        col[i, 0] = 2.0 - c
        col[i, 1] = 1.0
        col[i, 2] = 0.0
    elif c < 3.0:
        col[i, 0] = 0.0
        col[i, 1] = 1.0
        col[i, 2] = c - 2.0
    elif c < 4.0:
        col[i, 0] = 0.0
        col[i, 1] = 4.0 - c
        col[i, 2] = 1.0
    elif c < 5.0:
        col[i, 0] = c - 4.0
        col[i, 1] = 0.0
        col[i, 2] = 1.0
    else:
        col[i, 0] = 1.0
        col[i, 1] = 0.0
        col[i, 2] = 6.0 - c

    # Free the memory.
    del c

# Free the memory.
try:
    del i
except (NameError, UnboundLocalError):
    pass
del C

# Iterate over the polygons and plot them.
for i in range(len(P)):
    # Try to reshape the array `P[i]` in a two-dimensional array of coordinates.
    # If an exception of type `ValueError` occurs, raise an exception of type
    # `ValueError` informing the user about the problem.
    try:
        P[i] = P[i].reshape((int(P[i].size) // 2, 2)).copy(order = 'F')
    except ValueError:
        raise ValueError(
            'Each polygon must be defined with an even number of coordinates '
            '(two coordinates for each vertex).'
        )

    # Check if any of the coordinates is NaN or infinite.  If there is such
    # coordinate, raise an exception of type `ValueError` informing the user
    # about the problem.
    if (np.isnan(P[i].ravel()) | np.isinf(P[i].ravel())).any():
        raise ValueError('Coordinates must not be NaN or infinite.')

    # Check if there are at least three distinct points in the array `P[i]`.  If
    # there are not as many distinf points, raise an exception of type
    # `ValueError` informing the user about the problem.
    if np.unique(P[i], axis = 0).shape[0] < 3:
        raise ValueError(
            'Number of distinct vertices of each polygon must be at least 3.'
        )

    # Fill the polygon area.
    ax.fill(
        P[i][:, 0],
        P[i][:, 1],
            alpha = 0.15,
            color = col[i],
            linestyle = 'None',
            linewidth = 0
    )

    # Draw the polygon's edges.
    ax.plot(
        np.concatenate((P[i][:, 0], [P[i][0, 0]])),
        np.concatenate((P[i][:, 1], [P[i][0, 1]])),
            alpha = 0.5,
            color = col[i],
            marker = 'None',
            linestyle = '-',
            linewidth = 1
    )

    # Plot all the vertices except the first and the last.
    ax.plot(
        P[i][1:-1, 0],
        P[i][1:-1, 1],
            color = 'black',
            marker = '.',
            markersize = 5,
            linestyle = 'None'
    )

    # Plot the first and the last vertices.
    ax.plot(
        P[i][0, 0],
        P[i][0, 1],
            color = 'black',
            marker = '>',
            markersize = 5,
            linestyle = 'None'
    )
    ax.plot(
        P[i][-1, 0],
        P[i][-1, 1],
            color = 'black',
            marker = 'X',
            markersize = 5,
            linestyle = 'None'
    )

# Set the aspect of axis scaling to equal.
ax.set_aspect('equal')

# Show the figure.
plt.show()
