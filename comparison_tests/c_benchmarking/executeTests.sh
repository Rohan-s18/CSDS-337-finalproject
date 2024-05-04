#!/bin/bash

# Compile and run all C programs in the current directory

# Find all C files in the current directory
c_files=$(find . -maxdepth 1 -type f -name "*.c")

# Compile and run each C file
for file in $c_files; do
    # Extract the filename without the extension
    filename=$(basename -- "$file")
    filename_no_ext="${filename%.*}"

    # Compile the C program
    gcc -o "$filename_no_ext" "$file"

    # Check if compilation was successful
    if [ $? -eq 0 ]; then
        echo "Compilation successful: $filename_no_ext"
        
        # Run the compiled program
        ./"$filename_no_ext"
    else
        echo "Compilation failed: $filename"
    fi
done

