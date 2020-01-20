# -*- coding: utf-8 -*-

"""
Functions for reproducing the Mills, Spanner and Tamblyn's CNN.

The convolutional neural network is explained in [1].

Notes
=====
In the rest of the documentation, the Mills, Spanner and Tamblyn's convolutional
neural network will be called "MST CNN".

References
==========
[1] K. Mills, M. Spanner, I. Tamblyn, Deep Learning and the Schrödinger
    Equation, 2017, arXiv: 1702.01361 [cond-mat.mtrl-sci], url:
    https://arxiv.org/abs/1702.01361.

"""

# Import standard library.
import copy as _copy
import math as _math
import numbers as _numbers
import six as _six

# Import TensorFlow.
import tensorflow as _tf

# Import Keras.
import keras as _keras
import keras.layers as _kl
import keras.models as _km
import keras.optimizers as _ko

# Define the function to create the optimiser for the MST CNN.
def MST_CNN_optimiser (lr = 1.0e-3, **kwargs):
    """
    Create an optimiser for the MST CNN.

    This function is a wrapper around the `keras.optimizers.Adadelta`
    constructor.  The function actually returns
        >>> kreas.optimizers.Adadelta(lr = lr, **kwargs)

    Parameters
    ==========
    lr : float in range [0, +inf), optional
        Learning rate of the optimiser (default is 0.001).  See the
        documentation for `keras.optimizers.Adadelta` for more information.

    kwargs
        Additional keyword arguments passed to the `keras.optimizers.Adadelta`
        constructor.  This arguments are not checked or sanitised, they are
        passed raw to the `keras.optimizers.Adadelta` constructor.  See the
        documentation for `keras.optimizers.Adadelta` for more information.

    Returns
    =======
    optimiser : Adadelta
        The Adadelta optimiser used in the MST CNN with specified parameters.

    Notes
    =====
    No argument is checked or sanitised.  The arguments are passed raw to the
    `keras.optimizers.Adadelta` contructor.  No exception thrown by the
    constructor is catched and should be catched by the calling routine.

    """

    # Return the optimiser.
    return _ko.Adadelta(lr = lr, **kwargs)

