mod matrix_2_d; // Declare the matrix module
mod matrix_3_d;

use matrix_2_d::Matrix; // Use the Matrix struct from the matrix module
use matrix_3_d::Matrix3D;
use std::time::{Duration, Instant};

// run with: cargo run 

fn main() {

    let dimensions = [10, 20, 30];
    let runs = 50;

    for &dimension in &dimensions {
        test_matrix_multiplication_2d(dimension, runs);
    }

    for &dimension in &dimensions {
        test_matrix_multiplication_3d(dimension, runs);
    }
}

fn test_matrix_multiplication_2d(dimensions: usize, runs: usize) {
    let mut total_duration_sequential = Duration::new(0, 0);
    let mut total_duration_parallel = Duration::new(0, 0);
    let mut total_duration_over_parallel = Duration::new(0, 0);

    for _ in 0..runs {
        let matrix_a = Matrix::new_random(dimensions, dimensions);
        let matrix_b = Matrix::new_random(dimensions, dimensions);

        // Sequential multiplication
        let start = Instant::now();
        let _ = matrix_a.multiply_sequential(&matrix_b);
        total_duration_sequential += start.elapsed();

        // 2 - Core
        let start = Instant::now();
        let _ = matrix_a.multiply_parallel(&matrix_b);
        total_duration_parallel += start.elapsed();

        // 3 - Core
        let start = Instant::now();
        let _ = matrix_a.multiply_over_parallel(&matrix_b);
        total_duration_over_parallel += start.elapsed();
    }

    // Calculate average durations
    let average_duration_sequential = total_duration_sequential / runs as u32;
    let average_duration_parallel = total_duration_parallel / runs as u32;
    let average_duration_over_parallel = total_duration_over_parallel / runs as u32;

    // Print average computation times
    println!("\n2D Matrix Multiplication Test for {}x{} Matrix over {} Runs", dimensions, dimensions, runs);
    println!("Average time (Sequential): {:?}", average_duration_sequential);
    println!("Average time (2 Core): {:?}", average_duration_parallel);
    println!("Average time (3-Core): {:?}", average_duration_over_parallel);
}

fn test_matrix_multiplication_3d(dimensions: usize, runs: usize) {
    let mut total_duration_sequential = Duration::new(0, 0);
    let mut total_duration_2_core = Duration::new(0, 0);
    let mut total_duration_3_core = Duration::new(0, 0);
    let mut total_duration_4_core = Duration::new(0, 0);

    for _ in 0..runs {
        let matrix_3d_a = Matrix3D::new_random(dimensions, dimensions, dimensions);
        let matrix_3d_b = Matrix3D::new_random(dimensions, dimensions, dimensions);

        // Sequential
        let start = Instant::now();
        let _ = matrix_3d_a.multiply_sequential(&matrix_3d_b);
        total_duration_sequential += start.elapsed();

        // 2 Core
        let start = Instant::now();
        let _ = matrix_3d_a.multiply_2_core(&matrix_3d_b);
        total_duration_2_core += start.elapsed();

        // 3 Core
        let start = Instant::now();
        let _ = matrix_3d_a.multiply_3_core(&matrix_3d_b);
        total_duration_3_core += start.elapsed();

        // 4 Core
        let start = Instant::now();
        let _ = matrix_3d_a.multiply_4_core(&matrix_3d_b);
        total_duration_4_core += start.elapsed();
    }

    // Calculate average durations
    let average_duration_sequential = total_duration_sequential / runs as u32;
    let average_duration_2_core = total_duration_2_core / runs as u32;
    let average_duration_3_core = total_duration_3_core / runs as u32;
    let average_duration_4_core = total_duration_4_core / runs as u32;

    // Print average computation times
    println!("\n3D Matrix Multiplication Test for {}x{}x{} Matrix over {} Runs:", dimensions, dimensions, dimensions, runs);
    println!("Average time (Sequential): {:?}", average_duration_sequential);
    println!("Average time (2 Core): {:?}", average_duration_2_core);
    println!("Average time (3 Core): {:?}", average_duration_3_core);
    println!("Average time (4 Core): {:?}", average_duration_4_core);
}

