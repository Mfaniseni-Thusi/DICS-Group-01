// Import the necessary modules from the `rand` crate for random number generation,
// and from `rayon` for parallel computing capabilities.
use rand::{distributions::Uniform, Rng};
use rayon::prelude::*;

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
        // sampled from the defined uniform distribution.
        let data = (0..rows)
            .map(|_| (0..cols).map(|_| rng.sample(&range)).collect())
            .collect();

        // Return a new `Matrix` instance containing the generated data.
        Matrix { data }
    }

    // A public method for performing sequential matrix multiplication.
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

    // A public method for performing parallel matrix multiplication.
    pub fn multiply_parallel(&self, other: &Matrix) -> Matrix {

        // Transpose the second matrix to optimize access patterns in the multiplication.
        let b_transposed: Vec<Vec<u32>> = (0..other.data[0].len())
            .map(|i| other.data.iter().map(|row| row[i]).collect())
            .collect();

        // Perform the matrix multiplication in parallel using Rayon's parallel iterators.
        // `.par_iter()` is used to iterate over rows of the first matrix in parallel.
        let result_data: Vec<Vec<u32>> = self.data.par_iter()
            .map(|a_row| {

                // For each row in the first matrix, iterate over each row in the transposed second matrix.
                b_transposed
                    .iter()
                    .map(|b_row| {

                        // Calculate the dot product of the row from the first matrix and the row from the transposed second matrix.
                        a_row
                            .iter()
                            .zip(b_row)
                            .map(|(a_val, b_val)| a_val * b_val)
                            .sum()
                    })
                    .collect()
            })
            .collect();

        // Return a new `Matrix` containing the result of the parallel multiplication.
        Matrix { data: result_data }
    }

    pub fn multiply_over_parallel(&self, other: &Matrix) -> Matrix {

        // Transpose the second matrix to optimize access patterns in the multiplication.
        let b_transposed: Vec<Vec<u32>> = (0..other.data[0].len())
            .map(|i| other.data.iter().map(|row| row[i]).collect())
            .collect();
    
        // Perform the matrix multiplication fully in parallel using Rayon's parallel iterators.
        // `.par_iter()` is used to iterate over rows of the first matrix in parallel.
        let result_data: Vec<Vec<u32>> = self.data.par_iter()
            .map(|a_row| {

                // Parallelize the iteration over each row in the transposed second matrix as well.
                b_transposed.par_iter()
                    .map(|b_row| {

                        // The dot product calculation is performed in parallel for each element of the resulting row.
                        a_row.par_iter()
                            .zip(b_row)
                            .map(|(a_val, b_val)| a_val * b_val)
                            .sum()
                    })
                    .collect()
            })
            .collect();
    
        // Return a new `Matrix` containing the result of the fully parallel multiplication.
        Matrix { data: result_data }
    }
    
}
