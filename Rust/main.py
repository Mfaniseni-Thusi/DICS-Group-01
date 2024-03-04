import pandas as pd
import matplotlib.pyplot as plt

# Load the data from CSV
results_df = pd.read_csv('results.csv')

# Define core configurations
cores = ['Sequential', '2-Core', '3-Core', '4-Core']

# Convert time from nanoseconds to milliseconds for better readability
results_df[cores] = results_df[cores] / 1e6

# Function to calculate speed-up relative to sequential
def calc_speed_up(row):
    return row['Sequential'] / row[cores]

# Calculate speed-ups for all rows and configurations
speed_up_df = results_df.copy()
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

# Plot raw performance time
fig_raw, axs_raw = plt.subplots(1, 2, figsize=(20, 6))

for ax, dtype in zip(axs_raw, ['2D', '3D']):
    data_current = results_df[results_df['Type'] == dtype]
    for dimension in data_current['Dimension'].unique():
        subset = data_current[data_current['Dimension'] == dimension]
        ax.plot(cores, subset[cores].iloc[0], marker='o', label=f'{dimension}x{dimension}' + ('x' + str(dimension) if dtype == '3D' else ''))

for ax in axs_raw:
    ax.set_xlabel('Core Configuration')
    ax.set_ylabel('Time (milliseconds)')
    ax.set_xticks(cores)
    ax.grid(True, which='both', linestyle='--', linewidth=0.7)
    ax.legend(title="Matrix Size", fontsize='large', title_fontsize='large')

axs_raw[0].set_title('2D Matrix Multiplication Raw Performance Time')
axs_raw[1].set_title('3D Matrix Multiplication Raw Performance Time')
fig_raw.tight_layout()
fig_raw.savefig('Raw_Performance.png', dpi=300)

