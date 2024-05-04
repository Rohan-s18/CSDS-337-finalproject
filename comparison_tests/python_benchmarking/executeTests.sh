#!/bin/bash

# Find all Python files in the current directory
python_files=$(find . -maxdepth 1 -type f -name "*.py")

# Run each Python file
for file in $python_files; do
    # Check if the file is executable, if not, change its permissions
    if [ ! -x "$file" ]; then
        chmod +x "$file"
    fi

    # Run the Python script
    python3 "$file"
done

