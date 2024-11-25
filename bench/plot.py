#!/usr/bin/env python3
# coding=utf-8

import json
import matplotlib.pyplot as plt
import numpy as np

x_point = np.array([])
y_mean = np.array([])
y_p99 = np.array([])
y_right_rate = np.array([])
arr = []

i = 500
while i < 5000:
    # load metrics{i}.json file as json, append to arr
    with open(f'metrics{i}.json', 'r') as f:
        data = json.load(f)
    arr.append(data)
    x_point = np.append(x_point, i)
    y_mean = np.append(y_mean, float(data['latencies']['mean'])/1e6)
    y_p99 = np.append(y_p99, float(data['latencies']['99th'])/1e6)
    y_right_rate = np.append(y_right_rate, float(data['status_codes']['200'])/float(data['requests']))

    i += 500

# draw and save to mean.png
fig, axs = plt.subplots(1, 2)
axs[0].plot(x_point, y_mean, label='mean')
axs[0].plot(x_point, y_p99, label='p99')
axs[0].legend()
axs[0].set_xlabel('QPS')
axs[0].set_ylabel('Latency(ms)')
axs[0].set_title('Latency vs QPS')

# draw right rate
axs[1].plot(x_point, y_right_rate, label='right rate')
axs[1].legend()
axs[1].set_xlabel('QPS')
axs[1].set_ylabel('Right Rate')
axs[1].set_title('Right Rate vs QPS')

fig.tight_layout()
plt.savefig('result.png')
# plt.show()
