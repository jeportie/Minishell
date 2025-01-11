#!/bin/bash
# outputs.sh

# ----------------------------
# Process Output Files
# ----------------------------
process_outputs() {
    move_cursor_below_banner
    echo -e "${CYAN}Processing output files...${RESET}"
    # Find all group names
    local test_files=($SCRIPT_DIR/test_commands/*.txt)
    group_names=()
    for test_file in "${test_files[@]}"; do
        group_names+=("$(basename "$test_file" .txt)")
    done

    # Process outputs for each group
    if "$SCRIPT_DIR/scripts/process_outputs.sh" "${group_names[@]}" > /dev/null 2>&1; then
        echo -e "${GREEN}Output files processed successfully.${RESET}"
    else
        echo -e "${RED}Processing output files failed. Please check process_outputs.sh.${RESET}"
        stop_banner
        exit 1
    fi

    # Remove escape codes from all output files
    output_files=()
    for group_name in "${group_names[@]}"; do
        output_files+=("test_outputs/bash_output_${group_name}.txt" "test_outputs/minishell_output_${group_name}.txt")
    done
    "$SCRIPT_DIR/scripts/remove_escape_codes.sh" "${output_files[@]}"
}
