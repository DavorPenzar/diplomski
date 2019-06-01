# -*- coding: utf-8 -*-

"""
Useful functions.

"""

# Standard Python library.
import copy as _copy
import math as _math
import numbers as _numbers
import six as _six
import warnings as _warnings

# SciPy packages.
import matplotlib as _mpl
import matplotlib.pyplot as _plt
import numpy as _np
import scipy.sparse as _sp
from matplotlib.axes import Axes as _Axes
from mpl_toolkits.mplot3d import Axes3D as _Axes3D

def triang (x = 1.0, y = None, num = 50):
    """
    Compute the discretization of a rectangle containing an open triangle.

    Parameters
    ----------
    x : float or (3,) array, optional
        If a single floating point value is passed, it must not be NaN, infinite
        or negative: it then defines the width of the triangle.  If the array of
        three values is passed, each defines the x-value of the three points of
        the triangle (none of them may be NaN or infinite).  Passing a single
        value 0 <= x < inf is the same as passing [-x / 2, x / 2, 0].  If a
        single value is passed as the parameter x and None is passed as y, the
        value of x does not affect the resulting rectangle (probably except for
        extremely large or extremely small values, and definitely except for
        zero and non-zero values), therefore the parameter x is optional.

    y : None or float or (3,) array, optional
        If a single floating point value is passed, it must not be NaN, infinite
        or negative: it then defines the height of the triangle.  If the array
        of three values is passed, each defines the y-value of the three points
        of the triangle (none of them may be NaN or infinite).  Passing a single
        value 0 <= y < inf is the same as passing [-y / 2, -y / 2, y / 2].  If
        None, 0.5 * abs(x[1] - x[0]) * sqrt(3) is used as the height of the
        triangle.

    num : int, optional
        The number of discretization points used to discretize the wider
        dimension of the resulting rectangle.  The parameter num must be greater
        than 1.

    Returns
    -------
    Omega : (M, N) array
        Array of trues and falses representing the discretization of the
        rectangle containing the closure of the triangle.  The rectangle will
        stretch from min(x) to max(x) on its first dimension, and from min(y) to
        max(y) on its second dimension, and the equality max(M, N) == num will
        be true.  The values of Omega are defined as: Omega[i, j] == True if and
        only if (i, j) is inside the open triangle.  Note that the first
        dimension represents the x-axis.

    Raises
    ------
    TypeError
        If x or y is not a floating point number or an array of real dtype (y
        may be None), an exception of type TypeError is raised.  If num is not
        an integral value, an exception of type TypeError is raised.

    ValueError
        If x or y is a single value and it is negative, an exception of type
        ValueError is raised.  If x or y contains a NaN or an infinite value, an
        exception of type ValueError is raised.  If num is less than 2, an
        exception of type ValueError is raised.

    """

    # Define comparison objects used for ordering < and >.
    LT_check = lambda r, s : r < s
    GT_check = lambda r, s : r > s

    # Sanitize the parameter x.
    if hasattr(x, '__iter__') or hasattr(x, '__array__'):
        if not isinstance(x, _np.ndarray):
            try:
                x = _np.array(x)
            except (TypeError, ValueError, AttributeError):
                raise TypeError('x must be a scalar or an array.')
        if not issubclass(x.dtype.type, _numbers.Real):
            raise TypeError('x must be real.')
        x = x.asytep(float)
        if not x.ndim == 1:
            raise ValueError('x must be 1-dimensional array.')
        if not x.size == 3:
            raise ValueError('x must have exactly 3 elements.')
    else:
        if not isinstance(x, _numbers.Real):
            raise TypeError('Width must be real.')
        x = _copy.deepcopy(float(x))
        if x < 0.0:
            raise ValueError('Width must not be negative')
        x = 0.5 * _np.array([-x, x, 0.0], dtype = float, order = 'F')
    if _np.isnan(x).any() or _np.isinf(x).any():
        raise ValueError('x must be non-NaN and finite.')

    # If y is None, define the heigt of the triangle.
    if y is None:
        y = 0.5 * _math.sqrt(3.0) * abs(float(x[1] - x[0]))

    # Sanitize the parameter y.
    if hasattr(y, '__iter__') or hasattr(y, '__array__'):
        if not isinstance(y, _np.ndarray):
            try:
                y = _np.array(y)
            except (TypeError, ValueError, AttributeError):
                raise TypeError('y must be a scalar or an array.')
        if not issubclass(y.dtype.type, _numbers.Real):
            raise TypeError('y must be real.')
        y = y.astype(float)
        if not y.ndim == 1:
            raise ValueError('y must be 1-dimensional array.')
        if not y.size == 3:
            raise ValueError('y must have exactly 3 elements.')
    else:
        if not isinstance(y, _numbers.Real):
            raise TypeError('Height must be real.')
        y = _copy.deepcopy(float(y))
        if y < 0.0:
            raise ValueError('Height must be positive.')
        y = 0.5 * _np.array([-y, -y, y], dtype = float, order = 'F')
    if _np.isnan(y).any() or _np.isinf(y).any():
        raise ValueError('y must be non-NaN and finite.')

    # Sanitize the parameter num.
    if not isinstance(num, _numbers.Integral):
        raise TypeError('Number of discretization points must be integral.')
    num = _copy.deepcopy(int(num))
    if num < 2:
        raise ValueError(
            'Number of discretization points must be greater than 1.'
        )

    # Compute the boundaries and the dimensions of the rectangle.

    x_min = float(x.min())
    x_max = float(x.max())

    y_min = float(y.min())
    y_max = float(y.max())

    a = x_max - x_min
    b = y_max - y_min

    # Initialize the rectangle to all falses.
    Omega = None
    if a > b:
        Omega = _np.zeros(
            (num, int(round(b / a * num))),
            dtype = _np.bool8,
            order = 'F'
        )
    elif a < b:
        Omega = _np.zeros(
            (int(round(a / b * num)), num),
            dtype = _np.bool8,
            order = 'F'
        )
    else:
        Omega = _np.zeros((num, num), dtype = _np.uint8, order = 'F')

    # Discretize the rectangle.
    u = _np.linspace(x_min, x_max, num = Omega.shape[0])
    v = _np.linspace(y_min, y_max, num = Omega.shape[1])

    # Re-enumerate the arrays x, y to satisfy the equality y[2] == max(y).
    while y[2] != y_max:
        x = _np.roll(x, 1)
        y = _np.roll(y, 1)

    ##  The following code is derived from the formula generating the line
    ##  interpolating two different points on a plane.  For the points
    ##  (x_A, y_A) and (x_B, y_B), if x_B != x_A, the line is generated by the
    ##  explicit formula
    ##      y = (y_B - y_A) / (x_B - x_A) * (x - x_A) + y_A.
    ##  If x_B == x_A, then y_B != y_A (otherwise the points would not be
    ##  different) and the formula could be generated to define x for a fixed
    ##  value of y.  Either way, a point is below the line if the equality is
    ##  replaced by <, and above it if the equality is replaced by >.  Then, we
    ##  multiply the inequality by (x_B - x_A) and eventualy get
    ##      (x_B - x_A) * (y - y_A) @ (y_B - y_A) * (x - x_A)
    ##  where @ is the corresponding sign among < and > (opposite the original
    ##  if x_B < x_A).  The same inequality is derived from the formula defining
    ##  x over y (inequalities then define the position as left or right rather
    ##  than down or up).

    # Using the objects LT_check and GT_check, define the comparison objects to
    # check if a point is inside the triangle.  The point is inside the triangle
    # if it is above the line (x[0], y[0]), (x[1], y[1]) and below the other two
    # lines; however, if x[(i + 1) % 3] < x[i], then the inequalities < and >
    # are inverted to check where a point lies in regards to the i-th line.
    liner_check = [
        GT_check,
        (
            LT_check if (
                (x[2] - x[1]) * (x[0] - x[1]) + (y[2] - y[1]) * (y[0] - y[1]) >
                0
            ) else GT_check
        ),
        (
            LT_check if (
                (x[1] - x[2]) * (x[0] - x[2]) + (y[1] - y[2]) * (y[0] - y[2]) >
                0
            ) else GT_check
        )
    ]
    if x[1] < x[0]:
        liner_check[0] = GT_check if liner_check[0] is LT_check else LT_check
    if x[2] < x[1]:
        liner_check[1] = GT_check if liner_check[1] is LT_check else LT_check
    if x[0] < x[1]:
        liner_check[2] = GT_check if liner_check[2] is LT_check else LT_check

    # Compute the values to compare to check if the point is inside the
    # triangle.
    u_liner = _np.array(
        [
            (y[1] - y[0]) * (u - x[0]),
            (y[2] - y[1]) * (u - x[1]),
            (y[0] - y[2]) * (u - x[2])
        ],
        dtype = float,
        order = 'F'
    )
    v_liner = _np.array(
        [
            (x[1] - x[0]) * (v - y[0]),
            (x[2] - x[1]) * (v - y[1]),
            (x[0] - x[2]) * (v - y[2])
        ],
        dtype = float,
        order = 'F'
    )

    # Fill the rectangle with trues where needed.
    for i in iter(range(int(u.size))):
        for j in iter(range(int(v.size))):
            Omega[i, j] = _np.bool8(
                liner_check[0](v_liner[0, j], u_liner[0, i]) and
                liner_check[1](v_liner[1, j], u_liner[1, i]) and
                liner_check[2](v_liner[2, j], u_liner[2, i])
            )

    # Return the rectangle.
    return Omega

