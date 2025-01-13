#!/bin/bash
# build.sh

# ----------------------------
# Clean Existing Output Files
# ----------------------------
clean_outputs() {
    move_cursor_below_banner
    echo "\n"
    echo -e "${CYAN}Cleaning existing output files...${RESET}"
    rm -f test_outputs/*.txt valgrind_outputs/*.txt
    echo -e "${GREEN}Output files cleaned.${RESET}"
}

# ----------------------------
# Build the Minishell Project
# ----------------------------
build_project() {
    move_cursor_below_banner
    echo -e "${CYAN}Building the project using make...${RESET}"
    
    # Capture the build output
    BUILD_LOG=$(mktemp)
    make -C .. fullclean > "$BUILD_LOG" 
    make -C .. nodebug >> "$BUILD_LOG"
    if make -C .. classic >> "$BUILD_LOG" 2>&1; then
        echo -e "${GREEN}Build successful.${RESET}"
    else
        echo -e "${RED}Build failed. Please check the error details below:${RESET}"
        stop_banner
        cat "$BUILD_LOG"
        exit 1
    fi
}
