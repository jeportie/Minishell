#!/bin/bash
# expect.sh

run_expect_scripts() {
    move_cursor_below_banner

    local section="$1"
    local single_test="$2"

    if [[ -n "$single_test" ]]; then
        local test_file
        test_file=$(find "$SCRIPT_DIR/test_commands" -type f -name "$single_test" 2>/dev/null | head -n 1)
        if [[ -z "$test_file" ]]; then
            echo -e "${RED}Error: single test '$single_test' not found in test_commands/.${RESET}"
            stop_banner
            exit 1
        fi
        local group_name
        group_name=$(basename "$test_file" .txt)

        echo -e "${CYAN}Running Expect scripts for single test '${group_name}'...${RESET}"
        if "$SCRIPT_DIR/expect_scripts/run_bash.expect" "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}bash.expect OK for '${group_name}'.${RESET}"
        else
            echo -e "${RED}bash.expect failed for '${group_name}'.${RESET}"
            stop_banner
            exit 1
        fi
        if "$SCRIPT_DIR/expect_scripts/run_minishell.expect" "$test_file" > /dev/null 2>&1; then
            echo -e "${GREEN}minishell.expect OK for '${group_name}'.${RESET}"
        else
            echo -e "${RED}minishell.expect failed for '${group_name}'.${RESET}"
            stop_banner
            exit 1
        fi

        return
    fi

    run_section() {
        local section_dir="$1"
        local folder_path="$SCRIPT_DIR/test_commands/$section_dir"

        if [[ ! -d "$folder_path" ]]; then
            return
        fi

        echo -e "\n${GREEN}======================================================================${RESET}"
        echo -e "${YELLOW}                        SECTION: ${section_dir}${RESET}"
        echo -e "${GREEN}======================================================================${RESET}\n"

        local test_files=( "$folder_path"/*.txt )
        for tf in "${test_files[@]}"; do
            [[ ! -f "$tf" ]] && continue

            local group_name
            group_name=$(basename "$tf" .txt)
            echo -e "${CYAN}Running Expect scripts for group '${group_name}'...${RESET}"
            if "$SCRIPT_DIR/expect_scripts/run_bash.expect" "$tf" > /dev/null 2>&1; then
                echo -e "${GREEN}bash.expect OK for '${group_name}'.${RESET}"
            else
                echo -e "${RED}bash.expect failed for '${group_name}'.${RESET}"
                stop_banner
                exit 1
            fi
            if "$SCRIPT_DIR/expect_scripts/run_minishell.expect" "$tf" > /dev/null 2>&1; then
                echo -e "${GREEN}minishell.expect OK for '${group_name}'.${RESET}"
            else
                if [[ "$group_name" != "mandatory_expansions_env_variables" ]]; then
                    echo -e "${RED}minishell.expect failed for '${group_name}'.${RESET}"
                    stop_banner
                    exit 1
                fi
            fi
        done
    }

    if [[ "$section" == "mandatory" ]]; then
        run_section "mandatory"
        return
    elif [[ "$section" == "bonus" ]]; then
        run_section "bonus"
        return
    elif [[ "$section" == "extended" ]]; then
        run_section "extended"
        return
    fi
    run_section "mandatory"
    run_section "bonus"
    run_section "extended"
}

