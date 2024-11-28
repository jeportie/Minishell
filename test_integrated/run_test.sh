#!/bin/bash

# run_tests.sh
# Script to automate testing of the 42 School Minishell project

# --- Step 0: Enable Debugging ---
#set -e  # Exit immediately if a command exits with a non-zero status
#set -x  # Uncomment for script debugging

# Define variables for files
BASH_OUTPUT="bash_output.txt"
MINISHELL_OUTPUT="minishell_output.txt"
COMMANDS_FILE="test_commands.txt"
DIFF_FILE="diff.txt"

# ----------------------------
# Color Codes for Formatting
# ----------------------------
GREEN="\033[1;32m"
RED="\033[1;31m"
YELLOW="\033[1;33m"
BLUE="\033[1;34m"
CYAN="\033[1;36m"
RESET="\033[0m"

# ----------------------------
# ASCII Art Banner
# ----------------------------
print_banner() {
    echo -e "${BLUE}"
    echo "  __  __  _         _  _______          _   " 
    echo " |  \/  |(_)       (_)|__   __|        | |  " 
    echo " | \  / | _  _ __   _    | |  ___  ___ | |_ " 
    echo " | |\/| || || '_ \ | |   | | / _ \/ __|| __|"
    echo " | |  | || || | | || |   | ||  __/\__ \| |_ "
    echo " |_|  |_||_||_| |_||_|   |_| \___||___/ \__|"
    echo -e "${RESET}"
}

# ----------------------------
# Step 1: Clean Existing Output Files
# ----------------------------
clean_outputs() {
    echo -e "${CYAN}Cleaning existing output files...${RESET}"
    rm -f "$BASH_OUTPUT" "$MINISHELL_OUTPUT" "$DIFF_FILE"
    echo -e "${GREEN}Cleaned existing output files.${RESET}"
}

# ----------------------------
# Step 2: Build the Minishell Project
# ----------------------------
build_project() {
    echo -e "${CYAN}Building the project using make...${RESET}"
    
    # Capture the build output
    BUILD_LOG=$(mktemp)
    
    if make -C .. re > "$BUILD_LOG" 2>&1; then
        echo -e "${GREEN}Build successful.${RESET}"
        rm "$BUILD_LOG"  # Remove the temporary log file if build succeeds
    else
        echo -e "${RED}Build failed. Please check the error details below:${RESET}"
        cat "$BUILD_LOG"
        rm "$BUILD_LOG"
        exit 1
    fi
}

# ----------------------------
# Step 3: Run Expect Scripts
# ----------------------------
run_expect_scripts() {
    echo -e "${CYAN}Running bash Expect script...${RESET}"
    if ./run_bash.expect > /dev/null 2>&1; then
        echo -e "${GREEN}bash.expect executed successfully.${RESET}"
    else
        echo -e "${RED}bash.expect failed. Please check the script and permissions.${RESET}"
        exit 1
    fi

    echo -e "${CYAN}Running minishell Expect script...${RESET}"
    if ./run_minishell.expect > /dev/null 2>&1; then
        echo -e "${GREEN}minishell.expect executed successfully.${RESET}"
    else
        echo -e "${RED}minishell.expect failed. Please check the script and permissions.${RESET}"
        exit 1
    fi
}

# ----------------------------
# Step 4: Process Output Files
# ----------------------------
process_outputs() {
    echo -e "${CYAN}Processing output files...${RESET}"
    if ./process_outputs.sh > /dev/null 2>&1; then
        echo -e "${GREEN}Output files processed successfully.${RESET}"
    else
        echo -e "${RED}Processing output files failed. Please check process_outputs.sh.${RESET}"
        exit 1
    fi
}

# ----------------------------
# Step 5: Compare Outputs
# ----------------------------
compare_outputs() {
    echo -e "${CYAN}Comparing outputs...${RESET}"
    if diff "$BASH_OUTPUT" "$MINISHELL_OUTPUT" > "$DIFF_FILE"; then
        echo -e "${GREEN}No differences found.${RESET}"
    else
        echo -e "${YELLOW}Differences detected. Check diff.txt for details.${RESET}"
    fi
}