# Define the function to create a sequential model of the MST CNN.
def MST_CNN_sequential (
    input_shape = (256, 256, 1),
    reductions = 7,
    nonreductions = 2,
    reduction_filters = 64,
    nonreduction_filters = 16,
    reduction_kernel_size = (3, 3),
    nonreduction_kernel_size = (4, 4),
    reduction_activation = 'relu',
    nonreduction_activation = 'relu',
    final_fully_connected_units = 1024,
    final_fully_connected_activation = 'relu',
    output_units = 1,
    output_activation = 'linear',
    data_format = 'channels_last',
    model_kwargs = dict(),
    input_kwargs = dict(),
    reduction_kwargs = dict(),
    nonreduction_kwargs = dict(),
    final_fully_connected_kwargs = dict(),
    output_kwargs = dict()
):
    """
    Create the MST CNN as a sequential model.

    Parameters
    ==========
    input_shape : tuple of ints, optional
        Shape of the input (default is `(256, 256, 1)`).  See the documentation
        for `keras.layers.Input` and `keras.layers.convolutional.Conv2D` for
        more information.

    reductions : int in range [0, +inf), optional
        Number of reduction convolutional layers (default is 7).

    nonreductions : int in range [0, +inf), optional
        Number of non-reduction convolutional layers between each pair of
        consecutive reduction convolutional layers (default is 2).

    reduction_filters : int, optional
        Number of output filters of each reduction convolutional layer (default
        is 64).  See the documentation for `keras.layers.convolutional.Conv2D`
        for more information.

    nonreduction_filters : int, optional
        Number of output filters of each non-reduction convolutional layer
        (default is 16).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_kernel_size : int or tuple of ints, optional
        Size of the kernel for each reduction convolutional layer (default is
        `(3, 3)`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    nonreduction_kernel_size : int or tuple of ints, optional
        Size of the kernel for each non-reduction convolutional layer (default
        is `(4, 4)`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_activation : function or str, optional
        Activation function for each reduction convolutional layer (default is
        `'relu'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_activation : function or str, optional
        Activation function for each non-reduction convolutional layer (default
        is `'relu'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    final_fully_connected_units : int
        Dimensionality of the final fully-connected hidden layer (default is
        1024).  See the documentation for `keras.layers.Dense` for more
        information.

    final_fully_connected_activation : function or str, optional
        Activation function for the final fully-connected hidden layer (default
        is `'relu'`).  See the documentation for `keras.layers.Dense` for more
        information.

    output_units : int, optional
        Dimensionality of the output (default is 1).  See the documentation for
        `keras.layers.Dense` for more information.

    final_fully_connected_activation : function or str, optional
        Activation function for the output layer (default is `'linear'`).  See
        the documentation for `keras.layers.Dense` for more information.

    data_format : str, optional
        Value `'channels_first'` for the dimensionality of channels on the first
        place or `'channels_last'` for the dimensionality of channels on the
        last place (default is `'channels_last'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    model_kwargs : dict, optional
        Additional keyword arguments for the sequential model (default is `{}`).
        See the documentation for `keras.models.Sequential` for more
        information.

    input_kwargs : dict, optional
        Additional keyword arguments for the input layer (default is `{}`).  See
        the documentation for `keras.layers.Input` for more information.

    reduction_kwargs : dict, optional
        Additional keyword arguments for each reduction convolutional layer
        (default is `{}`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    nonreduction_kwargs : dict, optional
        Additional keyword arguments for each non-reduction convolutional layer
        (default is `{}`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    final_fully_connected_kwargs : dict, optional
        Additional keyword arguments for the final fully-connected hidden layer
        (default is `{}`).  See the documentation for `keras.layers.Dense` for
        more information.

    input_kwargs : dict, optional
        Additional keyword arguments for the output layer (default is `{}`).
        See the documentation for `keras.layers.Dense` for more information.

    Returns
    =======
    model : Sequential
        Noncompiled sequential model of the MST CNN with specified parameters.

    Raises
    ======
    TypeError
        Parameter `reductions` is not an integer.  Parameter `nonreductions` is
        not an integer.

    ValueError
        Parameter `reductions` is not in range [0, +inf).  Parameter
        `nonreductions` is not in range [0, +inf).

    Others
        Other exceptions thrown by Keras model's and layers' constructors are
        not caught.  It is not even checked if the keys in keyword arguments'
        dictionaries are strings or not.

    """

    # Sanitise the parameter `reductions`.
    if hasattr(reductions, '__iter__') or hasattr(reductions, '__array__'):
        if not isinstance(reductions, _np.ndarray):
            try:
                reductions = _np.asarray(reductions)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(reductions, _np.ndarray):
        if reductions.size == 1 or reductions.shape == tuple():
            reductions = reductions.ravel()
            if not (reductions.size == 1):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
            try:
                reductions = reductions.dtype.type(reductions[0])
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
            isinstance(reductions, _six.integer_types) or
            isinstance(reductions, (_np.integer, _numbers.Integral))
    ) or isinstance(reductions, (bool, _np.bool_)):
        raise TypeError('Number of rows in subdataframes must be integral.')
    try:
        reductions = _copy.deepcopy(int(reductions))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Number of rows in subdataframes must be of type `int`.'
        )
    if _math.isnan(reductions) or _math.isinf(reductions):
        raise ValueError(
            'Number of rows in subdataframes must be finite and non-NaN.'
        )
    if reductions < 0:
        raise ValueError(
            'Number of rows in subdataframes must be non-negative.'
        )

    # Sanitise the parameter `nonreductions`.
    if (
        hasattr(nonreductions, '__iter__') or
        hasattr(nonreductions, '__array__')
    ):
        if not isinstance(nonreductions, _np.ndarray):
            try:
                nonreductions = _np.asarray(nonreductions)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(nonreductions, _np.ndarray):
        if nonreductions.size == 1 or nonreductions.shape == tuple():
            nonreductions = nonreductions.ravel()
            if not (nonreductions.size == 1):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
            try:
                nonreductions = nonreductions.dtype.type(nonreductions[0])
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
            isinstance(nonreductions, _six.integer_types) or
            isinstance(nonreductions, (_np.integer, _numbers.Integral))
    ) or isinstance(nonreductions, (bool, _np.bool_)):
        raise TypeError('Number of rows in subdataframes must be integral.')
    try:
        nonreductions = _copy.deepcopy(int(nonreductions))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Number of rows in subdataframes must be of type `int`.'
        )
    if _math.isnan(nonreductions) or _math.isinf(nonreductions):
        raise ValueError(
            'Number of rows in subdataframes must be finite and non-NaN.'
        )
    if nonreductions < 0:
        raise ValueError(
            'Number of rows in subdataframes must be non-negative.'
        )

    # Initialise the model.
    model = _km.Sequential(name = 'MST_CNN', **model_kwargs)

    # Add the input layer.
    model.add(_kl.Input(shape = input_shape, name = 'input', **input_kwargs))

    # If at least one reduction should be added, add the first reduction layer.
    if reductions:
        model.add(
            _kl.convolutional.Conv2D(
                filters = reduction_filters,
                kernel_size = reduction_kernel_size,
                strides = (2, 2),
                padding = 'same',
                activation = nonreduction_activation,
                data_format = data_format,
                name = "reduction__{red:d}".format(red = 0 + 1),
                **reduction_kwargs
            )
        )

    # Add the remaining reduction layers with `nonreductions` non-reduction
    # layers between each pair of consecutive reduction layers.
    for i in _six.moves.range(1, reductions):
        # Add `nonreductions` non-reduction layers.
        for j in _six.moves.range(nonreductions):
            model.add(
                _kl.convolutional.Conv2D(
                    filters = nonreduction_filters,
                    kernel_size = nonreduction_kernel_size,
                    strides = (1, 1),
                    padding = 'same',
                    activation = nonreduction_activation,
                    data_format = data_format,
                    name = (
                        "nonreduction_from_reduction_{red_before:d}_to_"
                        "reduction_{red_next:d}__{nonred:d}"
                    ).format(red_before = i, red_next = i + 1, nonred = j + 1),
                    **nonreduction_kwargs
                )
            )
        try:
            del j
        except (NameError, UnboundLocalError):
            pass

        # Add a reduction layer.
        model.add(
            _kl.convolutional.Conv2D(
                filters = reduction_filters,
                input_shape = input_shape,
                kernel_size = reduction_kernel_size,
                strides = (2, 2),
                padding = 'same',
                activation = nonreduction_activation,
                data_format = data_format,
                name = "reduction__{red:d}".format(red = i + 1),
                **reduction_kwargs
            )
        )
    try:
        del i
    except (NameError, UnboundLocalError):
        pass

    # Flatten the output of the last reducing convolutional layer.
    model.add(
        _kl.Flatten(
            data_format = data_format,
            name = 'flattening'
        )
    )

    # Add the final, fully-connected hidden layer.
    model.add(
        _kl.Dense(
            units = final_fully_connected_units,
            activation = final_fully_connected_activation,
            name = 'final_fully_connected',
            **final_fully_connected_kwargs
        )
    )

    # Add the output layer.
    model.add(
        _kl.Dense(
            units = output_units,
            activation = output_activation,
            name = 'output',
            **output_kwargs
        )
    )

    # Return the MST CNN model.
    return model