def ellips (a = 1.0, b = None, num = 50):
    """
    Compute the discretization of a rectangle containing an open ellipsis.

    Parameters
    ----------
    a : float, optional
        The width of the ellipsis.  The width must not be NaN, infinite or
        negative.  If a value is passed as the parameter a and None is passed as
        b, the value of a does not affect the resulting rectangle (probably
        except for extremely large or extremely small values, and definitely
        except for zero and non-zero values), therefore the parameter a is
        optional.

    b : None or float, optional
        The height of the ellipsis.  The width must not be NaN, infinite or
        negative.  If None, height will be equal to width.

    num : int, optional
        The number of discretization points used to discretize the wider
        dimension of the resulting rectangle.  The parameter num must be greater
        than 1.

    Returns
    -------
    Omega
        Array of trues and falses representing the discretization of the
        rectangle containing the closure of the ellipsis.  The rectangle will
        stretch from -a to a on its first dimension, and from -b to b on its
        second dimension, and the equality max(M, N) == num will be true.  The
        values of Omega are defined as: Omega[i, j] == True if and only if
        (i, j) is inside the open ellipsis.  Note that the first dimension
        represents the x-axis.

    Raises
    ------
    TypeError
        If any of a, b is not a floating point number, an exception of type
        TypeError is raised.  If num is not an integral value, an exception of
        type TypeError is raised.

    ValueError
        If a or b is NaN, infinite or negative, an exception of type ValueError
        is raised.  If num is less than 2, an exception of type ValueError is
        raised.

    """

    # Sanitize the parameter a.
    if not isinstance(a, _numbers.Real):
        raise TypeError('a must be real.')
    a = _copy.deepcopy(float(a))
    if _math.isnan(a) or _math.isinf(a):
        raise ValueError('a must be non-NaN and finite.')
    if a < 0:
        raise ValueError('a must not be negative.')

    # If b is None, define the height of the ellipsis.
    if b is None:
        b = a

    # Sanitize the parameter b.
    if not isinstance(b, _numbers.Real):
        raise TypeError('b must be real.')
    b = _copy.deepcopy(float(b))
    if _math.isnan(b) or _math.isinf(b):
        raise ValueError('b must be non-NaN and finite.')
    if b < 0:
        raise ValueError('b must not be negative.')

    # Sanitize the parameter num.
    if not isinstance(num, _numbers.Integral):
        raise TypeError('Number of discretization points must be integral.')
    num = _copy.deepcopy(int(num))
    if num < 2:
        raise ValueError(
            'Number of discretization points must be greater than 1.'
        )

    # Initialize the rectangle to all falses.
    Omega = None
    if a > b:
        Omega = _np.zeros(
            (num, int(round(b / a * num))),
            dtype = _np.bool8,
            order = 'F'
        )
    elif a < b:
        Omega = _np.zeros(
            (int(round(a / b * num)), num),
            dtype = _np.bool8,
            order = 'F'
        )
    else:
        Omega = _np.zeros((num, num), dtype = _np.uint8, order = 'F')

    # Discretize the rectangle.
    u = _np.linspace(-a, a, num = Omega.shape[0])
    v = _np.linspace(-b, b, num = Omega.shape[1])

    ##  The following code is derived from the implicit ellipsis formula
    ##      b^2 * x^2 + a^2 * y^2 = a^2 * b^2
    ##  because the formula above allows for a and b to be zero.

    # Compute a^2, b^2 and a^2 * b^2.
    a2 = a ** 2
    b2 = b ** 2
    ab2 = a2 * b2

    # Compute the squares of the coordinates inside the rectangle.
    u2 = u ** 2
    v2 = v ** 2

    # Fill the rectangle with trues where needed.
    for i in iter(range(int(u.size))):
        for j in iter(range(int(v.size))):
            Omega[i, j] = _np.bool8(b2 * u2[i] + a2 * v2[j] < ab2)

    # Return the rectangle.
    return Omega

