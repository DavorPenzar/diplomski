# -*- coding: utf-8 -*-

"""
Definitions of axis limits, ticks and tick labels for plotting data.

This file is a part of Davor Penzar's master thesis programing.

"""

# Import standard library modules.
import math as _math

# Set limits for plotting data from dataframes.
lim = {
    'x_0' : (0.0, 1.0),
    'y_0' : (-1.0, 1.0),
    'x_1' : (0.0, 0.5),
    'y_1' : (0.0, 0.9),
    'x_2' : (-1.0, 0.0),
    'y_2' : (-1.0, 1.0),
    'a' : (0.0, 2.0),
    'c' : (0.5, 1.0),
    'b' : (0.0, 1.0),
    'alpha' : (0.0, 2.5),
    'gamma' : (1.50, 3.25),
    'beta' : (2.00, 3.25),
    'sv_edge_0' : (2.5, 4.5),
    'sv_edge_1' : (0.0, 1.5),
    'sv_edge_2' : (0.0, 1.5),
    'sv_angle_0' : (8.885765865, 8.885765895),
    'sv_angle_1' : (0.0, 4.5),
    'sv_angle_2' : (0.0, 4.5),
    'r(b, alpha)' : (0.0, 1.5),
    'r(inv b, inv alpha)' : (0.00, 1.25),
    'r(inv b, gamma)' : (0.5, 1.5),
    'r(b, inv gamma)' : (0.1, 1,1),
    'r(b, beta)' : (0.95, 1.25),
    'r(b, sv_edge_0)' : (0.5, 1.5),
    'r(b, sv_edge_1)' : (0.6, 1.1),
    'r(inv b, sv_angle_1)' : (0.00, 1.25),
    'r(b, inv sv_angle_1)' : (0.0, 1.5),
    'r(alpha, gamma)' : (0.85, 1.25),
    'r(inv alpha, sv_edge_1)' : (0.00, 1.25),
    'r(alpha, inv sv_edge_1)' : (0.25, 1.50),
    'r(inv alpha, inv sv_angle_1)' : (0.5, 1.0),
    'r(gamma, sv_edge_1)' : (0.50, 1.25),
    'r(inv gamma, inv sv_edge_1)' : (0.1, 1.1),
    'r(inv beta, sv_edge_0)' : (0.6, 1.1),
    'r(beta, inv sv_edge_0)' : (0.6, 1.1),
    'r(inv beta, sv_edge_1)' : (0.3, 1.0),
    'r(beta, inv sv_edge_1)' : (0.95, 1.25),
    'r(inv sv_edge_0, sv_edge_1)' : (0.00, 1.25),
    'r(sv_edge_0, inv sv_edge_1)' : (0.5, 1.5),
    'r(sv_edge_1, sv_angle_1)' : (0.00, 1.25),
    'r(inv sv_edge_1, inv sv_angle_1)' : (0.0, 1.5),
    'l_0' : (0.0, 6000.0),
    'l_0_log10' : (1.5, 4.0),
    'l_0_inv' : (0.00, 0.02),
    'l_0_90_pc' : (0.0, 900.0)
}

