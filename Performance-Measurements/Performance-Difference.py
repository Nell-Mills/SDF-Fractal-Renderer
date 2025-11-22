import sys

# Arguments:
# 1: Unoptimized
# 2: Optimized

# Get arguments and check number:
arguments = sys.argv
if len(arguments) < 3:
	raise ValueError("(E) Error: Not enough arguments provided!")

unopt = float(arguments[1])
opt = float(arguments[2])

print("Unoptimized: " + str(int(unopt)))
print("Optimized:   " + str(int(opt)))

difference = ((unopt - opt) / unopt) * 100.0

print("Difference:  " + str(round(difference, 2)))