def eigenfunc (Omega, ignore_zeros = True):
    """
    Compute the Dirichlet Laplacian eigenfunctions and eigenvalues on Omega.

    Parameters
    ----------
    Omega : (M, N) array
        Omega must be a 2-dimensional non-empty array (Omega.size > 0) of
        integral dtype and must contain only the values 0 and 1 (at least one
        element must be non-zero, and no element in the first or the last row
        and in the first or the last column may be 1).  As such, Omega
        represents an equidistant discretization of a rectangle containing the
        2-dimensional domain at which the Dirichlet Laplacian eigenfunctions and
        eigenvalues are computed: Omega[i, j] == 1 if and only if (i, j) is
        inside Omega.  Note that the first dimension represents the x-axis.

    ignore_zeros : boolean, optional
        If true, eigenvalues close to or equal to zero are ignored (as well as
        their corresponding eigenfunctions); otherwise all the real eigenvalues
        and their corresponding real-valued eigenfunctions are returned.
        Closeness to zero is defined as
            >>> def is_zero (x):
            ...     numpy.isclose(1, 1 + x)

    Returns
    -------
    l : (K,) array
        Array containing the real Dirichlet Laplacian eigenvalues sorted
        ascendingly.

    u : (K, M, N) array
        Array containing the discrete approximations of the real-valued
        Dirichlet Laplacian eigenfunctions.  The "function" u[i] corresponds to
        the eigenvalue l[i].  It is guarranteed that all the "functions'" values
        are in the range [-1, 1] and that each "function" is a constant
        zero-valued "function" or it reaches 1 on Omega.

    Raises
    ------
    TypeError
        If Omega is not array-like or if its dtype is not integral, an exception
        of type TypeError is raised.  If ignore_zeros is not boolean, an
        exception of type TypeError is raised.

    ValueError
        If Omega is not a non-empty matrix containing only 0 and 1, with at
        least one non-zero value, and all zeros on its borderline rows and
        columns, an exception of type ValueError is raised.

    RuntimeError
        If no real non-zero eigenvalues corresponding to real-valued
        eigenfunctions are found, an exception of type RuntimeError is raised.

    """

    # Sanitize the parameter Omega.
    if not isinstance(Omega, _np.ndarray):
        if not (hasattr(Omega, '__iter__') or hasattr(Omega, '__array__')):
            raise TypeError('Omega must be a numpy array.')
        try:
            Omega = _np.array(Omega)
        except (TypeError, ValueError, AttributeError):
            raise TypeError('Omega must be a numpy array.')
    if not issubclass(
        Omega.dtype.type,
        (_numbers.Integral, int, bool, _np.bool, _np.bool_, _np.bool8)
    ):
        raise TypeError('Omega must be an integral array.')
    if Omega.ndim != 2:
        raise ValueError('Omega must be a matrix (a 2-dimensional array).')
    if not Omega.size:
        raise ValueError('Omega must be non-empty.')
    if not _np.isin(Omega.ravel(), [0, 1]).all():
        raise ValueError('Omega must contain only 0 and 1.')
    if not Omega.ravel().any():
        raise ValueError('Omega must contain at least one non-zero value.')
    if Omega[[0, -1], :].ravel().any() or Omega[:, [0, -1]].ravel().any():
        raise ValueError(
            'Borderline rows and columns of Omega must contain only zeros.'
        )
    Omega = Omega.astype(_np.bool8)

    # Sanitize the parameter ignore_zeros.
    if not isinstance(
        ignore_zeros,
        (_numbers.Integral, int, bool, _np.bool, _np.bool_, _np.bool8)
    ):
        raise TypeError('Parameter non_zero must be boolean.')
    if ignore_zeros not in {0, 1}:
        raise ValueError('Parameter non_zero must be either False or True.')
    try:
        ignore_zeros = _copy.deepcopy(bool(ignore_zeros))
    except (TypeError, ValueError, AttributeError):
        raise ValueError('Parameter non_zero must be boolean.')

    # Construct the complete Laplacian approximation matrix (no zero-rows).
    d0 = -4 * _np.ones(Omega.size, dtype = float)
    d1 = _np.ones(Omega.size, dtype = float)
    D = _sp.spdiags(
        (d1, d1, d0, d1, d1),
        (-Omega.shape[0], -1, 0, 1, Omega.shape[0]),
        Omega.size,
        Omega.size
    ).tolil()
    del d0
    del d1

    # Set the rows of the matrix D corresponding to coordinates outside of Omega
    # to zero.
    I = list(range(int(Omega.shape[0])))
    J = list(range(int(Omega.shape[1])))
    for i in iter(I):
        for j in iter(J):
            if not Omega[i, j]:
                D[i + Omega.shape[0] * j] = 0
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    try:
        del j
    except (NameError, UnboundLocalError):
        pass
    del I
    del J

    # Convert the Laplacian matrix to Compressed Sparse Column matrix.
    D = D.tocsc()

    ##  TODO: utilize the scipy.sparse library to compute eigenvalues and
    ##  eigenfunctions.

    # Compute the eigenvalues and eigenvectors of the matrix D.
    l, u = _np.linalg.eig(-D.todense(order = 'F'))

    # Convert the matrix u to true array.
    if isinstance(u, _np.matrix):
        u = u.A

    # Free the memory.
    del D

    # Compute the ascending order of the eigenvalues ignoring the values to be
    # ignored (non-real eigenvalues and eigenvectors and zeros if
    # ignore_zeros == True).
    I = None
    l_abs = _np.abs(l)
    u_abs = _np.abs(u)
    I = _np.argsort(l_abs)
    while I.size:
        if ignore_zeros:
            I = I[~_np.isclose(1, 1 + l_abs[I])]
            if not I.size:
                break
        else:
            l_abs[_np.isclose(1, 1 + l_abs)] = 1
        u_abs[_np.isclose(1, 1 + u_abs)] = 1
        I = I[_np.isclose(1, 1 + l[I].imag / l_abs[I])]
        if not I.size:
            break
        I = I[
            _np.isclose(1, 1 + u[:, I].imag / u_abs[:, I]).all(axis = 0).ravel()
        ]
        break
    del l_abs
    del u_abs

    # If no eigenvalue or eigenvector satisfying the conditions were found,
    # raise an exception of type RuntimeError.
    if not I.size:
        raise RuntimeError(
            'No real {bnz:s}eigenvalues and real eigenfunctions were '
            'found.'.format(bnz = ' non-zero' if ignore_zeros else '')
        )

    # Assure I is a stand-alone array (not a view to another array).
    I = I.copy(order = 'F')

    # Convert, reorder and filter the eigenvalues and eigenvectors.
    l = _np.array(l[I].real, dtype = float, copy = True, order = 'F')
    u = _np.array(u[:, I].real, dtype = float, copy = True, order = 'F')

    # Free the memory.
    del I

    # Reshape u to a three-dimensional array.
    u = _np.array(
        tuple(
            u[:, i].reshape(Omega.shape) for i in iter(range(int(u.shape[1])))
        ),
        dtype = float,
        order = 'C'
    )
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    if u.ndim == 2:
        u = u.reshape((1, u.shape[0], u.shape[1]))._copy(order = 'C')

    # Compute the inverse of Omega.
    Omega_inv = ~Omega

    # Free the memory.
    del Omega

    # Assure no eigenfunction evaluates to a non-zero value outside of Omega,
    # that the non-zero eigenfunctions' range is the subset of the interval
    # [-1, 1] and that the non-zero eigenfunctions reach 1.
    with _warnings.catch_warnings():
        _warnings.filterwarnings('error')
        with _np.errstate(divide = 'raise'):
            for i in iter(range(int(u.shape[0]))):
                u[i, Omega_inv] = 0
                aux_ui = u[i].ravel().copy(order = 'C')
                try:
                    u[i] /= aux_ui[_np.argmax(_np.abs(aux_ui))]
                except (FloatingPointError, ZeroDivisionError, RuntimeWarning):
                    u[i] = 0
                del aux_ui
                u[i, ~u[i].astype(bool)] = 0
            try:
                del i
            except (NameError, UnboundLocalError):
                pass

    # Free the memory.
    del Omega_inv

    # Return the eigenvalues and eigenfunctions.
    return (l, u)

