mod matrix_2_d;
mod matrix_3_d;

use matrix_2_d::Matrix; 
use matrix_3_d::Matrix3D;

use std::fs::File;
use std::io::{Write, BufWriter};
use std::time::{Duration, Instant};

fn main() {
    
    let dimensions = [10, 20, 30];
    let runs = 100;

    println!("Values averaging across {} runs...", runs);

    // Open a file for writing
    let file = File::create("Rust_results.csv").expect("Could not create file");
    let mut writer = BufWriter::new(file);

    // Write headers
    writeln!(&mut writer, "Dimension,Type,Sequential,2-Core,3-Core,4-Core").expect("Could not write headers");

    for &dimension in &dimensions {
        test_matrix_multiplication_2d(dimension, runs, &mut writer);
    }

    for &dimension in &dimensions {
        test_matrix_multiplication_3d(dimension, runs, &mut writer);
    }

    println!("Write to csv success!");
}
fn test_matrix_multiplication_2d(dimensions: usize, runs: usize, writer: &mut BufWriter<File>) {
    let mut total_duration_sequential = Duration::new(0, 0);
    let mut total_duration_2_core = Duration::new(0, 0);
    let mut total_duration_3_core = Duration::new(0, 0);
    let mut total_duration_4_core = Duration::new(0, 0);

    for _ in 0..runs {
        let matrix_a = Matrix::new_random(dimensions, dimensions);
        let matrix_b = Matrix::new_random(dimensions, dimensions);

        // Sequential multiplication
        let start = Instant::now();
        let _ = matrix_a.multiply_sequential(&matrix_b);
        total_duration_sequential += start.elapsed();

        // Parallel multiplication with 2 cores
        let start = Instant::now();
        let _ = matrix_a.rank2_tensor_mult_thread(&matrix_b, 2); // Using 2 cores
        total_duration_2_core += start.elapsed();

        // Parallel multiplication with 3 cores
        let start = Instant::now();
        let _ = matrix_a.rank2_tensor_mult_thread(&matrix_b, 3); // Using 3 cores
        total_duration_3_core += start.elapsed();

        // Parallel multiplication with 4 cores
        let start = Instant::now();
        let _ = matrix_a.rank2_tensor_mult_thread(&matrix_b, 4); // Using 4 cores
        total_duration_4_core += start.elapsed();
    }

    let average_duration_sequential = total_duration_sequential / runs as u32;
    let average_duration_2_core = total_duration_2_core / runs as u32;
    let average_duration_3_core = total_duration_3_core / runs as u32;
    let average_duration_4_core = total_duration_4_core / runs as u32;

    // Print in CSV format
    writeln!(writer, "{},2D,{},{},{},{}", dimensions, average_duration_sequential.as_nanos(), average_duration_2_core.as_nanos(), average_duration_3_core.as_nanos(), average_duration_4_core.as_nanos()).expect("Could not write 2D data");
}

fn test_matrix_multiplication_3d(dimensions: usize, runs: usize, writer: &mut BufWriter<File>) {
    let mut total_duration_sequential = Duration::new(0, 0);
    let mut total_duration_2_core = Duration::new(0, 0);
    let mut total_duration_3_core = Duration::new(0, 0);
    let mut total_duration_4_core = Duration::new(0, 0);

    for _ in 0..runs {
        let matrix_3d_a = Matrix3D::new_random(dimensions, dimensions, dimensions);
        let matrix_3d_b = Matrix3D::new_random(dimensions, dimensions, dimensions);

        // Sequential multiplication
        let start = Instant::now();
        let _ = matrix_3d_a.multiply_sequential(&matrix_3d_b);
        total_duration_sequential += start.elapsed();

        // Parallel multiplication with 2 cores
        let start = Instant::now();
        let _ = matrix_3d_a.rank3_tensor_mult_thread(&matrix_3d_b, 2);
        total_duration_2_core += start.elapsed();

        // Parallel multiplication with 3 cores
        let start = Instant::now();
        let _ = matrix_3d_a.rank3_tensor_mult_thread(&matrix_3d_b, 3);
        total_duration_3_core += start.elapsed();

        // Parallel multiplication with 4 cores
        let start = Instant::now();
        let _ = matrix_3d_a.rank3_tensor_mult_thread(&matrix_3d_b, 4);
        total_duration_4_core += start.elapsed();
    }

    let average_duration_sequential = total_duration_sequential / runs as u32;
    let average_duration_2_core = total_duration_2_core / runs as u32;
    let average_duration_3_core = total_duration_3_core / runs as u32;
    let average_duration_4_core = total_duration_4_core / runs as u32;

    // Print in CSV format
    writeln!(writer, "{},3D,{},{},{},{}", dimensions, average_duration_sequential.as_nanos(), average_duration_2_core.as_nanos(), average_duration_3_core.as_nanos(), average_duration_4_core.as_nanos()).expect("Could not write 3D data");
}
