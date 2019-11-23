#!/usr/bin/env python3

# -*- coding: utf-8 -*-

"""
Python script for displaying polygons.

Usage:
    ./display.py x_1 y_1 x_2 y_2 x_3 y_3 [x_4 y_4 ... x_n y_n]
where (x_i, y_i) are coordinates of the i-th vertex of the polygon.  The
coordinates are given in decimal format.  The vertices are given in a cyclical
order.  There must be at least three distinct vertices and no coordinate may be
NaN or infinite.

"""

# Import standard library modules.
import sys

# Import SciPy packages.
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np

# Check if the variable `__name__` is in the right format.
if not (isinstance(__name__, str) and __name__.__class__ is str):
    raise RuntimeError('`__name__` must be an instance of class `str`.')

# Check if the module is loaded as the main script.
if not (__name__ == '__main__'):
    raise RuntimeError('Module must be run as a Python script.')

# Check if the variable `sys.argv` is in the right format.
if not (isinstance(sys.argv, list) and sys.argv.__class__ is list):
    raise RuntimeError('`sys.argv` must be an instance of class `list`.')

# Check if the number of command line arguments is OK.
if len(sys.argv) < 7 or not (len(sys.argv) & 1):
    raise RuntimeError(
        'Number of additional command line arguments must be a positive number '
        'greater than or equal to 6.'
    )

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

# Load the additional command line arguments as numeric values.  If an exception
# is raised during the conversion, raise an exception of type `ValueError`
# informing the user that the arguments were not decimal representations of
# real numbers.
P = None
try:
    P = np.array(
        sys.argv[1:],
        dtype = float,
        copy = True,
        order = 'F',
        subok = False,
        ndmin = 1
    )
except (TypeError, ValueError, AttributeError):
    raise ValueError(
        'Command line arguments must be decimal representations of real '
        'numbers.'
    )

# Reshape the array `P` in a two-dimensional array of coordinates.
P = P.reshape((int(P.size) // 2, 2)).copy(order = 'F')

# Check if any of the coordinates is NaN or infinite.  If there is such
# coordinate, raise an exception of type `ValueError` informing the user about
# the problem.
if (np.isnan(P.ravel()) | np.isinf(P.ravel())).any():
    raise ValueError('Coordinates must not be NaN or infinite.')

# Check if there are at least three distinct points in the array `P`.  If there
# are not as many distinf points, raise an exception of type `ValueError`
# informing the user about the problem.
if np.unique(P, axis = 0).shape[0] < 3:
    raise ValueError('Number of distinct vertices must be at least 3.')

# Create a Matplotlib figure and an axis in the figure.
fig = plt.figure(num = 'Polygon', figsize = (8, 4.5))
ax = fig.add_subplot(1, 1, 1)

# Print the title.
ax.set_title("$ {0:d} $-gon".format(int(P.shape[0])))

# Set the aspect of axis scaling to equal.
ax.set_aspect('equal')

# Show the grid.
ax.grid(True, which = 'both', axis = 'both')

# Fill the polygon area.
ax.fill(
    P[:, 0],
    P[:, 1],
        alpha = 0.15,
        color = 'black',
        linestyle = 'None',
        linewidth = 0
)

# Draw the polygon's edges.
ax.plot(
    np.concatenate((P[:, 0], [P[0, 0]])),
    np.concatenate((P[:, 1], [P[0, 1]])),
        alpha = 0.5,
        color = 'black',
        marker = 'None',
        linestyle = '-',
        linewidth = 1
)

# Plot all the vertices except the first and the last.
ax.plot(
    P[1:-1, 0],
    P[1:-1, 1],
        color = 'black',
        marker = '.',
        markersize = 5,
        linestyle = 'None'
)

# Plot the first and the last vertices.
ax.plot(
    P[0, 0],
    P[0, 1],
        color = 'black',
        marker = '>',
        markersize = 5,
        linestyle = 'None'
)
ax.plot(
    P[-1, 0],
    P[-1, 1],
        color = 'black',
        marker = 'X',
        markersize = 5,
        linestyle = 'None'
)

# Show the figure.
plt.show()
