#!/usr/bin/env python3

# Import and create alias for package
import numpy as np

def hello_type(dtype):
	# Wrap the type just to be sure to make if work if the user passes a string
	dtype = np.dtype(dtype)
	a1 = np.array([1, 2, 3], dtype=dtype)
	filename = f'array.{dtype.name}.raw'

	# Print the array content to the command line
	print(f'Content for {filename}: {a1}')

	# Write array content to disk as binary format
	a1.tofile(filename)

	# We read the binary format array and store it as 
	# a NumPy array with the same type as dtype
	a2 = np.fromfile(filename, dtype=dtype)

	# We compute the difference between the two arrays...
	diff = a1 - a2

	# and print it the the command line again. We expect [0, 0, 0]
	print(f'Diff {diff}')

# Integer
hello_type(np.int32)

# Long Integer
hello_type(np.int64)

# Single precision floating point number
hello_type(np.float32)

# Double precision floating point number
hello_type(np.float64)
