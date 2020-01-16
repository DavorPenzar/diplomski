"""
Personal useful functions for exploring and extracting data in dataframes.

This file is part of Davor Penzar's master thesis programing.

"""

# -*- coding: utf-8 -*-

# Import standard library.
import copy as _copy
import csv as _csv
import functools as _functools
import inspect as _inspect
import math as _math
import numbers as _numbers
import six as _six
import types as _types
from collections.abc import Iterable as _Iterable
from collections.abc import Sequence as _Sequence

# Import SciPy packages.
import numpy as _np
import pandas as _pd
from numpy.random import choice as _choice
from pandas.api.types import is_numeric_dtype as _is_numeric_dtype
from pandas.core.frame import DataFrame as _DataFrame
from pandas.core.series import Series as _Series

def read_tsv (
    filepath_or_buffer,
    header = False,
    index = False,
    as_str = False,
    **kwargs
):
    """
    Read a TSV dataframe of format in the master thesis project.

    This function is a wrapper around the `pandas.read_csv` function.  The
    function actually returns
        >>> pandas.read_csv(filepath_or_buffer, sep = "\t", header = 0 if header else None, index_col = 0 if index else None, dtype = str if as_str else float, **kwargs)

    Parameters
    ==========
    filepath_or_buffer
        Source of the TSV dataframe.  The parameter is not sanitised or checked,
        it is passed raw to the `pandas.read_csv` function as the first
        positional argument.

    header : boolean, optional
        True if the first row shoul be read as the header row, false otherwise
        (default is false).

    index : boolean
        True if the first column should be read as the index column, false
        otherwise (default is false).

    as_str : boolean, optional
        True if data should be read as string, false if data should be read as
        real floating point numbers (default is false).

    kwargs
        Additional keyword arguments passed to the `pandas.read_csv` function.
        This arguments are not checked or sanitised, they are passed raw to the
        `pandas.read_csv` function.

    Returns
    =======
    DataFrame
        The dataframe that is read from the source.

    Raises
    ======
    TypeError
        Parameter `header` is not boolean.  Parameter `index` is not boolean.
        Parameter `as_str` is not boolean.

    ValueError
        Parameter `header` is not true or false.  Parameter `index` is not true
        or false.  Parameter `as_str` is not true or false.

    others
        Exceptions thrown by the `pandas.read_csv` function are not caught.

    """

    # Sanitise the parameter `header`.
    if hasattr(header, '__iter__') or hasattr(header, '__array__'):
        if not isinstance(header, _np.ndarray):
            try:
                header = _np.array(header)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(header, _np.ndarray):
        if header.size == 1 or header.shape == tuple():
            header = header.ravel()
            if not (header.size == 1):
                raise TypeError('Parameter `header` must be boolean.')
            try:
                header = header.dtype.type(header[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `header` must be boolean.')
    if not (
        isinstance(header, _six.integer_types) or
        isinstance(
            header,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `header` must be boolean.')
    if not (header == False or header == 0 or header == True or header == 1):
        raise ValueError('Parameter `header` must be true or false.')
    try:
        header = _copy.deepcopy(bool(header))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `header` must be of type `bool`.')

    # Sanitise the parameter `index`.
    if hasattr(index, '__iter__') or hasattr(index, '__array__'):
        if not isinstance(index, _np.ndarray):
            try:
                index = _np.array(index)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(index, _np.ndarray):
        if index.size == 1 or index.shape == tuple():
            index = index.ravel()
            if not (index.size == 1):
                raise TypeError('Parameter `index` must be boolean.')
            try:
                index = index.dtype.type(index[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `index` must be boolean.')
    if not (
        isinstance(index, _six.integer_types) or
        isinstance(
            index,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `index` must be boolean.')
    if not (index == False or index == 0 or index == True or index == 1):
        raise ValueError('Parameter `index` must be true or false.')
    try:
        index = _copy.deepcopy(bool(index))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `index` must be of type `bool`.')

    # Sanitise the parameter `as_str`.
    if hasattr(as_str, '__iter__') or hasattr(as_str, '__array__'):
        if not isinstance(as_str, _np.ndarray):
            try:
                as_str = _np.array(as_str)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(as_str, _np.ndarray):
        if as_str.size == 1 or as_str.shape == tuple():
            as_str = as_str.ravel()
            if not (as_str.size == 1):
                raise TypeError('Parameter `as_str` must be boolean.')
            try:
                as_str = as_str.dtype.type(as_str[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `as_str` must be boolean.')
    if not (
        isinstance(as_str, _six.integer_types) or
        isinstance(
            as_str,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `as_str` must be boolean.')
    if not (as_str == False or as_str == 0 or as_str == True or as_str == 1):
        raise ValueError('Parameter `as_str` must be true or false.')
    try:
        as_str = _copy.deepcopy(bool(as_str))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `as_str` must be of type `bool`.')

    # Read the TSV and return the read dataframe.
    return _pd.read_csv(
        filepath_or_buffer,
        sep = "\t",
        header = 0 if header else None,
        index_col = 0 if index else None,
        dtype = str if as_str else float,
        **kwargs
    )

def to_tsv (df, filepath_or_buffer, header = False, index = False, **kwargs):
    """
    Save a dataframe to a TSV dataframe of format in the master thesis project.

    This function is a wrapper around the `pandas.DataFrame.to_csv` method.
    The function actually returns
        >>> df.to_csv(filepath_or_buffer, sep = "\t", float_format = "%.8f", quoting = csv.QUOTE_NONE, header = header, index = index, **kwargs)

    Parameters
    ==========
    filepath_or_buffer
        Destination of the TSV dataframe.  The parameter is not sanitised or
        checked, it is passed raw to the `pandas.read_csv` function as the first
        positional argument.

    header : boolean, optional
        True if header should be printed as the first row, false otherwise
        (default is false).

    index : boolean
        True if index should be printed as the first column, false otherwise
        (default is false).

    kwargs
        Additional keyword arguments passed to the `pandas.DataFrame.to_csv`
        method.  This arguments are not checked or sanitised, they are passed
        raw to the `pandas.DataFrame.to_csv` method.

    Returns
    =======
    None or str
        The dataframe that is read from the source.

    Raises
    ======
    TypeError
        Parameter `header` is not boolean.  Parameter `index` is not boolean.

    ValueError
        Parameter `header` is not true or false.  Parameter `index` is not true
        or false.

    others
        Exceptions thrown by the `pandas.DataFrame.to_csv` method are not
        caught.

    """

    # Sanitise the parameter `df`.
    if isinstance(df, _Series):
        df = df.to_frame()
    if not isinstance(df, _DataFrame):
        raise TypeError('Parameter `df` must be of type `pandas.DataFrame`.')

    # Sanitise the parameter `header`.
    if hasattr(header, '__iter__') or hasattr(header, '__array__'):
        if not isinstance(header, _np.ndarray):
            try:
                header = _np.array(header)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(header, _np.ndarray):
        if header.size == 1 or header.shape == tuple():
            header = header.ravel()
            if not (header.size == 1):
                raise TypeError('Parameter `header` must be boolean.')
            try:
                header = header.dtype.type(header[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `header` must be boolean.')
    if not (
        isinstance(header, _six.integer_types) or
        isinstance(
            header,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `header` must be boolean.')
    if not (header == False or header == 0 or header == True or header == 1):
        raise ValueError('Parameter `header` must be true or false.')
    try:
        header = _copy.deepcopy(bool(header))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `header` must be of type `bool`.')

    # Sanitise the parameter `index`.
    if hasattr(index, '__iter__') or hasattr(index, '__array__'):
        if not isinstance(index, _np.ndarray):
            try:
                index = _np.array(index)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(index, _np.ndarray):
        if index.size == 1 or index.shape == tuple():
            index = index.ravel()
            if not (index.size == 1):
                raise TypeError('Parameter `index` must be boolean.')
            try:
                index = index.dtype.type(index[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `index` must be boolean.')
    if not (
        isinstance(index, _six.integer_types) or
        isinstance(
            index,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `index` must be boolean.')
    if not (index == False or index == 0 or index == True or index == 1):
        raise ValueError('Parameter `index` must be true or false.')
    try:
        index = _copy.deepcopy(bool(index))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `index` must be of type `bool`.')

    # Save the dataframe to a TSV and return the returned value.
    return df.to_csv(
        filepath_or_buffer,
        sep = "\t",
        float_format = "%.8f",
        quoting = _csv.QUOTE_NONE,
        header = header,
        index = index,
        **kwargs
    )

# Define the function to generate rainbows.
def rainbow (n, start = 0.0, end = None):
    """
    Generate a spectrum of colours equidistant in the hue wheel.

    Parameters
    ==========
    n : int in range [0, +inf)
        Number of colours to generate.

    start : float in range [0, 360)
        Position in the hue wheel to start the rainbow from (default is 0).  The
        red is at 0 (and 360), the green is at 120 and the blue is at 240.

    end : None or float in range [0, 360)
        Position in the hue wheel where the (`n` + 1)-th colour would be
        (defualt is `None`).  The red is at 0 (and 360), the green is at 120 and
        the blue is at 240.  If `None`, the (`n` + 1)-th colour would be the
        same as the first colour.  Note that if the end is specified, a complete
        circle will not be made, not even with the (`n` + 1)-th colour.  Hence
        setting the parameter `end` to the same value as the parameter `start`
        will result in all colours being the same---if you want the last colour
        to be distant from the first colour as all the other neighbouring
        colours are, set the parameter `end` to `None`.

    Returns
    =======
    (n, 3) array of floats in range [0, 1]
        RGB representation of colours.  Each row represents a colour as RGB
        values in the range [0, 1].  The first colour corresponds to the colour
        set by the value `start` (red by default) and then the colours proceed
        equidistantly in the hue wheel of increasing hue value (reset to 0
        instead of reaching 360).

    Raises
    ======
    TypeError
        Parameter `n` is not an integer.  Parameter `start` is not a real
        number.  Parameter `end` is not `None` or a real number.

    ValueError
        Parameter `n` is not in range [0, +inf).  Parameter `start` is not in
        range [0, 360).  Parameter `end` is not in range [0, 360).

    """

    # Sanitise the parameter `n`.
    if hasattr(n, '__iter__') or hasattr(n, '__array__'):
        if not isinstance(n, _np.ndarray):
            try:
                n = _np.array(n)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(n, _np.ndarray):
        if n.size == 1 or n.shape == tuple():
            n = n.ravel()
            if not (n.size == 1):
                raise TypeError('Number of colours must be integral.')
            try:
                n = n.dtype.type(n[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Number of colours must be integral.')
    if not (
            isinstance(n, _six.integer_types) or
            isinstance(n, (_np.integer, _numbers.Integral))
    ) or isinstance(n, (bool, _np.bool_)):
        raise TypeError('Number of colours must be integral.')
    try:
        n = _copy.deepcopy(int(n))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Number of colours must be of type `int`.')
    if _math.isnan(n) or _math.isinf(n):
        raise ValueError('Number of colours must be finite and non-NaN.')
    if n < 0:
        raise ValueError('Number of colours must be non-negative.')

    # Sanitise the parameter `start`.
    if hasattr(start, '__iter__') or hasattr(start, '__array__'):
        if not isinstance(start, _np.ndarray):
            try:
                start = _np.array(start)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(start, _np.ndarray):
        if start.size == 1 or start.shape == tuple():
            start = start.ravel()
            if not (start.size == 1):
                raise TypeError('Start position must be a real number.')
            try:
                start = start.dtype.type(start[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Start position must be a real number.')
    if (
        not (
            isinstance(
                start,
                (
                    float,
                    _np.integer,
                    _np.floating,
                    _numbers.Integral,
                    _numbers.Rational,
                    _numbers.Real
                )
            ) or isinstance(start, _six.integer_types)
        ) or isinstance(start, (bool, _np.bool_))
    ):
        raise TypeError('Start position must be a real number.')
    try:
        start = _copy.deepcopy(float(start))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Start position must be of type `float`.')
    if _math.isnan(start) or _math.isinf(start):
        raise ValueError('Start position must be finite and non-NaN.')
    if start < 0.0 or not (start < 360.0):
        raise ValueError('Start position must be in range [0, 360).')
    if start == 0.0:
        start = 0.0

    # Sanitise the parameter `end`.
    if hasattr(end, '__iter__') or hasattr(end, '__array__'):
        if not isinstance(end, _np.ndarray):
            try:
                end = _np.array(end)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(end, _np.ndarray):
        if end.size == 1 or end.shape == tuple():
            end = end.ravel()
            if not (end.size == 1):
                raise TypeError('End position must be a real number.')
            try:
                end = end.dtype.type(end[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('End position must be a real number.')
    if end is not None:
        if (
            not (
                isinstance(
                    end,
                    (
                        float,
                        _np.integer,
                        _np.floating,
                        _numbers.Integral,
                        _numbers.Rational,
                        _numbers.Real
                    )
                ) or isinstance(end, _six.integer_types)
            ) or isinstance(end, (bool, _np.bool_))
        ):
            raise TypeError('End position must be a real number.')
        try:
            end = _copy.deepcopy(float(end))
        except (TypeError, ValueError, AttributeError):
            raise TypeError('End position must be of type `float`.')
        if _math.isnan(end) or _math.isinf(end):
            raise ValueError('End position must be finite and non-NaN.')
        if end < 0.0 or not (end < 360.0):
            raise ValueError('End position must be in range [0, 360).')
        if end == 0.0:
            end = 0.0

    # Initialise the (relative) length of the used part of the hue wheel to
    # `None`. Length 6 means that the whole wheel is used (with `n` + 1
    # colours).
    length = None

    # Comput the length of the used part of the wheel.
    if end is None:
        length = 6.0
    else:
        if end < start:
            length = (360.0 - start + end) / 60.0
        else:
            length = (end - start) / 60.0

    # Free the memroy:
    del end

    # Divide the parameter `start` by 60.
    start /= 60.0

    # Initialise the array of colours to zeros.  The array is of shape (`n`, 3)
    # so that `col[i]` defines the `i`-th colour in the RGB format (in the range
    # [0, 1]).
    col = _np.zeros((n, 3), dtype = float, order = 'F')

    # Compute the colours.

    # Get the number of colours increased by 1 as `float`.
    C = _copy.deepcopy(float(n + 1))

    # Iterate over colours.
    for i in range(n):
        # Get the position of the current colour in spectrum (circular, from 0
        # to 6).
        c = start + length * float(i) / C
        if not (c < 6.0):
            c -= 6.0

        # Set the RGB values of the current colour according to the position
        # `c`.
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
    try:
        del i
    except (NameError, UnboundLocalError):
        pass

    # Free the memory.
    del C

    # Return the generated rainbow.
    return col

# Define the function to extract subdataframes.
def generate_subdf (
    dfs,
    n,
    ok = 1.0e-3,
    N_ITER = 1000,
    rtol = 1.0e-5,
    atol = 1.0e-8,
    err_f = max,
    compl = False,
    return_error = False
):
    """
    Extract subdataframes from dataframes.

    Parameters
    ==========
    dfs : iterable of DataFrames
        Dataframes from which to extract subdataframes.  This parameter may even
        be a generator object.

    n : int in range [0, +inf)
        Number of rows to extract into each subdataframe.  If an original
        dataframe has less than `n` rows, the complete dataframe is copied.

    ok : float in range [0, +inf), optional
        Supremum of allowed relative standard deviation error (default is
        1.0e-3).  If a subdataframe is extracted from an original dataframe and
        the computed total standard deviation error on all columns (see
        parameters `rtol`, `atol` and `err_f`) is strictly less than `ok`, the
        subdataframe is accepted and the algorithm proceeds to the next
        dataframe (or ends if the dataframe was the last one).  If the parameter
        `compl` is true, the complement's standard deviation error must also be
        strictly less than `ok` (see parameter `compl`).

    N_ITER : int in range [1, +inf), optional
        Maximal number of iterations to generate each subdataframe (default is
        1000).  If no subdataframe was accepted in `N_ITER` tries (see parameter
        `ok`), the one with the least total standard deviation error on all
        columns (see parameters `rtol`, `atol` and `err_f`) is used.  If the
        parameter `compl` is true, the complement's standard deviation error
        also tries to be minimalised in `N_ITER` iterations (see parameter
        `compl`).

    rtol : float in range [0, +inf), optional
        Relative tolerance parameter for ignoring standard deviations (see
        Notes) (default is 1.0e-5).

    atol : float in range [0, +inf), optional
        Relative tolerance parameter for ignoring standard deviations (see
        Notes) (default is 1.0e-8).

    err_f : function, optional
        Function to compute the total standard deviation error on all columns
        (default is `max`).  A standard deviation error on a subdataframe
        `subdf` of an original dataframe `df` on the column `c` is computed as
        `abs(subdf[c].std(ddof = 1) / df[c].std(ddof = 1) - 1.0)`.  This is
        computed on all columns (except the ignored ones; see parameters `rtol`,
        `atol` and Notes) and the results are joint in a tuple.  The tuple is
        then passed to the `err_f` function to compute the total standard
        deviation error.  The function should return a `float` or an object
        convertible to a `float`.

    compl : boolean, optional
        If true, the standard deviation error of the subdataframe's complement
        is also taken into consideration---the total standard deviation error is
        the maximum of the two standard deviation errors (the subdataframe's and
        its complement's) (default is false).  The standard deviation error on
        the subdataframe's complement is computed analogously to the
        subdataframe's standard deviation error.

    return_error : boolean, optional
        If true, the tuple of standard deviation errors of subdataframes is
        returned (default is false).

    Returns
    =======
    subdf : tuple of DataFrames
        Tuple of the accepted subdataframes.  The subdataframes are given in the
        same order as the original dataframes were.  Row indices in
        subdataframes are preserved from the original dataframes.

    E : tuple of floats in range [0, +inf)
        Total standard deviation errors on accepted subdataframes.  The errors
        are given in the same order as generated subdataframes/original
        dataframes.  If the parameter `compl` is true, the i-th value in `E` is
        the maximum of the total standard deviation error on the i-th
        subdataframe and the i-th subdataframe's complement.  This is returned
        only if the parameter `return_error` is true.

    Raises
    ======
    TypeError
        Parameter `dfs` is not iterable.  Parameter `n` is not an integer.
        Parameter `ok` is not a real number.  Parameter `N_ITER` is not an
        integer.  Parameter `rtol` is not a real number.  Parameter `atol` is
        not a real number.  Parameter `err_f` is not a function.  Parameter
        `compl` is not boolean.  Parameter `return_error` is not boolean.
        Length of the parameter `dfs` (if it has length) is not convertible to
        `int`.  A dataframe in `dfs` is not a Pandas dataframe.

    ValueError
        Parameter `n` is not in range [0, +inf).  Parameter `ok` is not in range
        [0, +inf).  Parameter N_ITER is not in range [1, +inf).  Parameter
        `rtol` is not in range [0, +inf).  Parameter `atol` is not in range
        [0, +inf).  Parameter `compl` is not false or true.  Parameter
        `return_error` is not false or true.  Parameter `dfs` has a strictly
        negative length.

    IndexError
        If the parameter `dfs` has known length (has the attribute `__len__`),
        but iterating over it exceeds the length, the algorithm will try to set
        values on nonexisting indices of lists which will then raise an
        exception of type `IndexError` which is not caught.

    others
        If iterating over the parameter `dfs` fails with an exception, it is not
        caught.  Exceptions thrown by the `err_f` function are not caught.
        Exceptions thrown by converting the returned value of `err_f` to `float`
        are not caught.

    Notes
    =====
    If the standard deviation `std` of a column `c` in an original dataframe is
    NaN or yields `True` when calling
    `numpy.isclose(1.0, 1.0 + std, rtol = rtol, atol = atol)`, the column is not
    used to compute the total standard deviation error on columns when checking
    generated potential subdataframes.  Columns that do not yield true when
    calling `pandas.api.types.is_numeric_dtype` on them are automatically
    ignored.

    The standard deviation is not computed on dataframes of 0 or 1 rows.  If the
    parameter `n` is 1 and the parameter `compl` is false, a uniformly arbitrary
    row is extracted from each dataframe.  If the parameter `n` is 0 or 1 and
    the parameter `compl` is false, all standard deviation errors will be set to
    0.

    If the parameter `dfs` has known length (has the attribute `__len__`),
    it is used to initialise lists of dataframes and standard deviation errors.
    This is done to speed up the algorithm.  However, if iterating over `dfs`
    exceeds this list, an exception of type `IndexError` will occur, and, if
    iterating stops before reaching the length, the resulting tuples will be of
    the initially known length with unexpected data at their ends.

    """

    # Define the function to insert elements to lists if the parameter `dfs` has
    # known length (has parameter `__len__`).
    def _known_length_insert_element (L, i, x):
        """
        Length is known; insert an element.

        """

        # Set the `i`-th element in `L` to `x`.
        L[i] = x

        # Return `L`.
        return L

    # Define the function to insert elements to lists if the parameter `dfs`
    # does not have known length (does not have parameter `__len__`).
    def _unknown_length_insert_element (L, i, x):
        """
        Length is unknows; insert an element.

        """

        # Append `x` to the end of `L`.
        L.append(x)

        # Return `L`.
        return L

    # Sanitise the parameter dfs.
    if not (
        isinstance(dfs, (_Sequence, _Iterable)) or
        hasattr(dfs, '__iter__') or
        hasattr(dfs, '__getitem__')
    ):
        raise TypeError('Collection of dataframes must be iterable.')

    # Sanitise the parameter `n`.
    if hasattr(n, '__iter__') or hasattr(n, '__array__'):
        if not isinstance(n, _np.ndarray):
            try:
                n = _np.array(n)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(n, _np.ndarray):
        if n.size == 1 or n.shape == tuple():
            n = n.ravel()
            if not (n.size == 1):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
            try:
                n = n.dtype.type(n[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
    if not (
            isinstance(n, _six.integer_types) or
            isinstance(n, (_np.integer, _numbers.Integral))
    ) or isinstance(n, (bool, _np.bool_)):
        raise TypeError('Number of rows in subdataframes must be integral.')
    try:
        n = _copy.deepcopy(int(n))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Number of rows in subdataframes must be of type `int`.'
        )
    if _math.isnan(n) or _math.isinf(n):
        raise ValueError(
            'Number of rows in subdataframes must be finite and non-NaN.'
        )
    if n < 0:
        raise ValueError(
            'Number of rows in subdataframes must be non-negative.'
        )

    # Sanitise the parameter `ok`.
    if hasattr(ok, '__iter__') or hasattr(ok, '__array__'):
        if not isinstance(ok, _np.ndarray):
            try:
                ok = _np.array(ok)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(ok, _np.ndarray):
        if ok.size == 1 or ok.shape == tuple():
            ok = ok.ravel()
            if not (ok.size == 1):
                raise TypeError(
                    'Accepted standard deviation error must be a real number.'
                )
            try:
                ok = ok.dtype.type(ok[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError(
                    'Accepted standard deviation error must be a real number.'
                )
    if (
        not (
            isinstance(
                ok,
                (
                    float,
                    _np.integer,
                    _np.floating,
                    _numbers.Integral,
                    _numbers.Rational,
                    _numbers.Real
                )
            ) or isinstance(ok, _six.integer_types)
        ) or isinstance(ok, (bool, _np.bool_))
    ):
        raise TypeError(
            'Accepted standard deviation error must be a real number.'
        )
    try:
        ok = _copy.deepcopy(float(ok))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Accepted standard deviation error must be of type `float`.'
        )
    if _math.isnan(ok) or _math.isinf(ok):
        raise ValueError(
            'Accepted standard deviation error must be finite and non-NaN.'
        )
    if ok < 0.0:
        raise ValueError(
            'Accepted standard deviation error must be at least 0.'
        )
    if ok == 0.0:
        ok = 0.0

    # Sanitise the parameter `N_ITER`.
    if hasattr(N_ITER, '__iter__') or hasattr(N_ITER, '__array__'):
        if not isinstance(N_ITER, _np.ndarray):
            try:
                N_ITER = _np.array(N_ITER)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(N_ITER, _np.ndarray):
        if N_ITER.size == 1 or N_ITER.shape == tuple():
            N_ITER = N_ITER.ravel()
            if not (N_ITER.size == 1):
                raise TypeError(
                    'Maximal number of iterations must be integral.'
                )
            try:
                N_ITER = N_ITER.dtype.type(N_ITER[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError(
                    'Maximal number of iterations must be integral.'
                )
    if not (
            isinstance(N_ITER, _six.integer_types) or
            isinstance(N_ITER, (_np.integer, _numbers.Integral))
        ) or isinstance(N_ITER, (bool, _np.bool_)):
        raise TypeError('Maximal number of iterations must be integral.')
    try:
        N_ITER = _copy.deepcopy(int(N_ITER))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Maximal number of iterations must be of type `int`.')
    if _math.isnan(N_ITER) or _math.isinf(N_ITER):
        raise ValueError(
            'Maximal number of iterations must be finite and non-NaN.'
        )
    if N_ITER < 1:
        raise ValueError('Maximal number of iterations must be greater than 1.')

    # Sanitise the parameter `rtol`.
    if hasattr(rtol, '__iter__') or hasattr(rtol, '__array__'):
        if not isinstance(rtol, _np.ndarray):
            try:
                rtol = _np.array(rtol)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(rtol, _np.ndarray):
        if rtol.size == 1 or rtol.shape == tuple():
            rtol = rtol.ravel()
            if not (rtol.size == 1):
                raise TypeError('Relative tolerance must be a real number.')
            try:
                rtol = rtol.dtype.type(rtol[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Relative tolerance must be a real number.')
    if (
        not (
            isinstance(
                rtol,
                (
                    float,
                    _np.integer,
                    _np.floating,
                    _numbers.Integral,
                    _numbers.Rational,
                    _numbers.Real
                )
            ) or isinstance(rtol, _six.integer_types)
        ) or isinstance(rtol, (bool, _np.bool_))
    ):
        raise TypeError('Relative tolerance must be a real number.')
    try:
        rtol = _copy.deepcopy(float(rtol))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Relative tolerance must be of type `float`.')
    if _math.isnan(rtol) or _math.isinf(rtol):
        raise ValueError('Relative tolerance must be finite and non-NaN.')
    if rtol < 0.0:
        raise ValueError('Relative tolerance must be at least 0.')
    if rtol == 0.0:
        rtol = 0.0

    # Sanitise the parameter `atol`.
    if hasattr(atol, '__iter__') or hasattr(atol, '__array__'):
        if not isinstance(atol, _np.ndarray):
            try:
                atol = _np.array(atol)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(atol, _np.ndarray):
        if atol.size == 1 or atol.shape == tuple():
            atol = atol.ravel()
            if not (atol.size == 1):
                raise TypeError('Absolute tolerance must be a real number.')
            try:
                atol = atol.dtype.type(atol[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Absolute tolerance must be a real number.')
    if (
        not (
            isinstance(
                atol,
                (
                    float,
                    _np.integer,
                    _np.floating,
                    _numbers.Integral,
                    _numbers.Rational,
                    _numbers.Real
                )
            ) or isinstance(atol, _six.integer_types)
        ) or isinstance(atol, (bool, _np.bool_))
    ):
        raise TypeError('Absolute tolerance must be a real number.')
    try:
        atol = _copy.deepcopy(float(atol))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Absolute tolerance must be of type `float`.')
    if _math.isnan(atol) or _math.isinf(atol):
        raise ValueError('Absolute tolerance must be finite and non-NaN.')
    if atol < 0.0:
        raise ValueError('Absolute tolerance must be at least 0.')
    if atol == 0.0:
        atol = 0.0

    # Sanitise the parameter `err_f`.
    if not (
        _inspect.isfunction(err_f) or
        _inspect.isbuiltin(err_f) or
        _inspect.isroutine(err_f) or
        isinstance(
            err_f,
            (
                _types.FunctionType,
                _types.BuiltinFunctionType,
                _functools.partial
            )
        ) or
        hasattr(err_f, '__call__')
    ):
        raise TypeError('Parameter `err_f` must be a function.')

    # Sanitise the parameter `compl`.
    if hasattr(compl, '__iter__') or hasattr(compl, '__array__'):
        if not isinstance(compl, _np.ndarray):
            try:
                compl = _np.array(compl)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(compl, _np.ndarray):
        if compl.size == 1 or compl.shape == tuple():
            compl = compl.ravel()
            if not (compl.size == 1):
                raise TypeError('Parameter `compl` must be boolean.')
            try:
                compl = compl.dtype.type(compl[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `compl` must be boolean.')
    if not (
        isinstance(compl, _six.integer_types) or
        isinstance(
            compl,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `compl` must be boolean.')
    if not (compl == False or compl == 0 or compl == True or compl == 1):
        raise ValueError('Parameter `compl` must be true or false.')
    try:
        compl = _copy.deepcopy(bool(compl))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `compl` must be of type `bool`.')

    # Sanitise the parameter `return_error`.
    if hasattr(return_error, '__iter__') or hasattr(return_error, '__array__'):
        if not isinstance(return_error, _np.ndarray):
            try:
                return_error = _np.array(return_error)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(return_error, _np.ndarray):
        if return_error.size == 1 or return_error.shape == tuple():
            return_error = return_error.ravel()
            if not (return_error.size == 1):
                raise TypeError('Parameter `return_error` must be boolean.')
            try:
                return_error = return_error.dtype.type(return_error[0])
            except (
                TypeError,
                ValueError,
                AttributeError,
                IndexError,
                KeyError
            ):
                raise TypeError('Parameter `return_error` must be boolean.')
    if not (
        isinstance(return_error, _six.integer_types) or
        isinstance(
            return_error,
            (bool, int, _np.bool_, _np.integer, _numbers.Integral)
        )
    ):
        raise TypeError('Parameter `return_error` must be boolean.')
    if not (
        return_error == False or
        return_error == 0 or
        return_error == True or
        return_error == 1
    ):
        raise ValueError('Parameter `return_error` must be true or false.')
    try:
        return_error = _copy.deepcopy(bool(return_error))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `return_error` must be of type `bool`.')

    # Define a dummy dataframe (empty).
    dummy_df = _pd.DataFrame(data = _np.zeros((0, 0), dtype = int, order = 'F'))

    # Define positive infinity of type `float`.
    infinity = float('inf')

    # Initialise the lists of subdataframes and standard deviation errors to
    # `None`.
    subdf = None
    E_min = None

    # Initialise the function to insert elements to lists to `None`.
    insert_element = None

    # If the parameter `dfs` has a known length (has the attribute `__len__`),
    # initialise the lists of subdataframes and standard deviation errors with
    # the length to speed up the algorithm, and set the function to insert
    # elements to lists to an adequate function.  Otherwise initialise the lists
    # to empty lists and set the function to insert elements to lists to an
    # adequate function.
    if hasattr(dfs, '__len__'):
        # Get the length of the parameter `dfs`.  If an exception occurs, raise
        # an exception of type `TypeError`.
        N_dfs = 0
        try:
            N_dfs = _copy.deepcopy(int(len(dfs)))
        except (TypeError, ValueError, AttributeError):
            # Free the memory.
            try:
                del N_dfs
            except (NameError, UnboundLocalError):
                pass
            del dummy_df
            del infinity

            # Raise an exception of type `TypeError`.
            raise TypeError(
                "Parameter `dfs` should have length of type `int` if it has "
                "known length."
            )

        # If the length is strictly negative, raise an exception of type
        # `ValueError`.
        if N_dfs < 0:
            # Free the memory.
            try:
                del N_dfs
            except (NameError, UnboundLocalError):
                pass
            del dummy_df
            del infinity

            # Raise an exception of type `ValueError`.
            raise ValueError('The number of dataframes must be non-negative.')

        # Initialise the list of subdataframes with empty dataframes.
        subdf = list(dummy_df for i in range(N_dfs))
        try:
            del i
        except (NameError, UnboundLocalError):
            pass

        # Initialise the list of standard deviation errors with zeros.
        E_min = list(infinity for i in range(N_dfs))
        try:
            del i
        except (NameError, UnboundLocalError):
            pass

        # Initialise the function to insert elements to lists.
        insert_element = _known_length_insert_element

        # Free the memory.
        del N_dfs
    else:
        # Initialise the list of subdataframes to an emty list.
        subdf = list()

        # Initialise the list of standard deviation errors to an empty list.
        E_min = list()

        # Initialise the function to insert elements to lists.
        insert_element = _unknown_length_insert_element

    # If `n` is at least 2, compute standard deviation errors of subdataframes.
    compute_std_err = _copy.deepcopy(bool(1 < n))

    # Generate subdataframes.
    for i, df in enumerate(dfs):
        # If the current dataframe is actually a series, convert it to a
        # dataframe.
        if isinstance(df, _Series):
            df = df.to_frame()

        # Check if the current dataframe is really a Pandas dataframe.  If not,
        # raise an exception of type `TypeError`.
        if not isinstance(df, _DataFrame):
            # Free the memory.
            try:
                del i
            except (NameError, UnboundLocalError):
                pass
            try:
                del df
            except (NameError, UnboundLocalError):
                pass
            del dummy_df
            del infinity
            del compute_std_err

            # Raise an exception of type `TypeError`.
            raise TypeError(
                'Original dataframes must be of type `pandas.DataFrame`.'
            )

        # If the current dataframe has `n` rows at the most, copy the complete
        # dataframe as its own subdataframe and add the standard deviation error
        # of 0 to the list `E_min`, then proceed to the next dataframe.
        if not (n < df.shape[0]):
            # Copy the complete dataframe.
            subdf = insert_element(subdf, i, df.copy(deep = True))

            # Add the standard deviation error of 0 to the list `E_min`.
            E_min = insert_element(E_min, i, 0.0)

            # Continue to the next iteration.
            continue

        # Set the current subdataframe to `None` and set the standard deviation
        # error to positive infinity.
        subdf = insert_element(subdf, i, dummy_df)
        E_min = insert_element(E_min, i, infinity)

        # Compute standard deviations on columns in the original dataframe.
        std = dict() if df.shape[0] < 2 else dict(
            (c, df[c].std(ddof = 1))
                for c in df.columns if _is_numeric_dtype(df[c].dtype)
        )
        try:
            del c
        except (NameError, UnboundLocalError):
            pass

        # Ignore NaN standard deviations and the ones too close to 0.
        std = dict(
            (c, std_c)
                for c, std_c in _six.iteritems(std)
                if not (
                    _np.isnan(std_c) or
                    _np.isclose(1.0, 1.0 + std_c, rtol = rtol, atol = atol)
                )
        )
        try:
            del c
        except (NameError, UnboundLocalError):
            pass
        try:
            del std_c
        except (NameError, UnboundLocalError):
            pass

        # Generate subdataframes of the current dataframe.
        for j in range(N_ITER):
            # Extract `n` rows from the current dataframe.
            aux = df.sample(n, replace = False)

            # Compute the total standard deviation error of the extracted
            # subdataframe.
            E = float(
                err_f(
                    tuple(
                        abs(aux[c].std(ddof = 1) / std_c - 1.0)
                            for c, std_c in _six.iteritems(std)
                    )
                )
            ) if compute_std_err else 0.0
            try:
                del c
            except (NameError, UnboundLocalError):
                pass
            try:
                del std_c
            except (NameError, UnboundLocalError):
                pass

            # If the complement should also be taken into consideration, compute
            # the maximal standard deviation error (of the extracted
            # subdataframe and of its complement).
            if compl:
                # Compute the complement of the extracted subdataframe.
                aux_c = df.loc[~df.index.isin(aux.index)]

                # Compute the maximal standard deviation error (of the extracted
                # subdataframe and of its complement) if the complement has at
                # least 2 rows.
                if 1 < aux_c.shape[0]:
                    E = max(
                        E,
                        float(
                            err_f(
                                tuple(
                                    abs(aux_c[c].std(ddof = 1) / std_c - 1.0)
                                        for c, std_c in _six.iteritems(std)
                                )
                            )
                        )
                    )
                    try:
                        del c
                    except (NameError, UnboundLocalError):
                        pass
                    try:
                        del std_c
                    except (NameError, UnboundLocalError):
                        pass

                # Free the memory.
                del aux_c

            # If the error is strictly less than the best error so far, update
            # the current subdataframe and the standard deviation error.
            if E < E_min[i]:
                subdf[i] = aux.copy(deep = True)
                E_min[i] = E

                # If the standard deviation error is acceptable, break the inner
                # `for`-loop.
                if E < ok:
                    # Free the memory.
                    del E
                    del aux

                    # Break the inner `for`-loop.
                    break

            # Free the memroy.
            del E
            del aux
        try:
            del j
        except (NameError, UnboundLocalError):
            pass

        # Free the memory.
        del std
    try:
        del i
    except (NameError, UnboundLocalError):
        pass
    try:
        del df
    except (NameError, UnboundLocalError):
        pass

    # Free the memory.
    del dummy_df
    del infinity
    del compute_std_err
    del insert_element

    # Return the extracted subdataframes and the corresponding standard
    # deviation errors if demanded.
    return (tuple(subdf), tuple(E_min)) if return_error else tuple(subdf)