# Set tick positions for plotting data from dataframes.
ticks = {
    'x_0' : tuple([0.5]),
    'y_0' : tuple([0.0]),
    'x_1' : (0.0, 0.1, 0.2, 0.3, 0.4, 0.5),
    'x_1_thirds' : (
        0.0000000000000000000000000000000000000000000000000000000000000000,
        0.1666666666666666666666666666666666666666666666666666666666666667,
        0.3333333333333333333333333333333333333333333333333333333333333333,
        0.5000000000000000000000000000000000000000000000000000000000000000
    ),
    'y_1' : (0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9),
    'y_1_thirds' : (0.0, 0.3, 0.6, 0.9),
    'x_2' : tuple([-0.5]),
    'y_2' : tuple([0.0]),
    'a' : tuple([1.0]),
    'c' : (0.5, 0.6, 0.7, 0.8, 0.9, 1.0),
    'b' : (0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0),
    'alpha' : (0.0, 0.5, 1.0, 1.5, 2.0, 2.5),
    'gamma' : (1.50, 1.75, 2.00, 2.25, 2.50, 2.75, 3.00, 3.25),
    'beta' : (2.00, 2.25, 2.50, 2.75, 3.00, 3.25),
    'sv_edge_0' : (2.5, 3.0, 3.5, 4.0, 4.5),
    'sv_edge_1' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25, 1.50),
    'sv_edge_2' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25, 1.50),
    'sv_angle_0' : (8.88576587, 8.88576588, 8.88576589),
    'sv_angle_1' : (0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5),
    'sv_angle_2' : (0.0, 0.5, 1.0, 1.5, 2.0, 2.5, 3.0, 3.5, 4.0, 4.5),
    'r(b, alpha)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25, 1.50),
    'r(inv b, inv alpha)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25),
    'r(inv b, gamma)' : (0.5, 0.7, 0.9, 1.1, 1.3, 1.5),
    'r(b, inv gamma)' : (0.1, 0.3, 0.5, 0.7, 0.9, 1.1),
    'r(b, beta)' : (0.95, 1.00, 1.05, 1.10, 1.15, 1.20, 1.25),
    'r(b, sv_edge_0)' : (0.5, 0.7, 0.9, 1.1, 1.3, 1.5),
    'r(b, sv_edge_1)' : (0.6, 0.7, 0.8, 0.9, 1.0, 1.1),
    'r(inv b, sv_angle_1)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25),
    'r(b, inv sv_angle_1)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25, 1.50),
    'r(alpha, gamma)' : (0.85, 0.90, 0.95, 1.00, 1.05, 1.10, 1.15, 1.20, 1.25),
    'r(inv alpha, sv_edge_1)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25),
    'r(alpha, inv sv_edge_1)' : (0.25, 0.50, 0.75, 1.00, 1.25, 1.50),
    'r(inv alpha, inv sv_angle_1)' : (0.5, 0.6, 0.7, 0.8, 0.9, 1.0),
    'r(gamma, sv_edge_1)' : (0.50, 0.75, 1.00, 1.25),
    'r(inv gamma, inv sv_edge_1)' : (0.1, 0.3, 0.5, 0.7, 0.9, 1.1),
    'r(inv beta, sv_edge_0)' : (0.6, 0.7, 0.8, 0.9, 1.0, 1.1),
    'r(beta, inv sv_edge_0)' : (0.6, 0.7, 0.8, 0.9, 1.0, 1.1),
    'r(inv beta, sv_edge_1)' : (0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0),
    'r(beta, inv sv_edge_1)' : (0.95, 1.00, 1.05, 1.10, 1.15, 1.20, 1.25),
    'r(inv sv_edge_0, sv_edge_1)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25),
    'r(sv_edge_0, inv sv_edge_1)' : (0.50, 0.75, 1.00, 1.25),
    'r(sv_edge_1, sv_angle_1)' : (0.00, 0.25, 0.50, 0.75, 1.00, 1.25),
    'r(inv sv_edge_1, inv sv_angle_1)' : (
        0.00,
        0.25,
        0.50,
        0.75,
        1.00,
        1.25,
        1.50
    ),
    'l_0' : (0.0, 1000.0, 2000.0, 3000.0, 4000.0, 5000.0, 6000.0),
    'l_0_log10' : (2.0, 2.5, 3.0, 3.5),
    'l_0_inv' : (0.000, 0.005, 0.010, 0.015, 0.020),
    'l_0_90_pc' : (
        0.0,
        100.0,
        200.0,
        300.0,
        400.0,
        500.0,
        600.0,
        700.0,
        800.0,
        900.0
    )
}

