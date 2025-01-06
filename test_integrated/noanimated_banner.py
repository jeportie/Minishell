#!/usr/bin/env python3

# Color Codes for Formatting
GREEN = "\033[1;32m"
RED = "\033[1;31m"
CYAN = "\033[1;36m"
RESET = "\033[0m"

# ----------------------------
# ASCII Art Lines
# ----------------------------
lines = [
    # Lines 1 and 2: Cyan Color
    {
        'text': "            __  __  _         _  _______          _   ",
        'color': CYAN,
        'initial_indent': 5,
    },
    {
        'text': "           |  \\/  |(_)       (_)|__   __|        | |  ",
        'color': CYAN,
        'initial_indent': 5,
    },
    # Lines 3 and 4: Green Color
    {
        'text': "           | \\  / | _  _ __   _    | |  ___  ___ | |_",
        'color': GREEN,
        'initial_indent': 5,
    },
    {
        'text': "           | |\\/| || || '_ \\ | |   | | / _ \\/ __|| __|",
        'color': GREEN,
        'initial_indent': 5,
    },
    # Lines 5 and 6: Red Color
    {
        'text': "           | |  | || || | | || |   | ||  __/\\__ \\| |_",
        'color': RED,
        'initial_indent': 5,
    },
    {
        'text': "           |_|  |_||_||_| |_||_|   |_| \\___||___/\\___|",
        'color': RED,
        'initial_indent': 5,
    },
    # Line 7: Empty Line (Optional)
    {
        'text': "",
        'color': RED,
        'initial_indent': 5,
    },
]

def print_static_banner():
    for line in lines:
        indent = ' ' * line['initial_indent']
        color = line['color']
        text = line['text']
        # If the line is empty, just print a newline without color
        if text.strip() == "":
            print()
        else:
            print(f"{indent}{color}{text}{RESET}")

if __name__ == '__main__':
    print_static_banner()

