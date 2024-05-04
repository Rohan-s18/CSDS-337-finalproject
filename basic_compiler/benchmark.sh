#!/bin/bash

start_time=$(date +%s.%N)

./executeTests.sh

end_time=$(date +%s.%N)

# Calculate elapsed time in seconds with floating-point precision using awk
elapsed_time=$(awk "BEGIN {print $end_time - $start_time}")

echo "Total execution time: $elapsed_time seconds"


