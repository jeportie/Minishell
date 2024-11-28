#!/bin/bash

# Ensure the script exits if any command fails
set -e

# Define the input files
BASH_OUTPUT="bash_output.txt"
MINISHELL_OUTPUT="minishell_output.txt"
COMMANDS_FILE="test_commands.txt"

# Check if all required files exist
for file in "$BASH_OUTPUT" "$MINISHELL_OUTPUT" "$COMMANDS_FILE"; do
    if [[ ! -f "$file" ]]; then
        echo "Error: File '$file' not found."
        exit 1
    fi
done

# Step 1: Clean hidden characters from the files
clean_files() {
    sed -i 's/\r//g' "$1"  # Remove carriage return characters (^M)
    sed -i 's/\x1B\[[0-9;]*[a-zA-Z]//g' "$1"  # Remove ANSI escape codes
}

clean_files "$BASH_OUTPUT"
clean_files "$MINISHELL_OUTPUT"

# Initialize an empty sed script
sed_script=""

# Step 3: Read each command from test_commands.txt and construct exact match removal patterns
while IFS= read -r cmd || [[ -n "$cmd" ]]; do
    # Skip empty lines
    [[ -z "$cmd" ]] && continue

    # Escape special characters for sed
    escaped_cmd=$(printf '%s\n' "$cmd" | sed 's/[\/&]/\\&/g')

    # Append a line deletion command for exact matches
    sed_script="${sed_script}; /^${escaped_cmd}$/d"
done < "$COMMANDS_FILE"

# Step 4: Add the removal patterns for "echo $?" and "exit"
sed_script="${sed_script}; /echo \$?/d"
sed_script="${sed_script}; /^exit$/d"

# Remove the leading semicolon from the sed script
sed_script=${sed_script#; }

# Step 5: Apply the sed script to both output files
sed -i "$sed_script" "$BASH_OUTPUT" "$MINISHELL_OUTPUT"

tail -n +3 "$BASH_OUTPUT" > tmp_file && mv tmp_file "$BASH_OUTPUT"

echo "Truncation complete. Specified lines have been removed from '$BASH_OUTPUT' and '$MINISHELL_OUTPUT'."