# ----------------------------
# Step 6: Parse and Report Test Results
# ----------------------------
report_results() {
    echo -e "\n${BLUE}========================================${RESET}"
    echo -e "${BLUE}           TEST RESULTS SUMMARY         ${RESET}"
    echo -e "${BLUE}========================================${RESET}\n"

    # Initialize counters
    total_tests=0
    tests_passed=0
    tests_failed=0

    # Initialize array to hold test results
    declare -a test_results=()

    # Open file descriptors for reading bash and minishell outputs
    exec 3< "$BASH_OUTPUT"
    exec 4< "$MINISHELL_OUTPUT"

    while true; do
        # Read 3 lines from bash_output.txt
        read -r bash_prompt_cmd <&3 || break
        read -r bash_output <&3 || break
        read -r bash_error <&3 || break

        # Read 3 lines from minishell_output.txt
        read -r mini_prompt_cmd <&4 || break
        read -r mini_output <&4 || break
        read -r mini_error <&4 || break

        # Increment total tests
        ((total_tests++))

        # Extract command from prompt lines
        bash_cmd=$(echo "$bash_prompt_cmd" | sed 's/.*\$> //')
        mini_cmd=$(echo "$mini_prompt_cmd" | sed 's/.*\$> //')

        # Use the command as the test name
        test_name="$bash_cmd"

        # Compare outputs and error codes
        if [[ "$bash_output" == "$mini_output" && "$bash_error" == "$mini_error" ]]; then
            # Test Passed
            test_results+=("$test_name|PASS|||")
            ((tests_passed++))
        else
            # Test Failed
            error_msg=""
            diff_output=""
            diff_error_code=""

            if [[ "$bash_output" != "$mini_output" ]]; then
                diff_output=$(diff <(echo "$bash_output") <(echo "$mini_output"))
            fi

            if [[ "$bash_error" != "$mini_error" ]]; then
                diff_error_code=$(diff <(echo "$bash_error") <(echo "$mini_error"))
            fi

            # Combine error messages and diffs
            if [[ -n "$diff_output" ]]; then
                error_msg+="Output mismatch. "
            fi
            if [[ -n "$diff_error_code" ]]; then
                error_msg+="Error code mismatch. "
            fi

            test_results+=("$test_name|FAIL|$error_msg|$diff_output|$diff_error_code")
            ((tests_failed++))
        fi
    done

    # Close file descriptors
    exec 3<&-
    exec 4<&-

    # Iterate over all tests and display results
    for result in "${test_results[@]}"; do
        IFS='|' read -r test_name status error_msg diff_output diff_error_code <<< "$result"
        if [ "$status" == "PASS" ]; then
            printf "\033[1;32m%-50s (PASS)\033[0m\n" "$test_name"
        else
            printf "\033[1;31m%-50s (FAIL)\033[0m\n" "$test_name"
            echo -e "    \033[1;33mError(s):\033[0m $error_msg"
            if [ -n "$diff_output" ]; then
                echo -e "    \033[1;33mOutput Differences:\033[0m"
                echo -e "      $diff_output"
            fi
            if [ -n "$diff_error_code" ]; then
                echo -e "    \033[1;33mError Code Differences:\033[0m"
                echo -e "      $diff_error_code"
            fi
        fi
    done

    echo -e "\n${BLUE}========================================${RESET}"
    echo -e "Total tests: $total_tests"
    echo -e "Passed: \033[1;32m$tests_passed\033[0m"
    echo -e "Failed: \033[1;31m$tests_failed\033[0m"
    echo -e "${BLUE}========================================${RESET}\n"

    # Print Total Test Time (Placeholder)
    echo -e "${CYAN}Total Test time (real) = 0.00 sec${RESET}"

    # Exit with appropriate status
    if [ "$tests_failed" -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
}

# ----------------------------
# Step 7: Main Execution Flow
# ----------------------------
main() {
    print_banner
    clean_outputs
    build_project
    run_expect_scripts
    process_outputs
    compare_outputs
    report_results
}

# Execute the main function
main

