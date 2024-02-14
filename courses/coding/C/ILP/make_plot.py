#!/usr/bin/env python3


import matplotlib.pyplot as plt
import numpy as np

# Define file paths
file_paths = ["e1.txt", "e2.txt", "e3.txt", "e4.txt"]

# Create empty list for data
data_arrays = []

# Read data from each file
for file_path in file_paths:
    data = np.loadtxt(file_path)  # Adjust separator if needed
    data = data.astype(np.float32) / 1e6
    data_arrays.append(data)

# Create labels for each array
labels = ["C++ std lib", "C vanilla", "C DLP", "C ILP/DLP"]

# Create a line plot
plt.figure(figsize=(8, 6))  # Set the figure size
for i, data in enumerate(data_arrays):
    plt.plot(data, label=labels[i])

# Add labels and title
plt.xlabel("X-axis")
plt.ylabel("seconds")
plt.title(" Accumulate")
plt.yscale('log') 

# Add legend
plt.legend()

# Add grid
plt.grid(True)

# Show the plot
plt.show()
