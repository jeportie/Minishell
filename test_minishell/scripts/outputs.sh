#!/bin/bash
# outputs.sh

process_outputs() {
    move_cursor_below_banner
    echo -e "${CYAN}Processing output files...${RESET}"

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
        local subdir
        subdir="$(basename "$(dirname "$test_file")")"
        local name_no_ext
        name_no_ext="$(basename "$test_file" .txt)"

        local group_info=( "$subdir:$name_no_ext" )

    else
        declare -a test_files=()

        if [[ "$section" == "mandatory" ]]; then
            test_files=( $(find "$SCRIPT_DIR/test_commands/mandatory" -type f -name "*.txt" 2>/dev/null) )
        elif [[ "$section" == "bonus" ]]; then
            test_files=( $(find "$SCRIPT_DIR/test_commands/bonus" -type f -name "*.txt" 2>/dev/null) )
        elif [[ "$section" == "extended" ]]; then
            test_files=( $(find "$SCRIPT_DIR/test_commands/extended" -type f -name "*.txt" 2>/dev/null) )
        else
            test_files=( $(find "$SCRIPT_DIR/test_commands" -type f -name "*.txt") )
        fi

        local group_info=()
        for test_file in "${test_files[@]}"; do
            local subdir
            subdir="$(basename "$(dirname "$test_file")")"
            local name_no_ext
            name_no_ext="$(basename "$test_file" .txt)"
            group_info+=( "$subdir:$name_no_ext" )
        done
    fi

    if "$SCRIPT_DIR/scripts/process_outputs.sh" "${group_info[@]}" > /dev/null 2>&1; then
        echo -e "${GREEN}Output files processed successfully.${RESET}"
    else
        echo -e "${RED}Processing output files failed. Please check process_outputs.sh.${RESET}"
        stop_banner
        exit 1
    fi

    local output_files=()
    for pair in "${group_info[@]}"; do
        local subdir="${pair%%:*}"
        local group_name="${pair#*:}"
        output_files+=( "test_outputs/$subdir/bash_output_${group_name}.txt"
                        "test_outputs/$subdir/minishell_output_${group_name}.txt" )
    done
    "$SCRIPT_DIR/scripts/remove_escape_codes.sh" "${output_files[@]}"
}
