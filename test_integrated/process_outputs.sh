#!/bin/bash

# Ensure the script exits if any command fails
set -e

# Define the input files
BASH_OUTPUT=".bash_output.txt"
MINISHELL_OUTPUT=".minishell_output.txt"
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

sed -i '/exit$/d' "$BASH_OUTPUT" "$MINISHELL_OUTPUT"

# Remove the first line from bash_output.txt if it contains PS1='$USER@minishell $> '
first_line=$(head -n 1 "$BASH_OUTPUT")
if [[ "$first_line" == *"PS1='\$USER@minishell \$> '"* ]]; then
    # Remove the first line
    sed -i '1d' "$BASH_OUTPUT"
fi

# Remove the last line from both output files if it contains both '#' and '$> ' (prompt)
for file in "$BASH_OUTPUT" "$MINISHELL_OUTPUT"; do
    last_line=$(tail -n 1 "$file")
    if [[ "$last_line" == *"@minishell"* && "$last_line" == *"\$> "* ]]; then
        # Remove the last line
        sed -i '$d' "$file"
    fi
done

# Remove lines containing 'jeportie@minishell $> echo $?'
for file in "$BASH_OUTPUT" "$MINISHELL_OUTPUT"; do
    sed -i '/jeportie@minishell \$> echo \$?/d' "$file"
done
