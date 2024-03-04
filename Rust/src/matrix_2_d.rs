// Troubleshooting advice for parallel running from:
// https://users.rust-lang.org/t/par-iter-not-running-in-parallel/67582

use rand::{distributions::Uniform, Rng};
use rayon::prelude::*;
use rayon::ThreadPoolBuilder;

// Define a public `Matrix` struct that will hold the data for a 2D matrix.
// The matrix data is stored as a vector of vectors of unsigned 32-bit integers (`Vec<Vec<u32>>`).
pub struct Matrix {
    pub data: Vec<Vec<u32>>,
}

impl Matrix {

    // A public method to create a new `Matrix` instance with randomly generated numbers.
    // It takes two parameters, `rows` and `cols`, specifying the dimensions of the matrix.
    pub fn new_random(rows: usize, cols: usize) -> Self {

        // Create a new random number generator thread.
        let mut rng = rand::thread_rng();

        // Define a uniform distribution range from 1 to 20, inclusive.
        let range = Uniform::from(1..=20);

        // Generate the matrix data: a vector of `rows` vectors, each containing `cols` random numbers
        let data = (0..rows)
            .map(|_| (0..cols).map(|_| rng.sample(&range)).collect())
            .collect();

        // Return a new `Matrix` instance containing the generated data.
        Matrix { data }
    }

    // It takes a reference to another `Matrix` as a parameter and returns a new `Matrix` that is
    // the result of multiplying `self` with the other matrix.
    pub fn multiply_sequential(&self, other: &Matrix) -> Matrix {

        // Determine the number of rows and columns in the matrices.
        let rows_a = self.data.len();
        let cols_a = self.data[0].len();
        let cols_b = other.data[0].len();

        // Initialize the result matrix with zeros, having dimensions that match the resulting matrix size.
        let mut result_data = vec![vec![0; cols_b]; rows_a];

        // Perform the multiplication using three nested loops.
        for i in 0..rows_a {
            for j in 0..cols_b {
                for k in 0..cols_a {

                    // Accumulate the product of corresponding elements in the result matrix.
                    result_data[i][j] += self.data[i][k] * other.data[k][j];
                }
            }
        }
        // Return a new `Matrix` containing the result.
        Matrix { data: result_data }
    }

    // This function performs parallel 2D matrix multiplication using a specified number of threads.
    pub fn rank2_tensor_mult_thread(&self, other: &Matrix, num_cores: usize) -> Matrix {

        // Creates a thread pool with a specified number of threads (`num_cores`). 
        // `unwrap()` is called to get the result of `build()`, which may panic if the thread pool cannot be created.
        let pool = ThreadPoolBuilder::new().num_threads(num_cores).build().unwrap();
    
        // Transposes the `other` matrix to facilitate efficient row-by-column multiplication.
        // It iterates over the columns of `other` and collects each column into a new row, effectively transposing the matrix.
        let b_transposed: Vec<Vec<u32>> = (0..other.data[0].len())
            .map(|i| other.data.iter().map(|row| row[i]).collect())
            .collect();
    
        // Uses the thread pool to execute the closure in parallel.
        pool.install(|| {

            // Maps each row of `self.data` in parallel (using `par_iter()` from Rayon) to a new row in the result matrix.
            let result_data: Vec<Vec<u32>> = self.data.par_iter().map(|a_row| {

                // For each row in `a_row`, iterates over each row in `b_transposed` to compute the dot product.
                b_transposed.iter().map(|b_row| {

                    // Calculates the dot product of `a_row` and `b_row`.
                    // `zip()` pairs up elements from `a_row` and `b_row`, then `map()` applies the multiplication to each pair.
                    // `sum()` aggregates the results of the multiplications into a single value, which is the element of the resulting matrix.
                    a_row.iter().zip(b_row).map(|(a, b)| a * b).sum()
                }).collect() 
            }).collect(); 
    
            // Returns a new `Matrix` instance containing the result of the multiplication.
            Matrix { data: result_data }
        })
    }
}
