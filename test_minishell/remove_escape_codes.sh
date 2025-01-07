#!/bin/bash

# Ensure that at least 2 args are passed as arguments
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <file1> <file2> [<file3> ...]"
    exit 1
fi

# Process each file
for input_file in "$@"; do
    # Function to remove escape codes
    remove_escape_codes() {
        local input_file="$1"
        local temp_file="${input_file}.cleaned"
    
        # Remove specific escape sequences (like ^[[?2004l and ^[[?2004h)
        sed -E 's/\x1B\[[0-9;?]*[a-zA-Z]//g' "$input_file" > "$temp_file"
    
        # Overwrite the original file
        mv "$temp_file" "$input_file"
    }
    
    # Function to suppress leading "[?" or "[?[?" patterns
    suppress_leading_patterns() {
        local input_file="$1"
        local temp_file="${input_file}.cleaned"
    
        # Remove "[?" or "[?[?" sequences at the start of lines repeatedly
        sed -E ':a; s/^(\[\?)+//; ta' "$input_file" > "$temp_file"
    
        # Overwrite the original file
        mv "$temp_file" "$input_file"
    }
    
    # Process the files
    remove_escape_codes "$input_file"
    suppress_leading_patterns "$input_file"
done 
