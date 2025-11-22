import matplotlib.pyplot as plt
import numpy as np
import csv
import sys

# Arguments:
# 1: Y-Minimum
# 2: Y-Maximum
# 3: First File
# 4: Second File (optional)
# 5: 0 = Plot both lines, 1 = Plot gain of second file (choose the optimization one).
# 6: 0 = No line of best fit, 1 = Line of best fit on first set of data.

"""
Recommended Y limits:

Hall of Pillars:
Flythrough:	7000000, 18000000
Parameter:	      0, 42500000

Mandelbulb:
Parameter:	      0, 11000000
"""

# Get arguments and check number:
arguments = sys.argv
if len(arguments) < 4:
	raise ValueError("(E) Error: Not enough arguments provided!")

# Open first file:
performance_file = open(arguments[3], 'r')
reader = csv.reader(performance_file, delimiter='\t')

# Check for second file and open:
second_file = 0
if len(arguments) > 4:
	second_file = 1
	performance_file_2 = open(arguments[4], 'r')
	reader_2 = csv.reader(performance_file_2, delimiter='\t')

# Check if plotting two lines or the difference between them:
plot_difference = 0
if len(arguments) > 5:
	plot_difference = int(arguments[5])

# Check if plotting line of best fit:
plot_best_fit = 0
if len(arguments) > 6:
	plot_best_fit = int(arguments[6])

# Read files:
skip = 2
skipped = 0
data = []
for row in reader:
	if skipped < skip:
		skipped = skipped + 1
		continue
	data.append([int(row[0]), float(row[1]), float(row[2]), float(row[3])])

if second_file > 0:
	skip = 2
	skipped = 0
	data_2 = []
	for row in reader_2:
		if skipped < skip:
			skipped = skipped + 1
			continue
		data_2.append([int(row[0]), float(row[1]), float(row[2]), float(row[3])])

# Extract data:
data = np.array(data)
median = np.transpose(data[:,[0,1]])
minimum = np.transpose(data[:,[0,2]])
maximum = np.transpose(data[:,[0,3]])

if second_file > 0:
	data_2 = np.array(data_2)
	median_2 = np.transpose(data_2[:,[0,1]])
	minimum_2 = np.transpose(data_2[:,[0,2]])
	maximum_2 = np.transpose(data_2[:,[0,3]])

# Get plot limits:
y_min = int(arguments[1])
y_max = int(arguments[2])

plot_x = median[0]
plot_y = median[1]

if second_file > 0:
	plot_x_2 = median_2[0]
	plot_y_2 = median_2[1]

# Plot:
if plot_difference == 0:
	# Plot both lines:
	plt.plot(plot_x, plot_y, 'b')
	if second_file > 0:
		plt.plot(plot_x_2, plot_y_2, 'g')

	plt.title("")
	plt.ylim((y_min, y_max))
	plt.xlabel("Frame")
	plt.ylabel("Render Pass Time (ns)")
else:
	second_file = 0

	# Calculate percentage difference:
	plot_y = ((plot_y - plot_y_2) / plot_y) * 100

	# Plot difference line:
	plt.plot(plot_x, plot_y, 'b')

	# Plot line y = 0 to make difference easier to see:
	x_axis_line = [[min(plot_x) - 1000, max(plot_x) + 1000], [0, 0]]
	plt.plot(x_axis_line[0], x_axis_line[1], 'k')

	plt.title("")

	# Choose min and max without outliers:
	plot_y_new = plot_y
	for i in range(len(plot_y_new)):
		if abs(plot_y_new[i]) > 15:
			plot_y_new[i] = 0

	y_max = max(map(abs, plot_y_new)) + 1
	y_min = -y_max
	plt.ylim((y_min, y_max))
	plt.xlim((min(plot_x) - 500, max(plot_x) + 500))

	plt.xlabel("Frame")
	plt.ylabel("Percentage Difference in Median Render Pass Time")

# Line of best fit. First eliminate the outlier section in data (may not have been done already):
if (plot_best_fit == 1):
	plot_y_new = plot_y
	for i in range(len(plot_y_new)):
		if abs(plot_y_new[i]) > 15:
			plot_y_new[i] = 0
	degree = 15
	theta = np.polyfit(plot_x, plot_y_new, degree)
	line_of_best_fit = 0
	for i in range(0, degree + 1):
		line_of_best_fit += theta[degree - i] * pow(plot_x, i)

	plt.plot(plot_x, line_of_best_fit, 'r')

if second_file > 0:
	plt.legend(['Unoptimized', 'Temporal Cache'])

# Plot key frames:
"""key_frames_x = [0, 600, 1050, 1450, 1950, 2950, 4950, 6950,
		7050, 7250, 7600, 9000, 10000, 10100, 11299]
key_frames_y = []

for i in key_frames_x:
	for j in range(11300):
		if plot_x[j] == i:
			if plot_best_fit == 1:
				key_frames_y.append(line_of_best_fit[j])
			else:
				key_frames_y.append(plot_y[j])

plt.scatter(key_frames_x, key_frames_y, s=100, c='r', marker='x', zorder=5)"""

plt.show()
