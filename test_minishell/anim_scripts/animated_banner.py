#!/usr/bin/env python3

import time
import sys
import threading
import signal

# Color Codes for Formatting
# ----------------------------
GREEN = "\033[1;32m"
RED = "\033[1;31m"
YELLOW = "\033[1;33m"
BLUE = "\033[1;34m"
CYAN = "\033[1;36m"
RESET = "\033[0m"

# ----------------------------
# ASCII Art Lines with Movement Parameters
# ----------------------------
lines = [
    # Lines 1 and 2: Move left and return
    {
        'text': "            __  __  _         _  _______          _   ",
        'color': CYAN,
        'initial_indent': 5,
        'movement': {
            'movement_amount': -5,
            'start_frame': 0,
            'duration': 30,
        }
    },
    {
        'text': "           |  \\/  |(_)       (_)|__   __|        | |  ",
        'color': CYAN,
        'initial_indent': 5,
        'movement': {
            'movement_amount': -5,
            'start_frame': 0,
            'duration': 30,
        }
    },
    # Lines 3 and 4: Move right and return
    {
        'text': "           | \\  / | _  _ __   _    | |  ___  ___ | |_",
        'color': GREEN,
        'initial_indent': 5,
        'movement': {
            'movement_amount': 5,
            'start_frame': 10,
            'duration': 30,
        }
    },
    {
        'text': "           | |\\/| || || '_ \\ | |   | | / _ \\/ __|| __|",
        'color': GREEN,
        'initial_indent': 5,
        'movement': {
            'movement_amount': 5,
            'start_frame': 10,
            'duration': 30,
        }
    },
    # Lines 5 and 6: Move left and return
    {
        'text': "           | |  | || || | | || |   | ||  __/\\__ \\| |_",
        'color': RED,
        'initial_indent': 5,
        'movement': {
            'movement_amount': -5,
            'start_frame': 20,
            'duration': 30,
        }
    },
    {
        'text': "           |_|  |_||_||_| |_||_|   |_| \\___||___/\\___|",
        'color': RED,
        'initial_indent': 5,
        'movement': {
            'movement_amount': -5,
            'start_frame': 20,
            'duration': 30,
        }
    },
    {
        'text': "",
        'color': RED,
        'initial_indent': 5,
        'movement': {
            'movement_amount': -5,
            'start_frame': 20,
            'duration': 30,
        }
    },
]

banner_height = len(lines)  # Number of lines in the banner

# Threading lock to prevent data races
print_lock = threading.Lock()

def compute_indentation(initial_indent, movement_amount, start_frame, duration, current_frame):
    # Wrap the current frame to loop the animation
    total_animation_duration = start_frame + duration
    frame_in_animation = current_frame % total_animation_duration

    if frame_in_animation < start_frame:
        # Movement hasn't started yet
        return initial_indent
    else:
        # Movement in progress
        progress = (frame_in_animation - start_frame) / duration
        if progress <= 0.5:
            # First half of movement
            movement_progress = progress * 2  # 0 to 1
        else:
            # Second half
            movement_progress = (1 - progress) * 2  # 1 back to 0
        indent = initial_indent + movement_amount * movement_progress
        return int(round(indent))

def print_banner(frame):
    with print_lock:
        # Save the current cursor position
        sys.stdout.write('\0337')  # Save cursor position with DEC Private Mode (DECSC)

        # Move cursor to the top-left corner
        sys.stdout.write('\033[1;1H')  # Move to line 1, column 1

        # Print each line with its current indentation and color
        for i, line in enumerate(lines):
            # Move to the correct line in case other outputs moved the cursor
            sys.stdout.write(f'\033[{i+1};1H')  # Move to line i+1, column 1
            sys.stdout.write('\033[K')  # Clear the line

            indent = compute_indentation(
                line['initial_indent'],
                line['movement']['movement_amount'],
                line['movement']['start_frame'],
                line['movement']['duration'],
                frame
            )
            sys.stdout.write(' ' * indent + line['color'] + line['text'] + RESET + '\n')

        # Restore the cursor position
        sys.stdout.write('\0338')  # Restore cursor position with DECRC
        sys.stdout.flush()

def print_static_banner():
    with print_lock:
        # Save the current cursor position
        sys.stdout.write('\0337')  # Save cursor position

        # Move cursor to the top-left corner
        sys.stdout.write('\033[1;1H')

        # Clear the banner area
        for i in range(banner_height):
            sys.stdout.write(f'\033[{i+1};1H')  # Move to line i+1, column 1
            sys.stdout.write('\033[K')  # Clear the line

        # Print the static banner
        for i, line in enumerate(lines):
            sys.stdout.write(f'\033[{i+1};1H')  # Move to line i+1, column 1
            indent = line['initial_indent']
            sys.stdout.write(' ' * indent + line['color'] + line['text'] + RESET + '\n')

        # Restore the cursor position
        sys.stdout.write('\0338')
        sys.stdout.flush()

def animate_banner(stop_event):
    frame = 0
    while not stop_event.is_set():
        print_banner(frame)
        time.sleep(0.1)  # Adjust the sleep duration for animation speed
        frame += 1

    # Before exiting, clear the banner area and print the static banner
    print_static_banner()

    # Move cursor below the banner when exiting
    with print_lock:
        sys.stdout.write(f'\033[{banner_height + 1};1H')
        sys.stdout.flush()

if __name__ == '__main__':
    stop_event = threading.Event()

    def signal_handler(sig, frame):
        stop_event.set()

    signal.signal(signal.SIGTERM, signal_handler)
    signal.signal(signal.SIGINT, signal_handler)

    animate_banner(stop_event)

