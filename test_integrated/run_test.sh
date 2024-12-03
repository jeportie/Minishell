#!/bin/bash

# run_test.sh
# Script to automate testing of the 42 School Minishell project

# --- Step 0: Enable Debugging ---
#set -e  # Exit immediately if a command exits with a non-zero status
# set -x  # Uncomment for script debugging

# Define variables for files
BASH_OUTPUT=".bash_output.txt"
MINISHELL_OUTPUT=".minishell_output.txt"
COMMANDS_FILE="test_commands.txt"

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
    echo -e "${CYAN}Creating empty output files...${RESET}"
    touch "$BASH_OUTPUT" "$MINISHELL_OUTPUT"
    echo -e "${GREEN}Output files cleaned and initialized.${RESET}"
}

# ----------------------------
# Step 2: Build the Minishell Project
# ----------------------------
build_project() {
    echo -e "${CYAN}Building the project using make...${RESET}"
    
    # Capture the build output
    BUILD_LOG=$(mktemp)
    make -C .. fullclean > "$BUILD_LOG" 
    make -C .. nodebug > "$BUILD_LOG"
    if make -C .. classic > "$BUILD_LOG" 2>&1; then
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
    ./remove_escape_codes.sh .bash_output.txt .minishell_output.txt
}

# ----------------------------
# Step 5: Parse and Report Test Results
# ----------------------------
report_results() {
    local elapsed_time="$1"  # Receive elapsed time as an argument

    echo -e "\n${BLUE}========================================${RESET}"
    echo -e "${BLUE}           TEST RESULTS SUMMARY         ${RESET}"
    echo -e "${BLUE}========================================${RESET}\n"

    # Initialize counters
    total_tests=0
    tests_passed=0
    tests_failed=0

    # Read commands into array
    mapfile -t commands < "$COMMANDS_FILE"

    # Function to parse outputs into an array of outputs per command
    parse_outputs() {
        local output_file="$1"
        local -n outputs_ref=$2  # Use nameref for output array

        outputs_ref=()  # Initialize the output array
        local current_output=""
        local is_first_prompt_found=false

        while IFS= read -r line || [[ -n "$line" ]]; do
            if [[ "$line" == *"jeportie@minishell \$>"* ]]; then
                # Found a prompt line
                if $is_first_prompt_found; then
                    outputs_ref+=("$current_output")
                else
                    is_first_prompt_found=true
                fi
                current_output="$line"$'\n'
            else
                current_output+="$line"$'\n'
            fi
        done < "$output_file"

        # Exclude the last output if it's from the 'exit' command
        if [[ -n "$current_output" ]]; then
            # Check if the current_output corresponds to 'exit'
            if [[ "$current_output" != *"exit"* ]]; then
                outputs_ref+=("$current_output")
            fi
        fi
    }

    # Parse bash outputs
    declare -a bash_outputs
    parse_outputs "$BASH_OUTPUT" bash_outputs

    # Parse minishell outputs
    declare -a minishell_outputs
    parse_outputs "$MINISHELL_OUTPUT" minishell_outputs

    # Check if the number of commands matches the number of outputs
    if [ "${#commands[@]}" -ne "${#bash_outputs[@]}" ] || [ "${#commands[@]}" -ne "${#minishell_outputs[@]}" ]; then
        echo -e "${RED}Error: Number of commands and outputs do not match.${RESET}"
        echo -e "Commands: ${#commands[@]}, Bash Outputs: ${#bash_outputs[@]}, Minishell Outputs: ${#minishell_outputs[@]}"
        exit 1
    fi

    # Iterate over each command
    for ((i=0; i<${#commands[@]}; i++)); do
        test_name="${commands[i]}"
        bash_output="${bash_outputs[i]}"
        mini_output="${minishell_outputs[i]}"

        # Increment total tests
        ((total_tests++))

        # Remove trailing newlines
        bash_output="$(echo -e "$bash_output" | sed '/^[[:space:]]*$/d')"
        mini_output="$(echo -e "$mini_output" | sed '/^[[:space:]]*$/d')"

        # Split outputs into lines
        IFS=$'\n' read -d '' -r -a bash_lines <<< "$bash_output"
        IFS=$'\n' read -d '' -r -a mini_lines <<< "$mini_output"

        # Remove the prompt line
        bash_prompt="${bash_lines[0]}"
        mini_prompt="${mini_lines[0]}"
        bash_lines=("${bash_lines[@]:1}")
        mini_lines=("${mini_lines[@]:1}")

        # Compare number of lines
        bash_line_count=${#bash_lines[@]}
        mini_line_count=${#mini_lines[@]}

        output_mismatch=false
        exit_code_mismatch=false
        output_number_mismatch=false

        # Check for output number mismatch
        if [ "$bash_line_count" -ne "$mini_line_count" ]; then
            output_number_mismatch=true
        else
            if [ "$bash_line_count" -eq 1 ]; then
                # Only exit code is present
                if [ "${bash_lines[0]}" != "${mini_lines[0]}" ]; then
                    exit_code_mismatch=true
                fi
            elif [ "$bash_line_count" -eq 2 ]; then
                # Output and exit code
                if [ "${bash_lines[0]}" != "${mini_lines[0]}" ]; then
                    output_mismatch=true
                fi
                if [ "${bash_lines[1]}" != "${mini_lines[1]}" ]; then
                    exit_code_mismatch=true
                fi
            else
                # More than 2 lines
                # Compare outputs except last line (assumed to be exit code)
                bash_output_content="${bash_lines[@]:0:${#bash_lines[@]}-1}"
                mini_output_content="${mini_lines[@]:0:${#mini_lines[@]}-1}"
                if [ "${bash_output_content[*]}" != "${mini_output_content[*]}" ]; then
                    output_mismatch=true
                fi
                # Compare exit codes
                if [ "${bash_lines[-1]}" != "${mini_lines[-1]}" ]; then
                    exit_code_mismatch=true
                fi
            fi
        fi

        # Determine test result
        if ! $output_mismatch && ! $exit_code_mismatch && ! $output_number_mismatch; then
            # Test Passed
            printf "\033[1;32m✓ %-50s\t(PASS)\033[0m\n" "$test_name"
            ((tests_passed++))
        else
            # Test Failed
            printf "\033[1;31m✘ %-45s\t\t(FAIL)\033[0m\n" "$test_name"

            # Display errors
            error_msg=""
            if $output_mismatch; then
                error_msg+="Output mismatch. "
            fi
            if $exit_code_mismatch; then
                error_msg+="Error code mismatch. "
            fi
            if $output_number_mismatch; then
                error_msg+="Output number mismatch. "
            fi

            echo -e "    \033[1;33mError(s):\033[0m $error_msg"

            # Display differences
            # Prepare outputs for diff
            bash_diff_content="${bash_lines[*]}"
            mini_diff_content="${mini_lines[*]}"

            if $output_number_mismatch; then
                echo -e "    \033[1;33mBash Output Lines:\033[0m $bash_line_count"
                echo -e "    \033[1;33mMinishell Output Lines:\033[0m $mini_line_count"
            fi

            if $output_mismatch || $exit_code_mismatch || $output_number_mismatch; then
                echo -e "    \033[1;33mDiff:\033[0m"
                diff_output=$(diff -u <(echo "${mini_lines[*]}") <(echo "${bash_lines[*]}"))
                # Indent and color the diff
                while IFS= read -r line; do
                    if [[ $line == ---* ]] || [[ $line == +++* ]]; then
                        continue  # Skip file headers in diff output
                    elif [[ $line == @@* ]]; then
                        echo -e "        \033[1;34m$line\033[0m"
                    elif [[ $line == -* ]]; then
                        echo -e "        \033[1;31m$line\033[0m"
                    elif [[ $line == +* ]]; then
                        echo -e "        \033[1;32m$line\033[0m"
                    else
                        echo -e "        $line"
                    fi
                done <<< "$diff_output"
            fi

            # Display exit code differences if they mismatch
            if $exit_code_mismatch; then
                bash_exit_code="${bash_lines[-1]}"
                mini_exit_code="${mini_lines[-1]}"
                echo -e "    \033[1;33mError Code Differences:\033[0m"
                echo -e "      \033[1;36mBash Error Code:\033[0m $bash_exit_code"
                echo -e "      \033[1;36mMinishell Error Code:\033[0m $mini_exit_code"
            fi

            ((tests_failed++))
        fi
    done

    # Print summary
    echo -e "\n${BLUE}========================================${RESET}"
    echo -e "Total tests: $total_tests"
    echo -e "Passed: \033[1;32m$tests_passed\033[0m"
    echo -e "Failed: \033[1;31m$tests_failed\033[0m"
    echo -e "${BLUE}========================================${RESET}\n"

    # Print Total Test Time with the captured elapsed_time
    echo -e "${CYAN}Total Test time (real) = ${elapsed_time} ms${RESET}"

    # Exit with appropriate status
    if [ "$tests_failed" -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
}

# ----------------------------
# Step 6: Main Execution Flow
# ----------------------------
main() {
    print_banner
    clean_outputs
    build_project

    # ----------------------------
    # Start Time Capture
    # ----------------------------
    start_time=$(date +%s%N)

    run_expect_scripts
    process_outputs

    # ----------------------------
    # End Time Capture
    # ----------------------------
    end_time=$(date +%s%N)

    # Calculate elapsed time in milliseconds
    elapsed=$(( (end_time - start_time)/1000000 ))

    # Pass elapsed time to report_results
    report_results "$elapsed"
    make -C .. fullclean
 }

# Execute the main function
main
