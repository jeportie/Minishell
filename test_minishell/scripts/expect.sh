#!/bin/bash
# expect.sh

# ----------------------------
# Run Expect Scripts
# ----------------------------
run_expect_scripts() {
    move_cursor_below_banner
    # Find all test command files in test_commands/ directory
    local test_files=($SCRIPT_DIR/test_commands/*.txt)

    # Check if any test files are found
    if [ ${#test_files[@]} -eq 0 ]; then
        echo -e "${RED}No test command files found in test_commands/ directory.${RESET}"
        stop_banner
        exit 1
    fi

    # Iterate over each test file
    for test_file in "${test_files[@]}"; do
        local group_name=$(basename "$test_file" .txt)
        echo -e "${CYAN}Running Expect scripts for group '${group_name}'...${RESET}"
        if "$SCRIPT_DIR/expect_scripts/run_bash.expect" "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}bash.expect executed successfully for group '${group_name}'.${RESET}"
        else
            echo -e "${RED}bash.expect failed for group '${group_name}'. Please check the script and permissions.${RESET}"
            stop_banner
            exit 1
        fi
        if "$SCRIPT_DIR/expect_scripts/run_minishell.expect" "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}minishell.expect executed successfully for group '${group_name}'.${RESET}"
        else
            if [ "$group_name" != "mandatory_expansions_env_variables" ]; then
                echo -e "${RED}minishell.expect failed for group '${group_name}'. Please check the script and permissions.${RESET}"
                stop_banner
                exit 1
            fi
        fi
    done
}
