import pandas as pd
import matplotlib.pyplot as plt

# Load the data from CSV
results_df = pd.read_csv('execution_times.csv')  # Ensure the file name matches your CSV file

# Group by 'Size' and 'Method' and calculate the mean execution time
avg_times_df = results_df.groupby(['Size', 'Method']).mean().reset_index()

# Pivot the table to have 'Size' as index, 'Method' as columns, and 'ExecutionTime(ms)' as values
pivot_df = avg_times_df.pivot(index='Size', columns='Method', values='ExecutionTime(ms)')

# Plot raw performance times
fig_raw_performance, ax_raw_performance = plt.subplots(figsize=(10, 6))
pivot_df.plot(kind='line', ax=ax_raw_performance)
ax_raw_performance.set_title('Raw Performance Times')
ax_raw_performance.set_xlabel('Size')
ax_raw_performance.set_ylabel('Average Execution Time (ms)')
ax_raw_performance.set_xticklabels(pivot_df.index, rotation=0)
ax_raw_performance.grid(True, which='both', linestyle='--', linewidth=0.7)
ax_raw_performance.legend(title="Method")
fig_raw_performance.tight_layout()

# Save the raw performance times plot
fig_raw_performance.savefig('Raw_Performance_Times.png')

# Calculate speed-up
pivot_df['SpeedUp'] = pivot_df['serial'] / pivot_df['parallel']

# Plot speed-up
fig_speed_up, ax_speed_up = plt.subplots(figsize=(10, 6))
pivot_df['SpeedUp'].plot(kind='line', ax=ax_speed_up, color='skyblue')
ax_speed_up.set_title('Speed-Up of Parallel vs Serial')
ax_speed_up.set_xlabel('Size')
ax_speed_up.set_ylabel('Speed-Up Factor')
ax_speed_up.axhline(1, color='gray', linewidth=1.5, linestyle='--')  # Line indicating no speed-up
ax_speed_up.set_xticklabels(pivot_df.index, rotation=0)
ax_speed_up.grid(True, which='both', linestyle='--', linewidth=0.7)
fig_speed_up.tight_layout()

# Save the speed-up plot
fig_speed_up.savefig('Speed_Up_Comparison.png')
