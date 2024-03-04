#!/bin/bash

# Navigate to the Rust project directory
cd Rust/

# Run the Rust program
cargo run

# Return to main project directory
cd ..

# Navigate to C project directory
cd C/

# Run the C program
gcc -o matrixProgram main.c matrix2D.c matrix3D.c -lpthread
./matrixProgram

# Return to main project directory
cd ..

# Execute the Python script for analysis
python main.py
