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

# Import TensorFlow.
import tesnorflow as _tf

# Import Keras.
import keras as _keras
import keras.layers as _kl
import keras.models as _km
import keras.optimizers as _ko

# Define the function to create the optimiser for the MST CNN.
def MST_CNN_optimiser ():
    """
    Create an optimiser used in the MST CNN.

    Returns
    =======
    optimiser : Adadelta
        The Adadelta optimiser used in the MST CNN.

    """

    # Return the optimiser.
    return _ko.Adadelta(lr = 1.0e-3)

# Define the function to create a sequential model of the MST CNN.
def MST_CNN_sequential ():
    """
    Create the MST CNN as a sequential model.

    Returns
    =======
    model : Sequential
        Noncompiled sequential model of the MST CNN.

    """

    # Define the model.
    model = _km.Sequential(name = 'Mills_Spanner_Tamblyn_CNN')

    # Add the first reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            input_shape =(256, 256, 1),
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__1'
        )
    )

    # Add the two non-reducing convolutional layers between the first and the
    # second reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_1_to_reduction_2__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_1_to_reduction_2__2'
        )
    )

    # Add the second reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__2'
        )
    )

    # Add the two non-reducing convolutional layers between the second and the
    # third reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_2_to_reduction_3__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_2_to_reduction_3__2'
        )
    )

    # Add the third reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__3'
        )
    )

    # Add the two non-reducing convolutional layers between the third and the
    # fourth reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_3_to_reduction_4__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_3_to_reduction_4__2'
        )
    )

    # Add the fourth reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__4'
        )
    )

    # Add the two non-reducing convolutional layers between the fourth and the
    # fifth reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_4_to_reduction_5__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_4_to_reduction_5__2'
        )
    )

    # Add the fifth reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__5'
        )
    )

    # Add the two non-reducing convolutional layers between the fifth and the
    # sixth reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_5_to_reduction_6__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_5_to_reduction_6__2'
        )
    )

    # Add the sixth reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__6'
        )
    )

    # Add the two non-reducing convolutional layers between the sixth and the
    # seventh reducing convolutional layers.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_6_to_reduction_7__1'
        )
    )
    model.add(
        _kl.convolutional.Conv2D(
            filters = 16,
            kernel_size = (4, 4),
            strides = (1, 1),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'nonreduction_from_reduction_6_to_reduction_7__2'
        )
    )

    # Add the seventh reducing convolutional layer.
    model.add(
        _kl.convolutional.Conv2D(
            filters = 64,
            kernel_size = (3, 3),
            strides = (2, 2),
            padding = 'same',
            activation = 'relu',
            data_format = 'channels_last',
            name = 'reduction__7'
        )
    )

    # Flatten the output of the seventh reducing convolutional layer.
    model.add(
        _kl.Flatten(
            data_format = 'channels_last',
            name = 'flattening'
        )
    )

    # Add the final, fully-connected hidden layer.
    model.add(
        _kl.Dense(
            units = 1024,
            activation = 'relu',
            name = 'final_fully_connected'
        )
    )

    # Add the output layer.
    model.add(
        _kl.Dense(
            units = 1,
            activation = 'linear',
            name = 'output'
        )
    )

    # Return the model.
    return model

# Define a function to create MST CNN as a tensor.
def MST_CNN_tensor ():
    """
    Create MST CNN as a tensor.

    Returns
    =======
    CNN : Tensor
        Tensor of the MST CNN.

    """

    # Create the CNN as an input layer.
    CNN = _kl.Input(shape = (256, 256, 1), name = 'Mills_Spanner_Tamblyn_CNN')

    # Add the first reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        input_shape = input_shape,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__1'
    )(CNN)

    # Add the two non-reducing convolutional layers between the first and the
    # second reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_1_to_reduction_2__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_1_to_reduction_2__2'
    )(CNN)

    # Add the second reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__2'
    )(CNN)

    # Add the two non-reducing convolutional layers between the second and the
    # third reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_2_to_reduction_3__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_2_to_reduction_3__2'
    )(CNN)

    # Add the third reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__3'
    )(CNN)

    # Add the two non-reducing convolutional layers between the third and the
    # fourth reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_3_to_reduction_4__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_3_to_reduction_4__2'
    )(CNN)

    # Add the fourth reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__4'
    )(CNN)

    # Add the two non-reducing convolutional layers between the fourth and the
    # fifth reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_4_to_reduction_5__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_4_to_reduction_5__2'
    )(CNN)

    # Add the fifth reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__5'
    )(CNN)

    # Add the two non-reducing convolutional layers between the fifth and the
    # sixth reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_5_to_reduction_6__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_5_to_reduction_6__2'
    )(CNN)

    # Add the sixth reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__6'
    )(CNN)

    # Add the two non-reducing convolutional layers between the sixth and the
    # seventh reducing convolutional layers.
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_6_to_reduction_7__1'
    )(CNN)
    CNN = _kl.convolutional.Conv2D(
        filters = 16,
        kernel_size = (4, 4),
        strides = (1, 1),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'nonreduction_from_reduction_6_to_reduction_7__2'
    )(CNN)

    # Add the seventh reducing convolutional layer.
    CNN = _kl.convolutional.Conv2D(
        filters = 64,
        kernel_size = (3, 3),
        strides = (2, 2),
        padding = 'same',
        activation = 'relu',
        data_format = 'channels_last',
        name = 'reduction__7'
    )(CNN)

    # Flatten the output of the seventh reducing convolutional layer.
    CNN = _kl.Flatten(
        data_format = 'channels_last',
        name = 'flattening'
    )(CNN)

    # Add the final, fully-connected hidden layer.
    CNN = _kl.Dense(
        units = 1024,
        activation = 'relu',
        name = 'final_fully_connected'
    )(CNN)

    # Add the output layer.
    CNN = _kl.Dense(
        units = 1,
        activation = 'linear',
        name = 'output'
    )(CNN)

    # Return the CNN.
    return CNN
