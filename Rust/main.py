import pandas as pd
import matplotlib.pyplot as plt

# Load the data from CSV
results_df = pd.read_csv('results.csv')

# Define core configurations
cores = ['Sequential', '2-Core', '3-Core', '4-Core']

# Convert time from nanoseconds to milliseconds
results_df[cores] = results_df[cores] / 1e6

# Function to calculate speed-up relative to sequential
def calc_speed_up(row, col):
    if col == 'Sequential':
        return 1  # Speed-up for Sequential is 1 (baseline)
    else:
        return row['Sequential'] / row[col]
    
# Initialize the figure and subplots
fig, axs = plt.subplots(1, 2, figsize=(20, 6))

# Loop over each subplot and matrix type (2D, 3D)
for ax, dtype in zip(axs, ['2D', '3D']):
    for dimension in results_df[results_df['Type'] == dtype]['Dimension'].unique():
        data_current = results_df[(results_df['Type'] == dtype) & (results_df['Dimension'] == dimension)]
        if not data_current.empty:
            speed_ups = [calc_speed_up(data_current.iloc[0], col) for col in cores]
            print(f"{dtype} {dimension}x{dimension}" + ("x" + str(dimension) if dtype == "3D" else ""))
            for core, speed_up in zip(cores, speed_ups):
                print(f"{core}: {speed_up:.2f}x speed-up")
            print()
            ax.plot(cores, speed_ups, marker='o', label=f'{dimension}x{dimension}' + ('x' + str(dimension) if dtype == '3D' else ''))

# Configure the subplots
for ax in axs:

    # Add a line at 0% to indicate the baseline
    ax.axhline(0, color='gray', linewidth=1.5)  

    ax.set_xlabel('Core Configuration')
    ax.set_ylabel('Speed-up Factor')
    ax.set_xticks(cores)
    ax.grid(True)
    ax.legend(title="Matrix Size", fontsize='large', title_fontsize='large')

axs[0].set_title('2D Matrix Multiplication Performance Relative to Sequential')
axs[1].set_title('3D Matrix Multiplication Performance Relative to Sequential')

# Save the plot for better visualization
plt.savefig('Rust plot.png')