# Set tick labels for plotting data from dataframes.
ticklabels = {
    'x_0' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['x_0'][i])
            for i in range(len(ticks['x_0']))
    ),
    'y_0' : tuple(
        "$ {tick:d} $".format(tick = int(round(ticks['y_0'][i])))
            for i in range(len(ticks['y_0']))
    ),
    'x_1' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['x_1'][i])
            for i in range(len(ticks['x_1']))
    ),
    'x_1_thirds' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['x_1_thirds'][i])
            for i in range(len(ticks['x_1_thirds']))
    ),
    'y_1' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['y_1'][i])
            for i in range(len(ticks['y_1']))
    ),
    'y_1_thirds' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['y_1_thirds'][i])
            for i in range(len(ticks['y_1_thirds']))
    ),
    'x_2' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['x_2'][i])
            for i in range(len(ticks['x_2']))
    ),
    'y_2' : tuple(
        "$ {tick:d} $".format(tick = int(round(ticks['y_2'][i])))
            for i in range(len(ticks['y_2']))
    ),
    'a' : tuple(
        "$ {tick:d} $".format(tick = int(round(ticks['a'][i])))
            for i in range(len(ticks['a']))
    ),
    'c' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['c'][i])
            for i in range(len(ticks['c']))
    ),
    'b' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['b'][i])
            for i in range(len(ticks['b']))
    ),
    'alpha' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['alpha'][i])
            for i in range(len(ticks['alpha']))
    ),
    'gamma' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['gamma'][i])
            for i in range(len(ticks['gamma']))
    ),
    'beta' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['beta'][i])
            for i in range(len(ticks['beta']))
    ),
    'sv_edge_0' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['sv_edge_0'][i])
            for i in range(len(ticks['sv_edge_0']))
    ),
    'sv_edge_1' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['sv_edge_1'][i])
            for i in range(len(ticks['sv_edge_1']))
    ),
    'sv_edge_2' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['sv_edge_2'][i])
            for i in range(len(ticks['sv_edge_2']))
    ),
    'sv_angle_0' : tuple(
        "$ {tick:d} $".format(
            tick = int(round(1.0e+8 * (ticks['sv_angle_0'][i] - 8.8857658)))
        ) for i in range(len(ticks['sv_angle_0']))
    ),
    'sv_angle_1' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['sv_angle_1'][i])
            for i in range(len(ticks['sv_angle_1']))
    ),
    'sv_angle_2' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['sv_angle_2'][i])
            for i in range(len(ticks['sv_angle_2']))
    ),
    'r(b, alpha)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(b, alpha)'][i])
            for i in range(len(ticks['r(b, alpha)']))
    ),
    'r(inv b, inv alpha)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(inv b, inv alpha)'][i])
            for i in range(len(ticks['r(inv b, inv alpha)']))
    ),
    'r(inv b, gamma)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(inv b, gamma)'][i])
            for i in range(len(ticks['r(inv b, gamma)']))
    ),
    'r(b, inv gamma)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(b, inv gamma)'][i])
            for i in range(len(ticks['r(b, inv gamma)']))
    ),
    'r(b, beta)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(b, beta)'][i])
            for i in range(len(ticks['r(b, beta)']))
    ),
    'r(b, sv_edge_0)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(b, sv_edge_0)'][i])
            for i in range(len(ticks['r(b, sv_edge_0)']))
    ),
    'r(b, sv_edge_1)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(b, sv_edge_1)'][i])
            for i in range(len(ticks['r(b, sv_edge_1)']))
    ),
    'r(inv b, sv_angle_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(inv b, sv_angle_1)'][i])
            for i in range(len(ticks['r(inv b, sv_angle_1)']))
    ),
    'r(b, inv sv_angle_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(b, inv sv_angle_1)'][i])
            for i in range(len(ticks['r(b, inv sv_angle_1)']))
    ),
    'r(alpha, gamma)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(alpha, gamma)'][i])
            for i in range(len(ticks['r(alpha, gamma)']))
    ),
    'r(inv alpha, sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(inv alpha, sv_edge_1)'][i])
            for i in range(len(ticks['r(inv alpha, sv_edge_1)']))
    ),
    'r(alpha, inv sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(alpha, inv sv_edge_1)'][i])
            for i in range(len(ticks['r(alpha, inv sv_edge_1)']))
    ),
    'r(inv alpha, inv sv_angle_1)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(inv alpha, inv sv_angle_1)'][i])
            for i in range(len(ticks['r(inv alpha, inv sv_angle_1)']))
    ),
    'r(gamma, sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(gamma, sv_edge_1)'][i])
            for i in range(len(ticks['r(gamma, sv_edge_1)']))
    ),
    'r(inv gamma, inv sv_edge_1)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(inv gamma, inv sv_edge_1)'][i])
            for i in range(len(ticks['r(inv gamma, inv sv_edge_1)']))
    ),
    'r(inv beta, sv_edge_0)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(inv beta, sv_edge_0)'][i])
            for i in range(len(ticks['r(inv beta, sv_edge_0)']))
    ),
    'r(beta, inv sv_edge_0)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(beta, inv sv_edge_0)'][i])
            for i in range(len(ticks['r(beta, inv sv_edge_0)']))
    ),
    'r(inv beta, sv_edge_1)' : tuple(
        "$ {tick:.1f} $".format(tick = ticks['r(inv beta, sv_edge_1)'][i])
            for i in range(len(ticks['r(inv beta, sv_edge_1)']))
    ),
    'r(beta, inv sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(beta, inv sv_edge_1)'][i])
            for i in range(len(ticks['r(beta, inv sv_edge_1)']))
    ),
    'r(inv sv_edge_0, sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(inv sv_edge_0, sv_edge_1)'][i])
            for i in range(len(ticks['r(inv sv_edge_0, sv_edge_1)']))
    ),
    'r(sv_edge_0, inv sv_edge_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(sv_edge_0, inv sv_edge_1)'][i])
            for i in range(len(ticks['r(sv_edge_0, inv sv_edge_1)']))
    ),
    'r(sv_edge_1, sv_angle_1)' : tuple(
        "$ {tick:.2f} $".format(tick = ticks['r(sv_edge_1, sv_angle_1)'][i])
            for i in range(len(ticks['r(sv_edge_1, sv_angle_1)']))
    ),
    'r(inv sv_edge_1, inv sv_angle_1)' : tuple(
        "$ {tick:.2f} $".format(
            tick = ticks['r(inv sv_edge_1, inv sv_angle_1)'][i]
        ) for i in range(len(ticks['r(inv sv_edge_1, inv sv_angle_1)']))
    ),
    'l_0' : tuple(
        "$ {tick:d} $".format(tick = int(round(ticks['l_0'][i])))
            for i in range(len(ticks['l_0']))
    ),
    'l_0_log10' : tuple(
        "$ {tick_d:.2f} \\cdot 10^{{{tick_e:d}}} $".format(
            tick_d = (
                10.0 ** ticks['l_0_log10'][i] /
                10.0 ** int(round(_math.floor(ticks['l_0_log10'][i])))
            ),
            tick_e = int(round(_math.floor(ticks['l_0_log10'][i])))
        ) for i in range(len(ticks['l_0_log10']))
    ),
    'l_0_inv' : tuple(
        "$ {tick:.3f} $".format(tick = ticks['l_0_inv'][i])
            for i in range(len(ticks['l_0_inv']))
    ),
    'l_0_90_pc' : tuple(
        "$ {tick:d} $".format(tick = int(round(ticks['l_0_90_pc'][i])))
            for i in range(len(ticks['l_0_90_pc']))
    )
}
try:
    del i
except (NameError, UnboundLocalError):
    pass
