import matplotlib.pyplot as plt
import numpy as np
import csv
import sys

second_file = 0

# Open:
arguments = sys.argv
if len(arguments) < 1:
	raise ValueError("(E) Error: No arguments provided!")
performance_file = open(arguments[1], 'r')
reader = csv.reader(performance_file, delimiter='\t')

if len(arguments) > 2:
	second_file = 1
	performance_file_2 = open(arguments[2], 'r')
	reader_2 = csv.reader(performance_file_2, delimiter='\t')

# Read:
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
y_min =  5000000
y_max = 18000000

# Plot:
plt.plot(median[0], median[1])
if second_file > 0:
	plt.plot(median_2[0], median_2[1])
plt.ylim((y_min, y_max))
plt.title("Nin")
plt.xlabel("Frame")
plt.ylabel("Median")
plt.show()
