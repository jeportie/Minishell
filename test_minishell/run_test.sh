#!/bin/bash
# run_test.sh

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
RUN_ONLY_SECTION=""
SINGLE_TEST=""

# Parse flags
while [[ $# -gt 0 ]]; do
    case "$1" in
        --valgrind)
            RUN_VALGRIND=true
            shift
            ;;
        --mandatory)
            RUN_ONLY_SECTION="mandatory"
            shift
            ;;
        --bonus)
            RUN_ONLY_SECTION="bonus"
            shift
            ;;
        --extended)
            RUN_ONLY_SECTION="extended"
            shift
            ;;
        --single)
            SINGLE_TEST="$2"
            shift 2
            ;;
        *)
            echo "Usage: $0 [--valgrind] [--mandatory|--bonus|--extended] [--single test_name.txt]"
            exit 1
            ;;
    esac
done

main() {
    clear
    start_banner
    echo "\n"
    clean_outputs
    build_project

    local start_time=$(date +%s%N)

    run_expect_scripts "$RUN_ONLY_SECTION" "$SINGLE_TEST"
    process_outputs "$RUN_ONLY_SECTION" "$SINGLE_TEST"
    stop_banner
    clear
    "$SCRIPT_DIR/anim_scripts/noanimated_banner.py"

    local end_time=$(date +%s%N)
    local elapsed=$(( (end_time - start_time)/1000000 ))

    report_results "$elapsed" "$RUN_ONLY_SECTION" "$SINGLE_TEST"

    if [ "$RUN_VALGRIND" == true ]; then
        run_valgrind_tests
    fi
    make -C .. fullclean >> "$BUILD_LOG"

    if [ "$test_failed_global" -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
}

main
