#!/bin/bash
# run_test.sh
# Script to automate testing of the 42 School Minishell project

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/scripts/colors.sh"
source "$SCRIPT_DIR/scripts/banner.sh"
source "$SCRIPT_DIR/scripts/build.sh"
source "$SCRIPT_DIR/scripts/expect.sh"
source "$SCRIPT_DIR/scripts/outputs.sh"
source "$SCRIPT_DIR/scripts/results.sh"
source "$SCRIPT_DIR/scripts/valgrind.sh"

test_failed_global=0
BUILD_LOG=""

RUN_VALGRIND=false

for arg in "$@"; do
    if [ "$arg" == "--valgrind" ]; then
        RUN_VALGRIND=true
    fi
done

main() {
    clear
    start_banner
    clean_outputs
    build_project

    # Start Time Capture
    local start_time=$(date +%s%N)

    run_expect_scripts
    process_outputs
    stop_banner
    clear
    "$SCRIPT_DIR/anim_scripts/noanimated_banner.py"

    # End Time Capture
    local end_time=$(date +%s%N)

    # Calculate elapsed time in milliseconds
    local elapsed=$(( (end_time - start_time)/1000000 ))

    # Pass elapsed time to report_results
    report_results "$elapsed"
    if [ "$RUN_VALGRIND" == true ]; then
        run_valgrind_tests
    fi
    make -C .. fullclean >> "$BUILD_LOG" 

    # Exit the script based on the test results
    if [ "$test_failed_global" -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
 }

# Execute the main function
main
