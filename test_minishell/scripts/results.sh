#!/bin/bash
# results.sh
#
# This file sources "report_helpers.sh" to access the
# modular functions for counting commands, filtering heredocs,
# parsing outputs, etc. The loop below becomes smaller.

# Initialize a global test failure counter
test_failed_global=0

# Source our new helpers
source "$SCRIPT_DIR/scripts/report_helpers.sh"
source "$SCRIPT_DIR/scripts/compare_single_command.sh"

report_results() {
    local elapsed_time="$1"

    echo -e "${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                   TESTING MINISHELL VS POSIX SUMMARY                 ${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    # Overall counters
    local total_tests=0
    local tests_passed=0
    local tests_failed=0

    # Associative array for group success rates
    declare -A group_results

    # Gather test files
    local test_files=("${SCRIPT_DIR}/test_commands/"*.txt)

    for test_file in "${test_files[@]}"; do
        local group_name
        group_name="$(basename "$test_file" .txt)"

        local BASH_OUTPUT="test_outputs/bash_output_${group_name}.txt"
        local MINISHELL_OUTPUT="test_outputs/minishell_output_${group_name}.txt"

        echo -e "${BLUE}======================================================================${RESET}"
        echo -e "${BLUE}                GROUP: ${group_name}${RESET}"
        echo -e "${BLUE}======================================================================${RESET}\n"

        # Group counters
        local group_total_tests=0
        local group_tests_passed=0
        local group_tests_failed=0

        # 1) Count total commands (including heredocs)
        count_commands "$test_file"

        # 2) Load entire file into an array
        mapfile -t commands < "$test_file"

        # 3) Detect if the file has heredoc lines
        local has_heredoc=false
        for cmd in "${commands[@]}"; do
            if [[ "$cmd" == *'<<'* ]]; then
                has_heredoc=true
                break
            fi
        done

        # 4) Filter out heredoc body lines
        declare -a filtered_commands
        if $has_heredoc; then
            filter_heredocs commands filtered_commands
        else
            filtered_commands=("${commands[@]}")
        fi

        # 5) Build final test names
        declare -a final_test_names
        build_final_test_names filtered_commands final_test_names "$has_heredoc"

        # 6) Parse Bash & Minishell outputs
        declare -a bash_outputs
        parse_outputs "$BASH_OUTPUT" bash_outputs

        declare -a minishell_outputs
        parse_outputs "$MINISHELL_OUTPUT" minishell_outputs

        local bash_output_count="${#bash_outputs[@]}"
        local minishell_output_count="${#minishell_outputs[@]}"

        # Decide how many commands to loop over
        local test_loop_count=0

        # 7) If group is not special, #commands must match #outputs
        if [ "$group_name" != "mandatory_builtins_exit" ] \
           && [ "$group_name" != "mandatory_redirections_heredoc" ]; then

            if [ "$total_commands" -ne "$bash_output_count" ] \
               || [ "$total_commands" -ne "$minishell_output_count" ]; then
                echo -e "${RED}Error: #commands != #outputs in group '${group_name}'.${RESET}"
                echo -e "Commands: $total_commands, Bash: $bash_output_count, Minishell: $minishell_output_count"
                exit 1
            fi
            test_loop_count=$total_commands
        else
            # For special groups
            test_loop_count=$bash_output_count
        fi

        # 8) Compare each command’s output
        local i
        for (( i=0; i<test_loop_count; i++ )); do
            local test_name="${final_test_names[i]}"
            compare_single_command \
                "$test_name" \
                "${bash_outputs[i]}" \
                "${minishell_outputs[i]}" \
                "$group_name"

            local result=$?
            ((group_total_tests++))
            ((total_tests++))

            if [ $result -eq 0 ]; then
                ((group_tests_passed++))
                ((tests_passed++))
            else
                ((group_tests_failed++))
                ((tests_failed++))
            fi
        done

        # ----------------------------------------------------------------------
        # Group Summary
        # ----------------------------------------------------------------------
        local group_success_rate=0
        if [ "$group_total_tests" -gt 0 ]; then
            group_success_rate=$(( (group_tests_passed * 100) / group_total_tests ))
        fi
        group_results["$group_name"]="$group_success_rate"

        echo -e "\n${BLUE}Group Summary for '${group_name}':${RESET}"
        echo -e "Total tests: $group_total_tests"
        echo -e "Passed: \033[1;32m$group_tests_passed\033[0m"
        echo -e "Failed: \033[1;31m$group_tests_failed\033[0m"
        echo -e "Success Rate: \033[1;36m${group_success_rate}%\033[0m\n"
    done

    # --------------------------------------------------------------------------
    # OVERALL SUMMARY
    # --------------------------------------------------------------------------
    local overall_success_rate=0
    if [ "$total_tests" -gt 0 ]; then
        overall_success_rate=$(( (tests_passed * 100) / total_tests ))
    fi

    echo -e "${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                       OVERALL BASH TEST RESULTS                      ${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    echo -e "${BLUE}Group Results:${RESET}"
    local group_key
    for group_key in "${!group_results[@]}"; do
        echo -e "- ${group_key}: \033[1;36m${group_results[$group_key]}% success\033[0m"
    done

    echo -e "\nTotal tests: $total_tests"
    echo -e "Passed: \033[1;32m$tests_passed\033[0m"
    echo -e "Failed: \033[1;31m$tests_failed\033[0m"
    echo -e "Overall Success Rate: \033[1;36m${overall_success_rate}%\033[0m"

    echo -e "${CYAN}Total Test time (real) = ${elapsed_time} ms${RESET}"

    test_failed_global=$tests_failed
}

