#!/usr/bin/env bash

# Begin script

# Exit if an error is encountered
set -e

# Print every command to the command line
# set -x

# Create a python virtual environment (venv)
python3 -m venv venv

# Activate the python venv
source venv/bin/activate

# Install requirements for this course
pip install -r requirements.txt

# Deactivate the venv
deactivate

# End script
