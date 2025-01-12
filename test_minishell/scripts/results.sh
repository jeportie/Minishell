#!/bin/bash
# results.sh

source "$SCRIPT_DIR/scripts/report_helpers.sh"
source "$SCRIPT_DIR/scripts/compare_single_command.sh"

test_failed_global=0

parse_section() {
    local folder="$1"
    local folder_path="$SCRIPT_DIR/test_commands/$folder"
    [[ ! -d "$folder_path" ]] && return

    echo -e "${GREEN}======================================================================${RESET}"
    echo -e "${YELLOW}                SECTION: ${folder}${RESET}"
    echo -e "${GREEN}======================================================================${RESET}\n"

    local tfiles=( "$folder_path"/*.txt )
    for tf in "${tfiles[@]}"; do
        [[ ! -f "$tf" ]] && continue
        handle_section_results "$tf" total_tests tests_passed tests_failed group_results
    done
}

handle_section_results() {
    local tf="$1"
    local -n tot_ref="$2"
    local -n pass_ref="$3"
    local -n fail_ref="$4"
    local -n group_ref="$5"

    local group_name
    group_name=$(basename "$tf" .txt)

    echo -e "${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                GROUP: ${group_name}${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    count_commands "$tf"
    mapfile -t commands < "$tf"

    local has_heredoc=false
    for cmd in "${commands[@]}"; do
        [[ "$cmd" == *'<<'* ]] && has_heredoc=true && break
    done

    declare -a filtered_commands
    if $has_heredoc; then
        filter_heredocs commands filtered_commands
    else
        filtered_commands=("${commands[@]}")
    fi

    declare -a final_test_names
    build_final_test_names filtered_commands final_test_names "$has_heredoc"

    local subdir
    subdir=$(basename "$(dirname "$tf")")

    local BASH_OUTPUT="$SCRIPT_DIR/test_outputs/$subdir/bash_output_${group_name}.txt"
    local MINISHELL_OUTPUT="$SCRIPT_DIR/test_outputs/$subdir/minishell_output_${group_name}.txt"

    declare -a bash_outputs
    parse_outputs "$BASH_OUTPUT" bash_outputs

    declare -a minishell_outputs
    parse_outputs "$MINISHELL_OUTPUT" minishell_outputs

    local bash_output_count="${#bash_outputs[@]}"
    local minishell_output_count="${#minishell_outputs[@]}"

    local test_loop_count=0
    if [[ "$group_name" != "mandatory_builtins_exit" && "$group_name" != "mandatory_redirections_heredoc" ]]; then
        # must match
        if [[ "$total_commands" -ne "$bash_output_count" || "$total_commands" -ne "$minishell_output_count" ]]; then
            echo -e "${RED}Error: #commands != #outputs in '$group_name'.${RESET}"
            exit 1
        fi
        test_loop_count=$total_commands
    else
        test_loop_count=$bash_output_count
    fi

    local i
    local group_total=0
    local group_pass=0
    local group_fail=0

    for (( i=0; i<test_loop_count; i++ )); do
        local test_name="${final_test_names[i]}"
        compare_single_command \
            "$test_name" \
            "${bash_outputs[i]}" \
            "${minishell_outputs[i]}" \
            "$group_name"

        local r=$?
        ((group_total++))
        ((tot_ref++))

        if [[ $r -eq 0 ]]; then
            ((group_pass++))
            ((pass_ref++))
        else
            ((group_fail++))
            ((fail_ref++))
        fi
    done

    echo -e "\n${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                GROUP SUMMARY: ${group_name}${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    # Calculate success rate for this file
    local group_success_rate=0
    if [[ $group_total -gt 0 ]]; then
        group_success_rate=$(( (group_pass * 100) / group_total ))
    fi
    group_ref["$group_name"]="$group_success_rate"

    echo -e "Total tests: $group_total"
    echo -e "Passed: \033[1;32m$group_pass\033[0m"
    echo -e "Failed: \033[1;31m$group_fail\033[0m"
    echo -e "Success Rate: \033[1;36m${group_success_rate}%\033[0m\n"
}

print_overall_summary() {
    local tot="$1"
    local p="$2"
    local f="$3"
    local -n grp_ref="$4"
    local e_time="$5"

    local overall_success=0
    if [[ $tot -gt 0 ]]; then
        overall_success=$(( (p * 100) / tot ))
    fi

    echo -e "${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                       OVERALL BASH TEST RESULTS                      ${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    echo -e "${BLUE}Group Results:${RESET}"
    for name in "${!grp_ref[@]}"; do
        echo -e "- ${name}: \033[1;36m${grp_ref[$name]}% success\033[0m"
    done

    echo -e "\nTotal tests: $tot"
    echo -e "Passed: \033[1;32m$p\033[0m"
    echo -e "Failed: \033[1;31m$f\033[0m"
    echo -e "Overall Success Rate: \033[1;36m${overall_success}%\033[0m"

    echo -e "${CYAN}Total Test time (real) = ${e_time} ms${RESET}"
    test_failed_global=$f
}

report_results() {
    local elapsed_time="$1"
    local section="$2"
    local single_test="$3"

    echo -e "${BLUE}======================================================================${RESET}"
    echo -e "${BLUE}                TESTING MINISHELL VS POSIX SUMMARY                    ${RESET}"
    echo -e "${BLUE}======================================================================${RESET}\n"

    local total_tests=0
    local tests_passed=0
    local tests_failed=0

    declare -A group_results

    if [[ -n "$single_test" ]]; then
        local test_file
        test_file=$(find "$SCRIPT_DIR/test_commands" -type f -name "$single_test" 2>/dev/null | head -n 1)
        if [[ -z "$test_file" ]]; then
            echo -e "${RED}Error: single test '$single_test' not found in test_commands/.${RESET}"
            exit 1
        fi
        handle_section_results "$test_file" total_tests tests_passed tests_failed group_results
        print_overall_summary "$total_tests" "$tests_passed" "$tests_failed" group_results "$elapsed_time"
        return
    fi

    if [[ "$section" == "mandatory" ]]; then
        parse_section "mandatory"
    elif [[ "$section" == "bonus" ]]; then
        parse_section "bonus"
    elif [[ "$section" == "extended" ]]; then
        parse_section "extended"
    else
        parse_section "mandatory"
        parse_section "bonus"
        parse_section "extended"
    fi

    print_overall_summary "$total_tests" "$tests_passed" "$tests_failed" group_results "$elapsed_time"
}
