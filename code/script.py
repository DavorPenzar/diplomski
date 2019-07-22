# -*- coding: utf-8 -*-

"""
Useful functions.

Dependencies
------------
1.  Standard Python library:
    1.  `copy`,
    2.  `math`,
    3.  `numbers`,
    4.  `six`,
    5.  `warnings`,
2.  SciPy library:
    1.  `matplotlib`
        1.  `matplotlib.axes`,
        2.  `matplotlib.pyplot`,
    2.  `mpl_toolkits.mplot3d`,
    3.  `numpy`
        1.  `numpy.linalg`,
    4.  `scipy.sparse`
        1.  `scipy.sparse.linalg`.

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
from numpy.linalg import eig as _eig
from scipy.sparse.linalg import eigs as _eigs

def triang (x = 1, y = None, num = 50):
    """
    Compute the discretisation of a rectangle containing an open triangle.

    Parameters
    ----------
    x : real in range [0, +inf) or (3,) array_like of reals, optional
        Width or x-values of the vertices of the triangle (default is 1).  If a
        single floating point value is passed, it defines the width of the
        triangle.  If an array of three values is passed, each value defines the
        x-value of a vertix of the triangle.  Passing a single value `x` is the
        same as passing `[-x / 2, x / 2, 0]`.  If a single value is passed as
        the parameter `x` and `None` is passed as `y`, the value of `x` does not
        affect the resulting rectangle (probably except for extremely large or
        extremely small values, and definitely except for zero versus non-zero
        values), therefore the parameter `x` is optional.

    y : None or real in range [0, +inf) or (3,) array_like of reals, optional
        Height or y-values of the vertices of the triangle (default is `None`).
        If a single floating point value is passed, it defines the height of the
        triangle.  If an array of three values is passed, each value defines the
        y-value of a vertix of the triangle.  Passing a single value `y` is the
        same as passing `[-y / 2, -y / 2, y / 2]`.  If `None`,
        `sqrt(0.75) * abs(x[1] - x[0])` is used as the height of the
        triangle.

    num : int in range [2, +inf), optional
        The number of discretisation points used to discretise the wider
        dimension of the resulting rectangle (default is 50).

    Returns
    -------
    Omega : (M, N) array of booleans
        Array of trues and falses representing the discretisation of the
        rectangle containing the closure of the triangle.  The rectangle will
        stretch from `min(x)` to `max(x)` on its first dimension, and from
        `min(y)` to `max(y)` on its second dimension, and the equality
        `max(M, N) == num` will be true.  The values of Omega are defined as:
        `Omega[i, j] == True` if and only if the point at the intersection of
        the `i`-th discretisation line of the x-axis and the `j`-th
        discretisation line of the y-axis is inside the open triangle.  Note
        that the first dimension represents the x-axis.

    Raises
    ------
    TypeError
        If any of the parameters is not of a supported type or `dtype`, an
        exception of type `TypeError` is raised.

    ValueError
        If any of the parameters is not of a supported shape or value, an
        exception of type `ValueError` is raised.

    """

    # Define comparison objects used for ordering <, >, <= and >=.
    LT_check = lambda r, s : r < s
    GT_check = lambda r, s : r > s
    LE_check = lambda r, s : r <= s
    GE_check = lambda r, s : r >= s

    # Sanitise the parameter x.
    if isinstance(x, _np.ndarray):
        if x.size == 1:
            x = x.ravel()
            x = x.dtype.type(x[0])
    if hasattr(x, '__iter__') or hasattr(x, '__array__'):
        if not isinstance(x, _np.ndarray):
            try:
                x = _np.array(x)
            except (TypeError, ValueError, AttributeError):
                raise TypeError('x must be a scalar or an array.')
        if not issubclass(x.dtype.type, _numbers.Real):
            raise TypeError('x must be real.')
        x = x.astype(float)
        if not x.ndim == 1:
            raise ValueError('x must be a 1-dimensional array.')
        if not x.size == 3:
            raise ValueError('x must have exactly 3 elements.')
    else:
        if not isinstance(x, _numbers.Real):
            raise TypeError('Width must be real.')
        try:
            x = _copy.deepcopy(float(x))
        except (TypeError, ValueError, AttributeError):
            raise TypeError('Width must be of type float.')
        if _math.isnan(x) or _math.isinf(x):
            raise ValueError('Width must not be NaN or infinite.')
        if x < 0.0:
            raise ValueError('Width must not be negative')
        x = 0.5 * _np.array([-x, x, 0.0], dtype = float, order = 'F')
    if _np.isnan(x).any() or _np.isinf(x).any():
        raise ValueError('x must be non-NaN and finite.')

    # If y is None, define the heigt of the triangle.
    if y is None:
        y = _math.sqrt(0.75) * abs(float(x[1] - x[0]))

    # Sanitise the parameter y.
    if isinstance(y, _np.ndarray):
        if y.size == 1:
            y = y.ravel()
            y = y.dtype.type(y[0])
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
            raise ValueError('y must be a 1-dimensional array.')
        if not y.size == 3:
            raise ValueError('y must have exactly 3 elements.')
    else:
        if not isinstance(y, _numbers.Real):
            raise TypeError('Height must be real.')
        try:
            y = _copy.deepcopy(float(y))
        except (TypeError, ValueError, AttributeError):
            raise TypeError('Height must be of type float.')
        if _math.isnan(y) or _math.isinf(y):
            raise ValueError('Height must not be NaN or infinite.')
        if y < 0.0:
            raise ValueError('Height must be positive.')
        y = 0.5 * _np.array([-y, -y, y], dtype = float, order = 'F')
    if _np.isnan(y).any() or _np.isinf(y).any():
        raise ValueError('y must be non-NaN and finite.')

    # Sanitise the parameter num.
    if isinstance(num, _np.ndarray):
        if num.size == 1:
            num = num.ravel()
            num = num.dtype.type(num[0])
    if not isinstance(num, _numbers.Integral):
        raise TypeError('Number of discretisation points must be integral.')
    try:
        num = _copy.deepcopy(int(num))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Number of discretisation points must be of type int.')
    if num < 2:
        raise ValueError(
            'Number of discretisation points must be greater than 1.'
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

    # Discretise the rectangle.
    u = _np.linspace(x_min, x_max, num = Omega.shape[0])
    v = _np.linspace(y_min, y_max, num = Omega.shape[1])

    ##  The following code is derived from the formula generating the line
    ##  interpolating two different points on a plane.  For the points
    ##  (x_A, y_A) and (x_B, y_B), if x_B != x_A, the line is generated by the
    ##  explicit formula
    ##      y = (y_B - y_A) / (x_B - x_A) * (x - x_A) + y_A.
    ##  If x_B == x_A, then y_B != y_A (otherwise the points would not be
    ##  different) and the formula could be generated to define x for a fixed
    ##  value of y.  Either way, a point is below/to the left from the line if
    ##  the equality is replaced by <, and above/to the right it if the equality
    ##  is replaced by >.  Then, we multiply the inequality by (x_B - x_A) or
    ##  (y_B - y_A) depending on the case and eventualy get
    ##      (x_B - x_A) * (y - y_A) @ (y_B - y_A) * (x - x_A)
    ##  where @ is the corresponding sign among < and > (opposite the original
    ##  if x_B < x_A).  Knowing the coordinates of the points, the sign can be
    ##  inferred simply by checking which of the two is satisfied.

    # Using the objects LT_check and GT_check, define the comparison objects to
    # check if a point is inside the triangle.
    liner_check = [
        LT_check if LE_check(
            (x[1] - x[0]) * (y[2] - y[0]),
            (y[1] - y[0]) * (x[2] - x[0])
        ) else GT_check,
        LT_check if LE_check(
            (x[2] - x[1]) * (y[0] - y[1]),
            (y[2] - y[1]) * (x[0] - x[1])
        ) else GT_check,
        LT_check if LE_check(
            (x[0] - x[2]) * (y[1] - y[2]),
            (y[0] - y[2]) * (x[1] - x[2])
        ) else GT_check
    ]

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
    I = list(range(int(u.size)))
    J = list(range(int(v.size)))
    for i in I:
        for j in J:
            Omega[i, j] = _np.bool8(
                liner_check[0](v_liner[0, j], u_liner[0, i]) and
                liner_check[1](v_liner[1, j], u_liner[1, i]) and
                liner_check[2](v_liner[2, j], u_liner[2, i])
            )
        try:
            del j
        except (NameError, UnboundLocalError):
            pass
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    del I
    del J

    # Free the memory.
    del u_liner
    del v_liner
    del u
    del v
    del a
    del b
    del x_min
    del x_max
    del y_min
    del y_max

    # Return the rectangle.
    return Omega

def ellips (a = 1, b = None, num = 50):
    """
    Compute the discretisation of a rectangle containing an open ellipsis.

    Parameters
    ----------
    a : real in range [0, +inf), optional
        The width of the ellipsis (default is 1).  If a value is passed as the
        parameter `a` and `None` is passed as `b`, the value of `a` does not
        affect the resulting rectangle (probably except for extremely large or
        extremely small values, and definitely except for zero versus non-zero
        values), therefore the parameter `a` is optional.

    b : None or real in range [0, +inf), optional
        The height of the ellipsis (default is `None`).  If `None`, height will
        be equal to width.

    num : int in range [2, +inf), optional
        The number of discretisation points used to discretise the wider
        dimension of the resulting rectangle (default is 50).

    Returns
    -------
    Omega : (M, N) array of booleans
        Array of trues and falses representing the discretisation of the
        rectangle containing the closure of the ellipsis.  The rectangle will
        stretch from `-a` to `a` on its first dimension, and from `-b` to `b` on
        its second dimension, and the equality `max(M, N) == num` will be true.
        The values of `Omega` are defined as: `Omega[i, j] == True` if and only
        if the point at the intersection of the `i`-th discretisation line of
        the x-axis and the `j`-th discretisation line of the y-axis is inside
        the open ellipsis.  Note that the first dimension represents the x-axis.

    Raises
    ------
    TypeError
        If any of the parameters is not of a supported type or `dtype`, an
        exception of type `TypeError` is raised.

    ValueError
        If any of the parameters is not of a supported shape or value, an
        exception of type `ValueError` is raised.

    """

    # Sanitise the parameter a.
    if isinstance(a, _np.ndarray):
        if a.size == 1:
            a = a.ravel()
            a = a.dtype.type(a[0])
    if not isinstance(a, _numbers.Real):
        raise TypeError('a must be real.')
    try:
        a = _copy.deepcopy(float(a))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('a must be of type float.')
    if _math.isnan(a) or _math.isinf(a):
        raise ValueError('a must be non-NaN and finite.')
    if a < 0:
        raise ValueError('a must not be negative.')

    # If b is None, define the height of the ellipsis.
    if b is None:
        b = a

    # Sanitise the parameter b.
    if isinstance(b, _np.ndarray):
        if b.size == 1:
            b = b.ravel()
            b = b.dtype.type(b[0])
    if not isinstance(b, _numbers.Real):
        raise TypeError('b must be real.')
    try:
        b = _copy.deepcopy(float(b))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('b must be of type float.')
    if _math.isnan(b) or _math.isinf(b):
        raise ValueError('b must be non-NaN and finite.')
    if b < 0:
        raise ValueError('b must not be negative.')

    # Sanitise the parameter num.
    if isinstance(num, _np.ndarray):
        if num.size == 1:
            num = num.ravel()
            num = num.dtype.type(num[0])
    if not isinstance(num, _numbers.Integral):
        raise TypeError('Number of discretisation points must be integral.')
    try:
        num = _copy.deepcopy(int(num))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Number of discretisation points must be of type int.')
    if num < 2:
        raise ValueError(
            'Number of discretisation points must be greater than 1.'
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

    # Discretise the rectangle.
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
    I = list(range(int(u.size)))
    J = list(range(int(v.size)))
    for i in I:
        for j in J:
            Omega[i, j] = _np.bool8(b2 * u2[i] + a2 * v2[j] < ab2)
        try:
            del j
        except (NameError, UnboundLocalError):
            pass
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    del I
    del J

    # Free the memory.
    del u2
    del v2
    del a2
    del b2
    del ab2
    del u
    del v

    # Return the rectangle.
    return Omega

def eigenfunc (Omega, k = 1, as_sparse = False, h = None):
    """
    Compute the Dirichlet Laplacian eigenfunctions and eigenvalues on Omega.

    Parameters
    ----------
    Omega : (M, N) array_like of booleans
        Discretisation of the rectangle containig the domain.  `Omega` must be a
        2-dimensional non-empty array (`Omega.size > 0`) of integral `dtype` and
        must contain only the values 0 (`False`) and 1 (`True`) such that at
        least one element is non-zero, and no element in the first or the last
        row and in the first or the last column is 1.  As such, `Omega`
        represents an equidistant discretisation of a rectangle containing the
        2-dimensional domain at which the Dirichlet Laplacian eigenvalues and
        eigenfunctions are computed: `Omega[i, j] == 1` if and only if the point
        at the intersection of the `i`-th discretisation line of the x-axis and
        the `j`-th discretisation line of the y-axis is inside the domain.  Note
        that the first dimension represents the x-axis.

    k : int in range [1, Omega.sum()], optional
        Number of Dirichlet Laplacian eigenfunctions and eigenvectors to find at
        the most (default is 1).  The first `k` non-zero real eigenvalues of the
        least magnitude are found and their corresponding real-valued
        eigenfunctions (if they are also real-valued).

    as_sparse : boolean, optional
        If `True`, the eigenvalues and the eigenfunctions are computed using a
        matrix of class `scipy.sparse.spmatrix` and by calling the function
        `scipy.sparse.linalg.eigs`; otherwise the matrix is represented using a
        regular `numpy.ndarray` and the function `numpy.linalg.eig` (default is
        `False`).  The matrix is of shape `(Omega.size, Omega.size)` and at the
        most only `5 * Omega.size` of its elements are non-zero, therefore, if
        `Omega` is large, constructing the `numpy.ndarray` might raise a
        `MemoryError` while constructing the `scipy.sparse.spmatrix` should not.
        On the other hand, the function `numpy.linalg.eig` is much faster than
        the function `scipy.sparse.linalg.eigs`.

    h : None or real in range (0, +inf), optional
        The step of the discretisation (default is `None`).  The Laplacian of
        the function `u` (discretised in the way analogous to the way the domain
        is discretised) at `Omega[i, j]` such that `Omega[i, j] == True` is
        approximated as
        `(u[i + 1, j] + u[i, j + 1] - 4 * u[i, j] + u[i - 1, j] + u[i, j - 1]) / h ** 2`;
        however, if `h` is `None`, only the numerator is used for the
        approximation (as if `h == 1`).  Changing this parameter can result in
        forcing to include some of the eigenvalues if `h` is close to 0 (include
        eigenvalues that would otherwise be close to 0) or to ignore some if `h`
        is large (exclude eigenvalues of small magnitude).

    Returns
    -------
    l : float or (k,) array of floats
        Array containing the real Dirichlet Laplacian eigenvalues sorted
        ascendingly by magnitude.  If `k == 1`, only the first value is returned
        as a scalar (not an array).

    u : (M, N) array of floats or (k, M, N) array of floats
        Array containing the discrete approximations of the real-valued
        Dirichlet Laplacian eigenfunctions.  The "function" `u[i]` corresponds
        to the eigenvalue `l[i]`.  It is guarranteed that all the "functions'"
        values are in the range [-1, 1] and that each "function" is a constant
        zero-valued "function" or it reaches 1 on the domain.  If `k == 1`, only
        the first eigenfunction is returned as a 2-dimensional array (not an
        array of shape `(1, M, N)`).

    Raises
    ------
    TypeError
        If any of the parameters is not of a supported type or `dtype`, an
        exception of type `TypeError` is raised.

    ValueError
        If any of the parameters is not of a supported shape or value, an
        exception of type `ValueError` is raised.

    RuntimeError
        If no real non-zero eigenvalues corresponding to real-valued
        eigenfunctions are found, an exception of type `RuntimeError` is raised.

    MemoryError
        If `as_sparse` is `False`, an exception of type `MemoryError` might be
        raised --- it is not caught.

    other
        Exceptions raised by the functions `numpy.linalg.eig` or
        `scipy.sparse.linalg.eigs` are not caught.

    """

    # Sanitise the parameter Omega.
    if not isinstance(Omega, _np.ndarray):
        if not (hasattr(Omega, '__iter__') or hasattr(Omega, '__array__')):
            raise TypeError('Omega must be an array.')
        try:
            Omega = _np.array(Omega)
        except (TypeError, ValueError, AttributeError):
            raise TypeError('Omega must be an array.')
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
    if isinstance(Omega, _np.matrix):
        Omega = Omega.A
    Omega = Omega.astype(_np.bool8)

    # Sanitise the parameter k.
    if isinstance(k, _np.ndarray):
        if k.size == 1:
            k = k.ravel()
            k = k.dtype.type(k[0])
    if not isinstance(k, _numbers.Integral):
        raise TypeError('Number of eigenvalues/-functions must be integral.')
    try:
        k = _copy.deepcopy(int(k))
    except (TypeError, ValueError, AttributeError):
        raise ValueError(
            'Number of eigenvalues/-functions must be of type int.'
        )
    if k <= 0:
        raise ValueError(
            'Number of eigenvalues/-functions must be greater than 0.'
        )
    if k > Omega.sum(dtype = int):
        raise ValueError(
            'Number of eigenvalues/-functions must not be larger than the '
            'number of points in the domain.'
        )

    # Sanitise the parameter as_sparse.
    if isinstance(as_sparse, _np.ndarray):
        if as_sparse.size == 1:
            as_sparse = as_sparse.ravel()
            as_sparse = as_sparse.dtype.type(as_sparse[0])
    if not isinstance(
        as_sparse,
        (_numbers.Integral, int, bool, _np.bool, _np.bool8, _np.bool_)
    ):
        raise TypeError('Parameter as_sparse must be boolean.')
    try:
        if as_sparse not in {0, False, 1, True}:
            raise ValueError('Parameter as_sparse must be False or True.')
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter as_sparse must be False or True.')
    try:
        as_sparse = _copy.deepcopy(bool(as_sparse))
    except (TypeError, ValueError, AttributeError):
        raise ValueError('Parameter as_sparse must be of type bool.')

    # Sanitise the parameter h.
    if isinstance(h, _np.ndarray):
        if h.size == 1:
            h = h.ravel()
            h = h.dtype.type(h[0])
    if h is not None:
        if not isinstance(h, _numbers.Real):
            raise TypeError('Step of discretisation must be real.')
        try:
            h = _copy.deepcopy(float(h))
        except (TypeError, ValueError, AttributeError):
            raise ValueError('Step of discretisation must be of type float.')
        if _math.isnan(h) or _math.isinf(h):
            raise ValueError(
                'Step of discretisation must not be NaN or infinite.'
            )
        if h <= 0:
            raise ValueError('Step of discretisation must be greater than 0.')

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
    for i in I:
        for j in J:
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

    # Convert the Laplacian matrix to Compressed Sparse Column matrix dividing
    # it by h^2 if necesarry.
    D = (D if h is None else D / h ** 2).tocsc()

    # Compute the inverse of Omega.
    Omega_inv = ~Omega

    # Compute the eigenvalues and eigenvectors of the matrix D.
    l = None
    u = None
    if as_sparse:
        l, u = _eigs(-D, Omega_inv.sum(dtype = int) + k)
    else:
        l, u = _eig(-D.todense(order = 'F'))

    # Convert l to a 1-dimensional array and u to a 2-dimensional array.
    l = l.ravel().copy(order = 'F')
    if isinstance(u, _np.matrix):
        u = u.A
    if u.ndim <= 1:
        u = u.reshape((u.size, 1))
    u = u.copy(order = 'F')

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
        I = I[~_np.isclose(1, 1 + l_abs[I])].copy(order = 'F')
        l_abs[_np.isclose(1, 1 + l_abs)] = 1
        u_abs[_np.isclose(1, 1 + u_abs)] = 1
        if not I.size:
            break
        I = I[_np.isclose(1, 1 + l[I].imag / l_abs[I])].copy(order = 'F')
        if not I.size:
            break
        I = I[
            _np.isclose(1, 1 + u[:, I].imag / u_abs[:, I]).all(axis = 0).ravel()
        ].copy(order = 'F')
        break
    del l_abs
    del u_abs

    # If no real eigenvalue or eigenvector satisfying were found, raise an
    # exception of type RuntimeError.  Otherwise, if I has more than k entries,
    # set it to hold only its first k elements.
    if not I.size:
        raise RuntimeError(
            'No real eigenvalues and real eigenfunctions were found.'
        )
    elif I.size > k:
        I = I[:k].copy(order = 'F')

    # Convert, reorder and filter the eigenvalues and eigenvectors.
    l = _np.array(l[I].real, dtype = float, copy = True, order = 'F')
    u = _np.array(u[:, I].real, dtype = float, copy = True, order = 'F')

    # Free the memory.
    del I

    # Reshape u to a three-dimensional array.
    u = _np.array(
        tuple(u[:, i].reshape(Omega.shape) for i in range(int(u.shape[1]))),
        dtype = float,
        order = 'C'
    )
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    if u.ndim == 2:
        u = u.reshape((1, u.shape[0], u.shape[1]))._copy(order = 'C')

    # Free the memory.
    del Omega

    # Assure no eigenfunction evaluates to a non-zero value outside of Omega,
    # that the non-zero eigenfunctions' range is the subset of the interval
    # [-1, 1] and that the non-zero eigenfunctions reach 1.
    with _warnings.catch_warnings():
        _warnings.filterwarnings('error')
        with _np.errstate(divide = 'raise'):
            for i in range(int(u.shape[0])):
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

    # If only the first eigenvalue and function were to be found, convert
    # variables l and u.
    if k == 1:
        l = _copy.deepcopy(float(l[0]))
        u = u[0].copy(order = 'F')

    # Return the eigenvalue(s) and eigenfunction(s).
    return (l, u)

def show_2d_func (u, dom = None, ax = None, how = 'contourf', *args, **kwargs):
    """
    Plot a real function on a 2-dimensional rectangular domain.

    Parameters
    ----------
    u : (M, N) array_like of reals or (M, N) array_like of booleans
        Representation of a real function on a discrete rectangle.  Note that
        the first dimension represents the x-axis.

    dom : None or (2, 2) array_like of reals, optional
        Boundaries of the domain (default is `None`).  The function `u` is
        plotted as the funtion on the rectangle defined by the vertices
        (`dom[0, 0]`, `dom[1, 0]`), (`dom[0, 1]`, `dom[1, 0]`),
        (`dom[0, 1]`, `dom[1, 1]`) and (`dom[0, 0]`, `dom[1, 1]`).  The array
        must be sorted ascendingly on the axis 1.  If `None`, the vertices are
        computed such that the ratio of the rectangle's width by the rectangle's
        height is equal to `M / N` and that the narrower dimension of the
        rectangle stretches from -1 to 1.

    ax : None or matplotlib.axes.Axes or mpl_toolkits.mplot3d.Axes3D, optional
        Axis at which the surface is plotted (default is `None`).  If `None`,
        `matplotlib.pyplot.gca()` is used.

    how : str, optional
        Type of the plot (default is `'contourf'`).  The function is plotted by
        calling `ax.__getattribute__(how)(...)`.

    *args, **kwargs
        Additional arguments passed to `ax.__getattribute__(how)(...)` after the
        first three arguments (`X`, `Y` and `Z` data).

    Returns
    -------
    ax : matplotlib.axes.Axes or mpl_toolkits.mplot3d.Axes3D
        Axis at which the function is plotted.

    Raises
    ------
    TTypeError
        If any of the parameters is not of a supported type or `dtype`, an
        exception of type `TypeError` is raised.

    ValueError
        If any of the parameters is not of a supported shape or value, an
        exception of type `ValueError` is raised.  If any of the dimensions of
        the array `u` is (strictly) less than 2, an exception of type
        `ValueError` is raised.  If `dom` is an array, it must be sorted
        ascendingly on axis 1 (its rows must be strictly increasing sequences);
        otherwise an exception of type `ValueError` is raised.

    other
        If the command `ax.__getattribute__(how)(X, Y, Z, *args, **kwargs)`
        (where the shapes and values of `X`, `Y` and `Z` are valid if the plot
        type defined by the parameter `how` accepts the same arguments as
        `matplotlib.axes.Axes.contourf`) raises an exception, it is not caught.
        Not even `AttributeError` is caught if `how` is not a valid plot type.

    """

    # Sanitise the parameter u.
    if not isinstance(u, _np.ndarray):
        if not (hasattr(u, '__iter__') or hasattr(u, '__array__')):
            raise TypeError('u must be an array.')
        try:
            u = _np.array(u)
        except (TypeError, ValueError, AttributeError):
            raise TypeError('u must be an array.')
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
    if isinstance(u, _np.matrix):
        u = u.A

    # Sanitise the parameter dom.
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
            raise ValueError('Domain must be sorted ascendingly on axis 1.')
        if isinstance(dom, _np.matrix):
            dom = dom.A

    # Compute the domain.
    dom = tuple(
        _np.meshgrid(
            _np.linspace(dom[0, 0], dom[0, 1], num = u.shape[0], dtype = float),
            _np.linspace(dom[1, 0], dom[1, 1], num = u.shape[1], dtype = float),
            indexing = 'ij'
        )
    )

    # Sanitise the parameter ax.
    if ax is None:
        ax = _plt.gca()
    else:
        if isinstance(ax, _np.ndarray):
            if ax.size == 1:
                ax = ax.ravel()
                ax = ax.dtype.type(ax[0])
        if not isinstance(ax, (_Axes, _Axes3D)):
            raise TypeError(
                'Axis must be of type matplotlib.axes.Axes or '
                'mpl_toolkits.mplot3d.Axes3D.'
            )

    # Sanitise the parameter how.
    if isinstance(how, _np.ndarray):
        if how.size == 1:
            how = how.ravel()
            how = how.dtype.type(how[0])
    if not (
        isinstance(how, _six.string_types) or
        isinstance(how, _six.text_type) or
        isinstance(how, _six.binary_type) or
        isinstance(
            how,
            (
                _np.str,
                _np.str_,
                _np.unicode,
                _np.unicode_,
                _np.string_,
                _np.bytes_
            )
        )
    ):
        raise TypeError('Plot type must be a string.')
    try:
        how = _copy.deepcopy(str(how))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Plot type must be of type str.')

    # Plot the plot.
    ax.__getattribute__(how)(dom[0], dom[1], u, *args, **kwargs)

    # Return the axis.
    return ax
