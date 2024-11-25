#!/usr/bin/env python3
# coding=utf-8

import json
import matplotlib.pyplot as plt
import numpy as np
import os
import subprocess

x_point = np.array([])
y_mean = np.array([])
y_mean_gin = np.array([])
y_p99 = np.array([])
y_p99_gin = np.array([])
y_right_rate = np.array([])
y_right_rate_gin = np.array([])
y_wait = np.array([])
y_wait_gin = np.array([])

begin = 500
end = 5500
step = 500

i = begin
while i < end:
    # load metrics{i}.json file as json, append to arr
    with open(f'metrics{i}.json', 'r') as f:
        data = json.load(f)
    with open(f'metrics_gin_{i}.json', 'r') as f:
        data_gin = json.load(f)

    x_point = np.append(x_point, i)

    y_mean = np.append(y_mean, float(data['latencies']['mean'])/1e6)
    y_p99 = np.append(y_p99, float(data['latencies']['99th'])/1e6)
    y_right_rate = np.append(y_right_rate, float(
        data['status_codes']['200'])/float(data['requests']))
    y_wait = np.append(y_wait, float(data['wait']/1e6))

    y_mean_gin = np.append(y_mean_gin, float(
        data_gin['latencies']['mean'])/1e6)
    y_p99_gin = np.append(y_p99_gin, float(data_gin['latencies']['99th'])/1e6)
    y_right_rate_gin = np.append(y_right_rate_gin, float(
        data_gin['status_codes']['200'])/float(data_gin['requests']))
    y_wait_gin = np.append(y_wait_gin, float(data_gin['wait']/1e6))

    # remove file
    os.remove(f'metrics{i}.json')
    os.remove(f'metrics_gin_{i}.json')

    i += step

# get commit  hash
result = subprocess.run('git describe --tags --always',
                        shell=True, capture_output=True, text=True)

# draw and save
fig, axs = plt.subplots(2, 2, figsize=(15, 12))
axs[0, 0].plot(x_point, y_mean, label='cppnet')
axs[0, 0].plot(x_point, y_mean_gin, label='gin')
axs[0, 0].legend()
axs[0, 0].set_xlabel('QPS')
axs[0, 0].set_ylabel('Latency(ms)')
axs[0, 0].set_title('mean time of request')

# draw p99
axs[0, 1].plot(x_point, y_p99, label='cppnet')
axs[0, 1].plot(x_point, y_p99_gin, label='gin')
axs[0, 1].legend()
axs[0, 1].set_xlabel('QPS')
axs[0, 1].set_ylabel('Latency(ms)')
axs[0, 1].set_title('99th percentile of request')

# draw right rate
axs[1, 0].plot(x_point, y_right_rate, label='cppnet')
axs[1, 0].plot(x_point, y_right_rate_gin, label='gin')
axs[1, 0].legend()
axs[1, 0].set_xlabel('QPS')
axs[1, 0].set_ylabel('Right Rate')
axs[1, 0].set_title('right rate of request')

# draw throughput
axs[1, 1].plot(x_point, y_wait, label='cppnet')
axs[1, 1].plot(x_point, y_wait_gin, label='gin')
axs[1, 1].legend()
axs[1, 1].set_xlabel('QPS')
axs[1, 1].set_ylabel('Wait Time(ms)')
axs[1, 1].set_title('wait time of request')

fig.tight_layout()
fig.text(0.54, 0.958, 'commit hash '+result.stdout, ha='center', fontsize=12)
plt.savefig('result.png')
# plt.show()