# Define a function to create MST CNN as a tensor.
def MST_CNN_tensor (
    input_shape = (256, 256, 1),
    reductions = 7,
    nonreductions = 2,
    reduction_filters = 64,
    nonreduction_filters = 16,
    reduction_kernel_size = (3, 3),
    nonreduction_kernel_size = (4, 4),
    reduction_activation = 'relu',
    nonreduction_activation = 'relu',
    final_fully_connected_units = 1024,
    final_fully_connected_activation = 'relu',
    output_units = 1,
    output_activation = 'linear',
    data_format = 'channels_last',
    input_kwargs = dict(),
    reduction_kwargs = dict(),
    nonreduction_kwargs = dict(),
    final_fully_connected_kwargs = dict(),
    output_kwargs = dict()
):
    """
    Create MST CNN as a tensor.

    Parameters
    ==========
    input_shape : tuple of ints, optional
        Shape of the input (default is `(256, 256, 1)`).  See the documentation
        for `keras.layers.Input` and `keras.layers.convolutional.Conv2D` for
        more information.

    reductions : int in range [0, +inf), optional
        Number of reduction convolutional layers (default is 7).

    nonreductions : int in range [0, +inf), optional
        Number of non-reduction convolutional layers between each pair of
        consecutive reduction convolutional layers (default is 2).

    reduction_filters : int, optional
        Number of output filters of each reduction convolutional layer (default
        is 64).  See the documentation for `keras.layers.convolutional.Conv2D`
        for more information.

    nonreduction_filters : int, optional
        Number of output filters of each non-reduction convolutional layer
        (default is 16).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_kernel_size : int or tuple of ints, optional
        Size of the kernel for each reduction convolutional layer (default is
        `(3, 3)`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    nonreduction_kernel_size : int or tuple of ints, optional
        Size of the kernel for each non-reduction convolutional layer (default
        is `(4, 4)`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_activation : function or str, optional
        Activation function for each reduction convolutional layer (default is
        `'relu'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    reduction_activation : function or str, optional
        Activation function for each non-reduction convolutional layer (default
        is `'relu'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    final_fully_connected_units : int
        Dimensionality of the final fully-connected hidden layer (default is
        1024).  See the documentation for `keras.layers.Dense` for more
        information.

    final_fully_connected_activation : function or str, optional
        Activation function for the final fully-connected hidden layer (default
        is `'relu'`).  See the documentation for `keras.layers.Dense` for more
        information.

    output_units : int, optional
        Dimensionality of the output (default is 1).  See the documentation for
        `keras.layers.Dense` for more information.

    final_fully_connected_activation : function or str, optional
        Activation function for the output layer (default is `'linear'`).  See
        the documentation for `keras.layers.Dense` for more information.

    data_format : str, optional
        Value `'channels_first'` for the dimensionality of channels on the first
        place or `'channels_last'` for the dimensionality of channels on the
        last place (default is `'channels_last'`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    input_kwargs : dict, optional
        Additional keyword arguments for the input layer (default is `{}`).  See
        the documentation for `keras.layers.Input` for more information.

    reduction_kwargs : dict, optional
        Additional keyword arguments for each reduction convolutional layer
        (default is `{}`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    nonreduction_kwargs : dict, optional
        Additional keyword arguments for each non-reduction convolutional layer
        (default is `{}`).  See the documentation for
        `keras.layers.convolutional.Conv2D` for more information.

    final_fully_connected_kwargs : dict, optional
        Additional keyword arguments for the final fully-connected hidden layer
        (default is `{}`).  See the documentation for `keras.layers.Dense` for
        more information.

    input_kwargs : dict, optional
        Additional keyword arguments for the output layer (default is `{}`).
        See the documentation for `keras.layers.Dense` for more information.

    Returns
    =======
    CNN : Tensor
        Tensor of the MST CNN with specified parameters.

    Raises
    ======
    TypeError
        Parameter `reductions` is not an integer.  Parameter `nonreductions` is
        not an integer.

    ValueError
        Parameter `reductions` is not in range [0, +inf).  Parameter
        `nonreductions` is not in range [0, +inf).

    Others
        Other exceptions thrown by Keras model's and layers' constructors are
        not caught.  It is not even checked if the keys in keyword arguments'
        dictionaries are strings or not.

    """

    # Sanitise the parameter `reductions`.
    if hasattr(reductions, '__iter__') or hasattr(reductions, '__array__'):
        if not isinstance(reductions, _np.ndarray):
            try:
                reductions = _np.asarray(reductions)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(reductions, _np.ndarray):
        if reductions.size == 1 or reductions.shape == tuple():
            reductions = reductions.ravel()
            if not (reductions.size == 1):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
            try:
                reductions = reductions.dtype.type(reductions[0])
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
            isinstance(reductions, _six.integer_types) or
            isinstance(reductions, (_np.integer, _numbers.Integral))
    ) or isinstance(reductions, (bool, _np.bool_)):
        raise TypeError('Number of rows in subdataframes must be integral.')
    try:
        reductions = _copy.deepcopy(int(reductions))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Number of rows in subdataframes must be of type `int`.'
        )
    if _math.isnan(reductions) or _math.isinf(reductions):
        raise ValueError(
            'Number of rows in subdataframes must be finite and non-NaN.'
        )
    if reductions < 0:
        raise ValueError(
            'Number of rows in subdataframes must be non-negative.'
        )

    # Sanitise the parameter `nonreductions`.
    if (
        hasattr(nonreductions, '__iter__') or
        hasattr(nonreductions, '__array__')
    ):
        if not isinstance(nonreductions, _np.ndarray):
            try:
                nonreductions = _np.asarray(nonreductions)
            except (TypeError, ValueError, AttributeError):
                pass
    if isinstance(nonreductions, _np.ndarray):
        if nonreductions.size == 1 or nonreductions.shape == tuple():
            nonreductions = nonreductions.ravel()
            if not (nonreductions.size == 1):
                raise TypeError(
                    'Number of rows in subdataframes must be integral.'
                )
            try:
                nonreductions = nonreductions.dtype.type(nonreductions[0])
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
            isinstance(nonreductions, _six.integer_types) or
            isinstance(nonreductions, (_np.integer, _numbers.Integral))
    ) or isinstance(nonreductions, (bool, _np.bool_)):
        raise TypeError('Number of rows in subdataframes must be integral.')
    try:
        nonreductions = _copy.deepcopy(int(nonreductions))
    except (TypeError, ValueError, AttributeError):
        raise TypeError(
            'Number of rows in subdataframes must be of type `int`.'
        )
    if _math.isnan(nonreductions) or _math.isinf(nonreductions):
        raise ValueError(
            'Number of rows in subdataframes must be finite and non-NaN.'
        )
    if nonreductions < 0:
        raise ValueError(
            'Number of rows in subdataframes must be non-negative.'
        )

    # Create the CNN as an input layer.
    CNN = _kl.Input(shape = input_shape, name = 'MST_CNN_input', **input_kwargs)

    # If at least one reduction should be added, add the first reduction layer.
    if reductions:
        CNN = _kl.convolutional.Conv2D(
            filters = reduction_filters,
            kernel_size = reduction_kernel_size,
            strides = (2, 2),
            padding = 'same',
            activation = reduction_activation,
            data_format = data_format,
            name = "reduction__{red:d}".format(red = 0 + 1),
            **reduction_kwargs
        )(CNN)

    # Add the remaining reduction layers with `nonreductions` non-reduction
    # layers between each pair of consecutive reduction layers.
    for i in _six.moves.range(1, reductions):
        # Add `nonreductions` non-reduction layers.
        for j in _six.moves.range(nonreductions):
            CNN = _kl.convolutional.Conv2D(
                filters = nonreduction_filters,
                kernel_size = nonreduction_kernel_size,
                strides = (1, 1),
                padding = 'same',
                activation = nonreduction_activation,
                data_format = data_format,
                name = (
                    "nonreduction_from_reduction_{red_before:d}_to_"
                    "reduction_{red_next:d}__{nonred:d}"
                ).format(red_before = i, red_next = i + 1, nonred = j + 1),
                **nonreduction_kwargs
            )(CNN)
        try:
            del j
        except (NameError, UnboundLocalError):
            pass

        # Add a reduction layer.
        CNN = _kl.convolutional.Conv2D(
            filters = reduction_filters,
            input_shape = input_shape,
            kernel_size = reduction_kernel_size,
            strides = (2, 2),
            padding = 'same',
            activation = nonreduction_activation,
            data_format = data_format,
            name = "reduction__{red:d}".format(red = i + 1),
            **reduction_kwargs
        )(CNN)

    # Flatten the output of the seventh reducing convolutional layer.
    CNN = _kl.Flatten(
        data_format = data_format,
        name = 'flattening'
    )(CNN)

    # Add the final, fully-connected hidden layer.
    CNN = _kl.Dense(
        units = final_fully_connected_units,
        activation = final_fully_connected_activation,
        name = 'final_fully_connected',
        **final_fully_connected_kwargs
    )(CNN)

    # Add the output layer.
    CNN = _kl.Dense(
        units = output_units,
        activation = output_activation,
        name = 'output',
        **output_kwargs
    )(CNN)

    # Return the MST CNN tensor.
    return CNN
