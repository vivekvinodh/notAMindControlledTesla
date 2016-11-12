#!/usr/bin/env bash

# Create environment.
virtualenv venv

# Start environment.
source ./venv/bin/activate

# Install dependencies
pip install -U pip
pip install -U numpy
pip install -U pyserial
