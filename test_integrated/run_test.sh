#!/bin/bash
# run_test.sh
# Script to automate testing of the 42 School Minishell project

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
    echo -e "${CYAN}"
    echo -e "            __  __  _         _  _______          _   " 
    echo -e "           |  \/  |(_)       (_)|__   __|        | |  ${GREEN}" 
    echo -e "           | \  / | _  _ __   _    | |  ___  ___ | |_" 
    echo -e "           | |\/| || || '_ \ | |   | | / _ \/ __|| __|${RED}"
    echo -e "           | |  | || || | | || |   | ||  __/\__ \| |_"
    echo -e "           |_|  |_||_||_| |_||_|   |_| \___||___/\___|"
    echo -e "${RESET}"
}

# ----------------------------
# Step 1: Clean Existing Output Files
# ----------------------------
clean_outputs() {
    echo -e "${CYAN}Cleaning existing output files...${RESET}"
    rm -f test_outputs/*.txt valgrind_outputs/*.txt
    echo -e "${GREEN}Output files cleaned.${RESET}"
}

# ----------------------------
# Step 2: Build the Minishell Project
# ----------------------------
build_project() {
    echo -e "${CYAN}Building the project using make...${RESET}"
    
    # Capture the build output
    BUILD_LOG=$(mktemp)
    make -C .. fullclean > "$BUILD_LOG" 
    make -C .. nodebug >> "$BUILD_LOG"
    if make -C .. classic >> "$BUILD_LOG" 2>&1; then
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
    # Find all test command files in test_commands/ directory
    test_files=(test_commands/*.txt)

    # Check if any test files are found
    if [ ${#test_files[@]} -eq 0 ]; then
        echo -e "${RED}No test command files found in test_commands/ directory.${RESET}"
        exit 1
    fi

    # Iterate over each test file
    for test_file in "${test_files[@]}"; do
        group_name=$(basename "$test_file" .txt)
        echo -e "${CYAN}Running bash Expect script for group '${group_name}'...${RESET}"
        if ./run_bash.expect "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}bash.expect executed successfully for group '${group_name}'.${RESET}"
        else
            echo -e "${RED}bash.expect failed for group '${group_name}'. Pease check the script and permissions.${RESET}"
            exit 1
        fi
        echo -e "${CYAN}Running minishell Expect script for group '${group_name}'...${RESET}"
        if ./run_minishell.expect "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}minishell.expect executed successfully for group '${group_name}'.${RESET}"
        else
            echo -e "${RED}minishell.expect failed for group '${group_name}'. Please check the script and permissions.${RESET}"
            exit 1
        fi
    done


}

# ----------------------------
# Step 4: Process Output Files
# ----------------------------
process_outputs() {
    echo -e "${CYAN}Processing output files...${RESET}"
    # Find all group names
    test_files=(test_commands/*.txt)
    group_names=()
    for test_file in "${test_files[@]}"; do
        group_names+=("$(basename "$test_file" .txt)")
    done

    # Process outputs for each group
    if ./process_outputs.sh "${group_names[@]}" > /dev/null 2>&1; then
        echo -e "${GREEN}Output files processed successfully.${RESET}"
    else
        echo -e "${RED}Processing output files failed. Please check process_outputs.sh.${RESET}"
        exit 1
    fi

    # Remove escape codes from all output files
    output_files=()
    for group_name in "${group_names[@]}"; do
        output_files+=("test_outputs/bash_output_${group_name}.txt" "test_outputs/minishell_output_${group_name}.txt")
    done
    ./remove_escape_codes.sh "${output_files[@]}"
}

# ----------------------------
# Step 5: Parse and Report Test Results
# ----------------------------
report_results() {
    local elapsed_time="$1"  # Receive elapsed time as an argument

    echo -e "\n${BLUE}============================================================${RESET}"
    echo -e "${BLUE}              TESTING MINISHELL VS POSIX SUMMARY            ${RESET}"
    echo -e "${BLUE}============================================================${RESET}\n"

    # Initialize overall counters
    total_tests=0
    tests_passed=0
    tests_failed=0

    # Initialize group results associative array
    declare -A group_results

    # Find all test command files
    test_files=(test_commands/*.txt)

    # Iterate over each test group
    for test_file in "${test_files[@]}"; do
        group_name=$(basename "$test_file" .txt)
        BASH_OUTPUT="test_outputs/bash_output_${group_name}.txt"
        MINISHELL_OUTPUT="test_outputs/minishell_output_${group_name}.txt"
        COMMANDS_FILE="$test_file"

        echo -e "${BLUE}============================================================${RESET}"
        echo -e "${BLUE}                     GROUP: ${group_name}                   ${RESET}"
        echo -e "${BLUE}============================================================${RESET}\n"

        # Initialize group counters
        group_total_tests=0
        group_tests_passed=0
        group_tests_failed=0

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
                if [[ "$line" == *"@minishell \$>"* ]]; then
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
            echo -e "${RED}Error: Number of commands and outputs do not match in group '${group_name}'.${RESET}"
            echo -e "Commands: ${#commands[@]}, Bash Outputs: ${#bash_outputs[@]}, Minishell Outputs: ${#minishell_outputs[@]}"
            exit 1
        fi

        # Iterate over each command
        for ((i=0; i<${#commands[@]}; i++)); do
            test_name="${commands[i]}"
            bash_output="${bash_outputs[i]}"
            mini_output="${minishell_outputs[i]}"

            # Increment counters
            ((total_tests++))
            ((group_total_tests++))

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
                ((group_tests_passed++))
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
                ((group_tests_failed++))
            fi
        done

        # Calculate group success rate
        if [ "$group_total_tests" -gt 0 ]; then
            group_success_rate=$(( (group_tests_passed * 100) / group_total_tests ))
        else
            group_success_rate=0
        fi

        # Store group results
        group_results["$group_name"]="$group_success_rate"

        # Print group summary
        echo -e "\n${BLUE}Group Summary for '${group_name}':${RESET}"
        echo -e "Total tests: $group_total_tests"
        echo -e "Passed: \033[1;32m$group_tests_passed\033[0m"
        echo -e "Failed: \033[1;31m$group_tests_failed\033[0m"
        echo -e "Success Rate: \033[1;36m${group_success_rate}%\033[0m\n"
    done

    # Calculate overall success rate
    if [ "$total_tests" -gt 0 ]; then
        overall_success_rate=$(( (tests_passed * 100) / total_tests ))
    else
        overall_success_rate=0
    fi

    # Print overall summary
    echo -e "${BLUE}============================================================${RESET}"
    echo -e "${BLUE}                  OVERALL BASH TEST RESULTS                 ${RESET}"
    echo -e "${BLUE}============================================================${RESET}\n"

    echo -e "${BLUE}Group Results:${RESET}"
    for group_name in "${!group_results[@]}"; do
        echo -e "- ${group_name}: \033[1;36m${group_results[$group_name]}% success\033[0m"
    done

    echo -e "\nTotal tests: $total_tests"
    echo -e "Passed: \033[1;32m$tests_passed\033[0m"
    echo -e "Failed: \033[1;31m$tests_failed\033[0m"
    echo -e "Overall Success Rate: \033[1;36m${overall_success_rate}%\033[0m"

    # Print Total Test Time with the captured elapsed_time
    echo -e "${CYAN}Total Test time (real) = ${elapsed_time} ms${RESET}"
    test_failed_global=$tests_failed
}

# ----------------------------
# Step 6: Run Valgrind Tests
# ----------------------------
run_valgrind_tests() {
    echo -e "\n${BLUE}============================================================${RESET}"
    echo -e "${BLUE}                   VALGRIND MEMORY CHECK                    ${RESET}"
    echo -e "${BLUE}============================================================${RESET}\n"

    mkdir -p valgrind_outputs
    # Initialize counters
    valgrind_total_commands=0
    valgrind_commands_passed=0
    valgrind_commands_failed=0

    # Find all test command files
    test_files=(test_commands/*.txt)

    for test_file in "${test_files[@]}"; do
        group_name=$(basename "$test_file" .txt)
        echo -e "${CYAN}Testing group '${group_name}' under Valgrind...${RESET}"

        # Read commands into array
        mapfile -t commands < "$test_file"

        # Iterate over each command
        for ((i=0; i<${#commands[@]}; i++)); do
            command="${commands[i]}"
            # Skip empty lines
            if [[ -z "$command" ]]; then
                continue
            fi

            # Sanitize command for filename
            command_sanitized=$(echo "$command" | tr ' /|><&' '_' | tr -s '_')
            valgrind_log="valgrind_outputs/valgrind_${group_name}_command_${i}.txt"

            echo -e "${YELLOW}Running Valgrind for command: '${command}'${RESET}"

            # Run minishell under Valgrind with the command
            echo "$command" | valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all \
                --suppressions=../assets/supp.supp \
                --error-exitcode=1 --log-file="$valgrind_log" ../minishell > /dev/null 2>&1

            # Check Valgrind exit code
            if [ $? -eq 0 ]; then
                echo -e "\033[1;32m✓ Valgrind check passed for command: '${command}'\033[0m"
                ((valgrind_commands_passed++))
            else
                echo -e "\033[1;31m✘ Valgrind check failed for command: '${command}'\033[0m"
                echo -e "    \033[1;33mValgrind errors detected:\033[0m"
                # Extract summary from Valgrind log
                grep -E "ERROR SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable" "$valgrind_log" | sed 's/^/        /'
                ((valgrind_commands_failed++))
            fi
            ((valgrind_total_commands++))
        done
    done

    # Calculate success rate
    if [ "$valgrind_total_commands" -gt 0 ]; then
        valgrind_success_rate=$(( (valgrind_commands_passed * 100) / valgrind_total_commands ))
    else
        valgrind_success_rate=0
    fi

    # Print Valgrind summary
    echo -e "\n${BLUE}Valgrind Summary:${RESET}"
    echo -e "Total commands tested: $valgrind_total_commands"
    echo -e "Passed: \033[1;32m$valgrind_commands_passed\033[0m"
    echo -e "Failed: \033[1;31m$valgrind_commands_failed\033[0m"
    echo -e "Success Rate: \033[1;36m${valgrind_success_rate}%\033[0m\n"
}

# ----------------------------
# Step 7: Main Execution Flow
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
    echo next!
    run_valgrind_tests
    rm ../minishell

    # Exit the script based on the test results
    if [ "$test_failed_global" -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
 }

# Execute the main function
main