def show_func (u, dom = None, ax = None, how = 'contourf', *args, **kwargs):
    """
    Plot a real function on a twodimensional rectangular domain.

    Parameters
    ----------
    u : (M, N) array
        Representation of a real function on a discrete rectangle.  Note that
        the first dimension represents the x-axis.

    dom : None or (2, 2) array, optional
        Boundaries of the domain.  The function u is plotted as the funtion on
        the rectangle defined by the vertices (dom[0, 0], dom[1, 0]),
        (dom[0, 1], dom[1, 0]), (dom[0, 1], dom[1, 1]), (dom[0, 0], dom[1, 1]).
        The array must be sorted ascendingly on the axis 1.  If None, the
        vertices are computed such that the ratio of the rectangle's width
        by the rectangle's height is equal to M / N and that the narrower
        dimension of the rectangle stretches from -1 to 1.

    ax : None or matplotlib.axes.Axes or mpl_toolkits.mplot3d.Axes3D, optional
        Axis at which the surface is plotted.  If None, matplotlib.pyplot.gca()
        is used.

    how : str, optional
        Type of the plot.  The function is plotted by calling
        ax.__getattribute__(how)(...).

    *args, **kwargs
        Additional arguments passed to ax.__getattribute__(how)(...) after the
        first three arguments (X, Y and Z).

    Returns
    -------
    ax : matplotlib.axes.Axes or mpl_toolkits.mplot3d.Axes3D
        Axis at which the surface is plotted.

    Raises
    ------
    TypeError
        If the parameter u or the parameter dom is not an array of real values,
        an exception of type TypeError is raised.  If the parameter ax is not an
        isinstance of class matplotlib.axes.Axes or mpl_toolkits.mplot3d.Axes3D,
        an exception of type TypeError is raised.  If the parameter how is not a
        string, an exception of type TypeError is raised.

    ValueError
        If the parameter u or the parameter dom is not of the adequate shape, if
        the parameter u is an empty array or if any of its dimensions is equal
        to 1, an exception of type ValueError is raised.  If any of the values
        in the arrays u and dom is NaN or infinite, an exception of type
        ValueError is raised.  If the script is run in a Python2 environment and
        the parameter how is not a Python2 compatible string, an exception of
        type ValueError is raised.

    ...
        If the command
            >>> ax.__getattribute__(how)(X, Y, Z, *args, **kwargs)
        (where the shapes and values of X, Y and Z are not the problem if the
        parameter how defines a regular plot type that accepts 3 arrays as data
        to be plotted) raises an exception, it is not caught.

    """

    # Sanitize the parameter u.
    if not isinstance(u, _np.ndarray):
        if not (hasattr(u, '__iter__') or hasattr(u, '__array__')):
            raise TypeError('u must be a numpy array.')
        try:
            u = _np.array(u)
        except (TypeError, ValueError, AttributeError):
            raise TypeError('u must be a numpy array.')
    if not issubclass(
        u.dtype.type,
        (_numbers.Real, int, bool, _np.bool, _np.bool_, _np.bool8)
    ):
        raise TypeError('u must be a real-valued or a boolean array.')
    if u.ndim != 2:
        raise ValueError('u must be a matrix (2-dimensional array).')
    if not u.size:
        raise ValueError('u must be non-empty.')
    if u.shape[0] < 2 or u.shape[1] < 2:
        raise ValueError('Each dimension of u must be at least 2.')
    if _np.isnan(u).any() or _np.isinf(u).any():
        raise ValueError('u must not contain NaN or infinite values.')

    # Sanitize the parameter dom.
    if dom is None:
        dom = _np.array([[-1.0, 1.0], [-1.0, 1.0]], dtype = float, order = 'C')
        if u.shape[0] > u.shape[1]:
            q = float(u.shape[0]) / float(u.shape[1])
            dom[0, 0] = -q
            dom[0, 1] = q
            del q
        elif u.shape[1] > u.shape[0]:
            q = float(u.shape[1]) / float(u.shape[0])
            dom[1, 0] = -q
            dom[1, 1] = q
            del q
    else:
        if not isinstance(dom, _np.ndarray):
            if not (hasattr(dom, '__iter__') or hasattr(dom, '__array__')):
                raise TypeError('Domain must be a numpy array.')
            try:
                dom = _np.array(dom)
            except (TypeError, ValueError, AttributeError):
                raise TypeError('Domain must be a numpy array.')
        if not issubclass(dom.dtype.type, _numbers.Real):
            raise TypeError('Domain must be an real-valued array.')
        if dom.ndim != 2:
            raise ValueError('Domain must be a matrix (a 2-dimensional array).')
        if not dom.shape == (2, 2):
            raise ValueError('Domain must be a 2 x 2 array.')
        if _np.isnan(dom).any() or _np.isinf(dom).any():
            raise ValueError('Domain must not contain NaN or infinite values.')
        if not (dom[:, 0] < dom[:, 1]).all():
            raise ValueError('Domain must be sorted ascending on axis 1.')

    # Compute the domain.
    dom = tuple(
        _np.meshgrid(
            _np.linspace(dom[0, 0], dom[0, 1], num = u.shape[0], dtype = float),
            _np.linspace(dom[1, 0], dom[1, 1], num = u.shape[1], dtype = float)
        )
    )

    # Sanitize the parameter ax.
    if ax is None:
        ax = _plt.gca()
    elif not isinstance(ax, (_Axes, _Axes3D)):
        raise TypeError(
            'Axis must be of type matplotlib.axes.Axes or '
            'mpl_toolkits.mplot3d.Axes3D.'
        )

    # Sanitize the parameter how.
    if not (
        isinstance(how, _six.string_types) or
        isinstance(how, _six.text_type)
    ):
        raise TypeError('Plot type must be a string.')
    try:
        how = _copy.deepcopy(str(how))
    except (TypeError, ValueError, AttributeError):
        raise ValueError('Plot type must be a Python2 compatible string.')

    # Plot the plot.
    ax.__getattribute__(how)(dom[0], dom[1], u.T, *args, **kwargs)

    # Return the axis.
    return ax
