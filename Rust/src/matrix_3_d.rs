// Matrix multiplication assistance from:
// https://boydjohnson.dev/blog/concurrency-matrix-multiplication/

use rand::{distributions::Uniform, Rng};
use rayon::prelude::*;

pub struct Matrix3D {
    pub data: Vec<Vec<Vec<u32>>>, // Representing the 3D matrix as a vector of 2D matrices
}

impl Matrix3D {

    // Generates a new 3D matrix filled with random numbers within the specified range
    pub fn new_random(layers: usize, rows: usize, cols: usize) -> Self {
        let mut rng = rand::thread_rng();
        let range = Uniform::from(1..=20);

        let data = (0..layers)
            .map(|_| {
                (0..rows)
                    .map(|_| (0..cols).map(|_| rng.sample(&range)).collect())
                    .collect()
            })
            .collect();

        Matrix3D { data }
    }

    // Performs sequential (non-parallel) multiplication of corresponding 2D matrices from two 3D matrices
    pub fn multiply_sequential(&self, other: &Matrix3D) -> Matrix3D {
        assert_eq!(self.data.len(), other.data.len(), "The number of layers must be equal.");

        let result_data: Vec<Vec<Vec<u32>>> = self.data.iter().zip(other.data.iter())
            .map(|(a_layer, b_layer)| {

                // Transposing each 2D matrix in `other` for efficient dot product calculation
                let b_transposed: Vec<Vec<u32>> = (0..b_layer[0].len())
                    .map(|i| b_layer.iter().map(|row| row[i]).collect())
                    .collect();

                // Performing 2D matrix multiplication for each layer using sequential iteration
                a_layer.iter()
                    .map(|a_row| {
                        b_transposed.iter()
                            .map(|b_row| {
                                a_row.iter()
                                    .zip(b_row)
                                    .map(|(a_val, b_val)| a_val * b_val)
                                    .sum()
                            })
                            .collect()
                    })
                    .collect()
            })
            .collect();

        Matrix3D { data: result_data }
    }
    
    // A generic method for parallel matrix multiplication with a specified number of cores.
    pub fn rank3_tensor_mult_thread(&self, other: &Matrix3D, num_threads: usize) -> Matrix3D {
        assert_eq!(self.data.len(), other.data.len(), "The number of layers must be equal.");

        // Create a thread pool with the specified number of threads.
        let pool = rayon::ThreadPoolBuilder::new().num_threads(num_threads).build().unwrap();

        // Use the thread pool to perform the parallel multiplication.
        pool.install(|| {
            let result_data: Vec<Vec<Vec<u32>>> = self.data.par_iter().zip(other.data.par_iter())
                .map(|(a_layer, b_layer)| {
                    let b_transposed: Vec<Vec<u32>> = (0..b_layer[0].len())
                        .map(|i| b_layer.iter().map(|row| row[i]).collect())
                        .collect();
                    
                    // Performing 2D matrix multiplication for each layer.
                    a_layer.iter()
                        .map(|a_row| {
                            b_transposed.iter()
                                .map(|b_row| {
                                    a_row.iter()
                                        .zip(b_row)
                                        .map(|(a_val, b_val)| a_val * b_val)
                                        .sum()
                                })
                                .collect()
                        })
                        .collect()
                })
                .collect(); // Collects into Vec<Vec<Vec<u32>>>
        
            // Construct a Matrix3D from the result_data
            Matrix3D { data: result_data }
        })
    }
}

