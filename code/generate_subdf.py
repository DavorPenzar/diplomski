"""
Definition of a function for extracting subdatasets while preserving variation.

This file is part of Davor Penzar's master thesis programing.

"""

# -*- coding: utf-8 -*-

# Import standard library.
import copy as _copy
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

def generate_subdf (
    dfs,
    n,
    ok = 1.0e-3,
    N_ITER = 1000,
    rtol = 1.0e-5,
    atol = 1.0e-8,
    err_f = max,
    compl = False
):
    """
    Extract subdataframes.

    Parameters
    ==========
    dfs : list-like of DataFrames
        Dataframes from which to extract subdataframes.

    n : int in range [2, +inf)
        Number of rows to extract into each subdataframe.

    ok : float in range [0, +inf), optional
        Maximal allowed relative standard deviation error (default is 1.0e-3).
        If a subdataframe is extracted from an original dataframe and the
        computed total standard deviation error on all columns (see parameters
        `rtol`, `atol` and `err_f`) is strictly less than `ok`, the subdataframe
        is accepted and the algorithm proceeds to the next dataframe (or ends if
        the dataframe was the last one).  If the parameter `compl` is true, the
        complement's standard deviation error must also be strictly less than
        `ok` (see parameter `compl`).

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
        deviation error.

    compl : boolean, optional
        If true, the standard deviation error of the subdataframe's complement
        is also taken into consideration---the total standard deviation error is
        the maximum of the two standard deviation errors (the subdataframe's and
        its complement's) (default is false).  The standard deviation error on
        the subdataframe's complement is computed analogously to the
        subdataframe's standard deviation error.

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
        subdataframe and the i-th subdataframe's complement.

    Raises
    ======
    TypeError
        Parameter `dfs` is not iterable.  Parameter `n` is not an integer.
        Parameter `ok` is not a real number.  Parameter `N_ITER` is not an
        integer.  Parameter `rtol` is not a real number.  Parameter `atol` is
        not a real number.  Parameter `err_f` is not a function.  Parameter
        `compl` is not boolean.  A dataframe in `dfs` is not a Pandas dataframe.

    ValueError
        Parameter `n` is not in range [2, +inf).  Parameter `ok` is not in range
        [0, +inf).  Parameter N_ITER is not in range [1, +inf).  Parameter
        `rtol` is not in range [0, +inf).  Parameter `atol` is not in range
        [0, +inf).  Parameter `compl` is not false or true.  A dataframe in
        `dfs` does not have at least `n` rows.

    Other
        If iterating over the parameter `dfs` fails with an exception, it is not
        caught.  Exceptions thrown by the `err_f` function are not caught.

    Notes
    =====
    If the standard deviation `std` of a column `c` in an original dataframe is
    NaN or yields `True` when calling
    `numpy.isclose(1.0, 1.0 + std, rtol = rtol, atol = atol)`, the column is not
    used to compute the total standard deviation error on columns when checking
    generated potential subdataframes.  Columns that do not yield true when
    calling `pandas.api.types.is_numeric_dtype` on them are automatically
    ignored.

    """

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
            if n.size != 1:
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
    if n < 2:
        raise ValueError(
            'Number of rows in subdataframes must be strictly greater than 1.'
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
            if ok.size != 1:
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
            if N_ITER.size != 1:
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
            if rtol.size != 1:
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
            if atol.size != 1:
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
            if compl.size != 1:
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
    if compl not in {False, 0, True, 1}:
        raise ValueError('Parameter `compl` must be true or false.')
    try:
        compl = _copy.deepcopy(bool(compl))
    except (TypeError, ValueError, AttributeError):
        raise TypeError('Parameter `compl` must be of type `bool`.')

    # Initialise the lists of subdataframes.
    subdf = list()

    # Initialise the list of standard deviation errors.
    E_min = list()

    # Generate subdataframes.
    for df in dfs:
        # If the current dataframe is actually a series, convert it to a
        # dataframe.
        if isinstance(df, _Series):
            df = df.to_frame()

        # Check if the current dataframe is really a Pandas dataframe.  If not,
        # raise an exception of type `TypeError`.
        if not isinstance(df, _DataFrame):
            # Free the memory.
            try:
                del df
            except (NameError, UnboundLocalError):
                pass

            # Free the memory.
            del subdf
            del E_min

            # Raise an exception of type `TypeError`.
            raise TypeError(
                'Original dataframes must be of type `pandas.DataFrame`.'
            )

        # Set the current subdataframe to the original dataframe and set the
        # standard deviation error to positive infinity.
        subdf.append(df)
        E_min.append(float('inf'))

        # Check if the current dataframe has at least `n` rows.  If not, raise
        # an exception of type `ValueError`.
        if subdf[-1].shape[0] < n:
            # Free the memory.
            try:
                del df
            except (NameError, UnboundLocalError):
                pass

            # Free the memory.
            del subdf
            del E_min

            # Raise an exception of type `ValueError`.
            raise ValueError(
                'All original dataframes must have at least `n` rows.'
            )

        # Compute standard deviations on columns in the original dataframe.
        std = dict(
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
        for i in range(N_ITER):
            # Extract `n` different rows from the current dataframe.
            aux = df.loc[
                _np.array(
                    _choice(df.index, n, replace = False),
                    dtype = None,
                    copy = True,
                    order = 'F',
                    subok = False,
                    ndmin = 0
                )
            ]

            # Compute the total standard deviation error of the extracted
            # subdataframe.
            E = float(
                err_f(
                    tuple(
                        abs(aux[c].std(ddof = 1) / std_c - 1.0)
                            for c, std_c in _six.iteritems(std)
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

            # If the complement should also be taken into consideration, compute
            # the maximal standard deviation error (of the extracted
            # subdataframe and of its complement).
            if compl:
                # Compute the complement of the extracted subdataframe.
                aux_c = df.loc[~df.index.isin(aux.index)]

                # Compute the maximal standard deviation error (of the extracted
                # subdataframe and of its complement).
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
            if E < E_min[-1]:
                subdf[-1] = aux.copy(deep = True)
                E_min[-1] = E

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
            del i
        except (NameError, UnboundLocalError):
            pass

        # Free the memory.
        del std
    try:
        del df
    except (NameError, UnboundLocalError):
        pass

    # Return the extracted subdataframes and the corresponding standard
    # deviation errors.
    return (tuple(subdf), tuple(E_min))
