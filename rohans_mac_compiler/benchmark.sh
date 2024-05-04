#!/bin/bash

start_time=$(date +%s)

./executeTests.sh

end_time=$(date +%s)

elapsed_time=$((end_time - start_time))

echo "Total execution time: $elapsed_time seconds"
