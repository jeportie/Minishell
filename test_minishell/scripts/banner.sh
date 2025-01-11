#!/bin/bash
# banner.sh
#
# ----------------------------
# Start Animated Banner
# ----------------------------
start_banner() {
    # Start the animated banner in the background
    python3 "$SCRIPT_DIR/anim_scripts/animated_banner.py" &
    BANNER_PID=$!
    # Allow the banner to set up
    sleep 0.1
    # Move cursor below the banner area
    move_cursor_below_banner
}

# ----------------------------
# Stop Animated Banner
# ----------------------------
stop_banner() {
    # Stop the animated banner
    if [ -n "$BANNER_PID" ]; then
        kill "$BANNER_PID" 2>/dev/null
        wait "$BANNER_PID" 2>/dev/null
        unset BANNER_PID
        # Move cursor below the banner area
        BANNER_HEIGHT=6  # Number of lines in the banner
        echo -ne "\033[$((BANNER_HEIGHT + 1));1H"
        sleep 1
    fi
}

# ----------------------------
# Ensure Outputs Appear Below Banner
# ----------------------------
move_cursor_below_banner() {
    # Move the cursor to the line below the banner
    BANNER_HEIGHT=6  # Number of lines in the banner
    echo -ne "\033[$((BANNER_HEIGHT + 1));1H"  # Move cursor to line after the banner
}
