#!/bin/bash
# valgrind.sh
#
run_valgrind_tests() {
    echo -e "\n${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                        VALGRIND MEMORY CHECK                         ${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    mkdir -p valgrind_outputs
    # Initialize counters
    local valgrind_total_commands=0
    local valgrind_commands_passed=0
    local valgrind_commands_failed=0

    # Find all test command files
    local test_files=($SCRIPT_DIR/test_commands/*.txt)

    for test_file in "${test_files[@]}"; do
        local group_name=$(basename "$test_file" .txt)
        echo -e "${CYAN}Testing group '${group_name}' under Valgrind...${RESET}"

        # Read commands into array
        mapfile -t commands < "$test_file"

        # Iterate over each command
        for ((i=0; i<${#commands[@]}; i++)); do
            local command="${commands[i]}"
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
