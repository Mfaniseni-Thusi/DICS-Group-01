import pandas as pd
import matplotlib.pyplot as plt

# Load the Rust data from CSV
rust_df = pd.read_csv('Rust/Rust_results.csv')

# Define core configurations
cores = ['Sequential', '2-Core', '3-Core', '4-Core']

# Convert time from nanoseconds to milliseconds for better readability
rust_df[cores] = rust_df[cores] / 1e6

# Function to calculate speed-up relative to sequential
def calc_speed_up(row):
    # Using .loc to avoid SettingWithCopyWarning
    return row.loc['Sequential'] / row.loc[cores]

# Calculate speed-ups for all rows and configurations
speed_up_df = rust_df.copy()
speed_up_df[cores] = speed_up_df.apply(calc_speed_up, axis=1)

# Initialize the figure and subplots for speed-up
fig_speed_up, axs_speed_up = plt.subplots(1, 2, figsize=(20, 6)) 

# Plot speed-up factor
for ax, dtype in zip(axs_speed_up, ['2D', '3D']):
    data_current = speed_up_df[speed_up_df['Type'] == dtype]
    for dimension in data_current['Dimension'].unique():
        subset = data_current[data_current['Dimension'] == dimension]
        ax.plot(cores, subset[cores].iloc[0], marker='o', label=f'{dimension}x{dimension}' + ('x' + str(dimension) if dtype == '3D' else ''))

for ax in axs_speed_up:
    ax.axhline(1, color='gray', linewidth=1.5, linestyle='--')
    ax.set_xlabel('Core Configuration')
    ax.set_ylabel('Speed-up Factor')
    ax.set_xticks(cores)
    ax.grid(True, which='both', linestyle='--', linewidth=0.7)
    ax.legend(title="Matrix Size", fontsize='large', title_fontsize='large')

axs_speed_up[0].set_title('2D Matrix Multiplication Performance Relative to Sequential')
axs_speed_up[1].set_title('3D Matrix Multiplication Performance Relative to Sequential')
fig_speed_up.tight_layout()
fig_speed_up.savefig('Speed_Up_Performance.png', dpi=300)


# Load the C data from CSV, assuming it's correctly formatted as per the new structure
c_df = pd.read_csv('C/matrix_performance.csv')

# Change to milliseconds
c_df[cores] = c_df[cores] * 1e3

# Plotting comparison
fig_compare, axs_compare = plt.subplots(1, 2, figsize=(20, 6))  # One row, two columns for 2D and 3D

matrix_sizes = ['10', '20', '30']  # Sizes as they appear in the Dimension column
matrix_types = ['2D', '3D']

for i, matrix_type in enumerate(matrix_types):
    for size in matrix_sizes:
        # Rust Data
        rust_data = rust_df[(rust_df['Type'] == matrix_type) & (rust_df['Dimension'] == int(size))]
        
        # C Data
        c_data = c_df[(c_df['Type'] == matrix_type) & (c_df['Dimension'] == int(size))]

        # Plot Rust Data
        if not rust_data.empty:
            axs_compare[i].plot(cores, rust_data.iloc[0][cores], marker='o', label=f'Rust {size}x{size}' + ('x' + size if matrix_type == '3D' else ''))

        # Plot C Data
        if not c_data.empty:
            axs_compare[i].plot(cores, c_data.iloc[0][cores].values, marker='x', linestyle='--', label=f'C {size}x{size}' + ('x' + size if matrix_type == '3D' else ''))

    axs_compare[i].set_title(f'{matrix_type} Matrix Multiplication')
    axs_compare[i].set_xlabel('Threads/Core Configuration')
    axs_compare[i].set_ylabel('Time (milliseconds)')
    axs_compare[i].legend()
    axs_compare[i].grid(True)

fig_compare.tight_layout()
plt.savefig('Raw_performance.png', dpi=300)

