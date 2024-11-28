#!/bin/bash

# --- Step 0: Enable Debugging ---
set -e  # Exit immediately if a command exits with a non-zero status
#set -x  # Uncomment for script debugging

# Define variables for files
BASH_OUTPUT="bash_output.txt"
MINISHELL_OUTPUT="minishell_output.txt"
COMMANDS_FILE="test_commands.txt"

# Arrays to store test results
declare -a test_results
total_tests=0
tests_passed=0
tests_failed=0

# --- Step 1: Clean Previous Outputs ---
echo "Cleaning up previous output files..."
rm -f "$BASH_OUTPUT" "$MINISHELL_OUTPUT"
echo "Previous output files removed (if they existed)."

# --- Step 2: Build Minishell ---
echo "Building the latest version of minishell..."
if ! make -C .. re; then
    echo "Build failed."
    exit 1
fi
echo "Build successful."

# --- Step 3: Run .expect Scripts ---
echo "Running bash tests..."
if ! expect run_bash.expect; then
    echo "Error: run_bash.expect failed."
    exit 1
fi

echo "Running minishell tests..."
if ! expect run_minishell.expect; then
    echo "Error: run_minishell.expect failed."
    exit 1
fi

# --- Step 4: Process Outputs ---
echo "Processing outputs..."
if ! bash process_outputs.sh; then
    echo "Error: process_outputs.sh failed."
    exit 1
fi

# --- Step 5: Read Test Commands ---
mapfile -t test_commands < "$COMMANDS_FILE"
total_tests=${#test_commands[@]}

# --- Step 6: Read Outputs into Arrays ---
declare -a bash_outputs
declare -a minishell_outputs

# Function to read outputs and group them into sets of 3 lines
read_outputs() {
    local file="$1"
    local -n output_array=$2
    local lines_per_test=3
    local line_num=0
    local test_num=0
    local buffer=()

    while IFS= read -r line; do
        buffer+=("$line")
        ((line_num++))

        if (( line_num % lines_per_test == 0 )); then
            output_array[test_num]=("${buffer[@]}")
            buffer=()
            ((test_num++))
        fi
    done < "$file"
}

read_outputs "$BASH_OUTPUT" bash_outputs
read_outputs "$MINISHELL_OUTPUT" minishell_outputs

# --- Step 7: Iterate Over Tests ---
for i in $(seq 0 $((total_tests - 1))); do
    test_name="${test_commands[$i]}"
    bash_test_output=("${bash_outputs[$i]}")
    minishell_test_output=("${minishell_outputs[$i]}")

    # Extract lines
    bash_prompt="${bash_test_output[0]}"
    bash_output="${bash_test_output[1]}"
    bash_error_code="${bash_test_output[2]}"

    minishell_prompt="${minishell_test_output[0]}"
    minishell_output="${minishell_test_output[1]}"
    minishell_error_code="${minishell_test_output[2]}"

    # Initialize test status
    status="PASS"
    error_msgs=()

    # Compare outputs
    if [ "$bash_output" != "$minishell_output" ]; then
        status="FAIL"
        error_msgs+=("Output differs")
    fi

    # Compare error codes
    if [ "$bash_error_code" != "$minishell_error_code" ]; then
        status="FAIL"
        error_msgs+=("Error code differs")
    fi

    # Prepare diff outputs
    diff_output=""
    if [ "$status" == "FAIL" ]; then
        diff_output=$(diff --color=always -u <(echo "$bash_output") <(echo "$minishell_output") || true)
        diff_error_code=$(diff --color=always -u <(echo "$bash_error_code") <(echo "$minishell_error_code") || true)
    fi

    # Store the result
    error_msg="${error_msgs[*]}"
    test_results+=("$test_name|$status|$error_msg|$diff_output|$diff_error_code")
    if [ "$status" == "PASS" ]; then
        ((tests_passed++))
    else
        ((tests_failed++))
    fi
done

# --- Step 8: Report Test Results ---
echo -e "\n\033[1;34m========================================\033[0m"
echo -e "\033[1;34m           TEST RESULTS SUMMARY         \033[0m"
echo -e "\033[1;34m========================================\033[0m"

for result in "${test_results[@]}"; do
    IFS='|' read -r test_name status error_msg diff_output diff_error_code <<< "$result"
    if [ "$status" == "PASS" ]; then
        printf "\033[1;32m%-50s (PASS)\033[0m\n" "$test_name"
    else
        printf "\033[1;31m%-50s (FAIL)\033[0m\n" "$test_name"
        echo -e "    \033[1;33mError(s):\033[0m $error_msg"
        if [ -n "$diff_output" ]; then
            echo -e "    \033[1;33mOutput Differences:\033[0m"
            echo -e "$diff_output"
        fi
        if [ -n "$diff_error_code" ]; then
            echo -e "    \033[1;33mError Code Differences:\033[0m"
            echo -e "$diff_error_code"
        fi
    fi
done

echo -e "\033[1;34m========================================\033[0m"
echo -e "Total tests: $total_tests"
echo -e "Passed: \033[1;32m$tests_passed\033[0m"
echo -e "Failed: \033[1;31m$tests_failed\033[0m"
echo -e "\033[1;34m========================================\033[0m"

# Exit with appropriate status
if [ "$tests_failed" -gt 0 ]; then
    exit 1
else
    exit 0
fi

