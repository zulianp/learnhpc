#!/usr/bin/env python3

import pandas as pd
import matplotlib.pyplot as plt
import pdb
import numpy as np
import sys

# Specify the file path
file_path = sys.argv[2]

# Read the CSV file into a DataFrame
df = pd.read_csv(file_path,index_col=0, header=None).T

x_labels = df.columns[1:]  

x = df['Size'].values.astype(dtype=np.int64)

for key in df[0:0]:
    if key == "Size":
        continue
    y = df[key].values
    plt.plot(x, y, label=key)

# Add labels and title
plt.xlabel("Array size")
plt.ylabel(sys.argv[1])
plt.title(" Accumulate")
plt.xticks(x)

# Add legend
plt.legend()

# Add grid
plt.grid(True)

# # Show the plot
plt.savefig(sys.argv[3])
