#!/bin/bash

VALGRIND_CMD="$1"
TEST_EXEC="$2"
LOG_FILE="$3"
SUPPRESSIONS_FILE="$4"

# Run the test under Valgrind, tracing children
$VALGRIND_CMD --leak-check=full --trace-children=yes \
    --suppressions=$SUPPRESSIONS_FILE --log-file="$LOG_FILE" "$TEST_EXEC"
VALGRIND_EXIT_CODE=$?

# Initialize variables
ERROR_OCCURRED=0

# Parse the Valgrind log file to check for errors
TOTAL_ERRORS=$(grep -c "ERROR SUMMARY: [^0]" "$LOG_FILE")

if [ "$TOTAL_ERRORS" -gt 0 ]; then
    echo "Valgrind detected errors:"
    # Extract LEAK SUMMARY and any detailed errors
    awk '
    BEGIN { in_error = 0; }
    /==.*ERROR SUMMARY:/ {
        in_error = 1;
        print $0;
        next;
    }
    in_error && /^==[0-9]+==.*$/ {
        print $0;
        if (/^==[0-9]+==\s*$/) { in_error = 0; }
        next;
    }
    /==.*LEAK SUMMARY:/ {
        in_leak_summary = 1;
        print $0;
        next;
    }
    in_leak_summary {
        print $0;
        if (/^==[0-9]+==\s*$/) { in_leak_summary = 0; }
    }
    ' "$LOG_FILE"
    EXIT_CODE=1
else
    EXIT_CODE=0
fi

exit $EXIT_CODE

