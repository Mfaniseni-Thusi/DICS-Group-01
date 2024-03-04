import pandas as pd
import matplotlib.pyplot as plt

# Load the data from CSV
results_df = pd.read_csv('execution_times.csv')  # Ensure the file name matches your CSV file

# Pivot the table to have 'Size' as index, 'Method' as columns, and 'ExecutionTime(ms)' as values
# This is assuming that your CSV is already structured appropriately
pivot_df = results_df.pivot(index='Size', columns='Method', values='ExecutionTime(ms)')

# Plot for Serial and Parallel execution times
plt.figure(figsize=(10, 6))
for method in pivot_df.columns:
    plt.plot(pivot_df.index, pivot_df[method], marker='o', label=method.capitalize())

# Enhancements for the plot
plt.title('Execution Time by Method')
plt.xlabel('Size')
plt.ylabel('Execution Time (ms)')
plt.xticks(pivot_df.index)
plt.grid(True, which='both', linestyle='--', linewidth=0.7)
plt.legend(title="Method")

# Save the plot for Serial and Parallel execution times
plt.tight_layout()
plt.savefig('Execution_Time_by_Method.png')

# Calculate speed-up by dividing serial times by parallel times
pivot_df['SpeedUp'] = pivot_df['serial'] / pivot_df['parallel']

# Plot for Speed-Up
plt.figure(figsize=(10, 6))
plt.plot(pivot_df.index, pivot_df['SpeedUp'], marker='o', color='skyblue', label='SpeedUp')

# Enhancements for the plot
plt.title('Speed-Up of Parallel vs Serial')
plt.xlabel('Size')
plt.ylabel('Speed-Up Factor')
plt.axhline(1, color='gray', linewidth=1.5, linestyle='--')  # Line indicating no speed-up
plt.xticks(pivot_df.index)
plt.grid(True, which='both', linestyle='--', linewidth=0.7)
plt.legend()

# Save the plot for Speed-Up
plt.tight_layout()
plt.savefig('Speed_Up_Comparison.png')
