#!/bin/bash
# compare_single_command.sh

# ------------------------------------------------------------------------------
# compare_single_command():
#   Compares one Bash output vs. Minishell output, decides PASS/FAIL,
#   prints the result, and returns 0 on success or 1 on failure.
#   Internally calls smaller local functions for clarity.
# ------------------------------------------------------------------------------

compare_single_command() {
    local test_name="$1"
    local bash_output="$2"
    local mini_output="$3"
    local group_name="$4"

    # 1) Clean blank lines
    local clean_bash
    local clean_mini
    clean_bash="$(remove_blank_lines "$bash_output")"
    clean_mini="$(remove_blank_lines "$mini_output")"

    # 2) Split each into arrays
    local -a bash_lines
    local -a mini_lines
    read_into_arrays "$clean_bash" bash_lines
    read_into_arrays "$clean_mini" mini_lines

    # 3) Compare them (line counts, content, exit codes). Sets mismatch variables.
    local output_mismatch=false
    local exit_code_mismatch=false
    local output_number_mismatch=false

    compare_bash_and_mini \
        bash_lines \
        mini_lines \
        "$group_name" \
        output_mismatch \
        exit_code_mismatch \
        output_number_mismatch

    # 4) Decide PASS/FAIL
    if ! $output_mismatch && ! $exit_code_mismatch && ! $output_number_mismatch; then
        print_pass "$test_name"
        return 0
    else
        print_fail \
            "$test_name" \
            bash_lines \
            mini_lines \
            "$group_name" \
            "$output_mismatch" \
            "$exit_code_mismatch" \
            "$output_number_mismatch"
        return 1
    fi
}

# ------------------------------------------------------------------------------
# remove_blank_lines():
#   Removes empty or whitespace-only lines via sed.
# ------------------------------------------------------------------------------
remove_blank_lines() {
    local text="$1"
    # echo -e so we preserve any newlines in $text
    echo -e "$text" | sed '/^[[:space:]]*$/d'
}

# ------------------------------------------------------------------------------
# read_into_arrays():
#   Splits text into an array line by line using IFS+read.
#   The “-d ''” trick ensures we properly capture lines with blank endings.
# ------------------------------------------------------------------------------
read_into_arrays() {
    local text="$1"
    local -n arr_ref="$2"

    IFS=$'\n' read -r -d '' -a arr_ref <<< "$text"
}

# ------------------------------------------------------------------------------
# compare_bash_and_mini():
#   Checks line counts, output content, exit code.  
#   We pass references to “output_mismatch”, etc., which are booleans.
# ------------------------------------------------------------------------------
compare_bash_and_mini() {
    local -n b_lines="$1"
    local -n m_lines="$2"
    local group_name="$3"

    # Booleans we will set
    local -n output_mismatch_ref="$4"
    local -n exit_code_mismatch_ref="$5"
    local -n output_number_mismatch_ref="$6"

    output_mismatch_ref=false
    exit_code_mismatch_ref=false
    output_number_mismatch_ref=false

    local bash_line_count=${#b_lines[@]}
    local mini_line_count=${#m_lines[@]}

    # 1) Compare line counts
    if [ "$bash_line_count" -ne "$mini_line_count" ]; then
        output_number_mismatch_ref=true
        return
    fi

    # 2) If line counts match, handle small or multi-line output
    if [ "$bash_line_count" -eq 1 ]; then
        # Only exit code
        if [ "${b_lines[0]}" != "${m_lines[0]}" ]; then
            exit_code_mismatch_ref=true
        fi
    elif [ "$bash_line_count" -eq 2 ]; then
        # Output + exit code
        if [ "${b_lines[0]}" != "${m_lines[0]}" ]; then
            output_mismatch_ref=true
        fi
        if [ "${b_lines[1]}" != "${m_lines[1]}" ]; then
            exit_code_mismatch_ref=true
        fi
    else
        # Possibly multi-line output
        local bash_output_content=("${b_lines[@]:0:${#b_lines[@]}-1}")
        local mini_output_content=("${m_lines[@]:0:${#m_lines[@]}-1}")

        if [ "${bash_output_content[*]}" != "${mini_output_content[*]}" ]; then
            output_mismatch_ref=true
        fi

        # Check exit code if group is not special
        if [ "$group_name" != "mandatory_builtins_exit" ] \
           && [ "$group_name" != "mandatory_redirections_heredoc" ]; then
            if [ "${b_lines[-1]}" != "${m_lines[-1]}" ]; then
                exit_code_mismatch_ref=true
            fi
        fi
    fi
}

# ------------------------------------------------------------------------------
# print_pass():
#   Prints a green “PASS” line for a test.
# ------------------------------------------------------------------------------
print_pass() {
    local test_name="$1"
    printf "\033[1;32m✓ %-60s\t(PASS)\033[0m\n" "$test_name"
}

# ------------------------------------------------------------------------------
# print_fail():
#   Prints a red “FAIL” line, plus error details and a diff if needed.
# ------------------------------------------------------------------------------
print_fail() {
    local test_name="$1"
    local -n b_lines="$2"
    local -n m_lines="$3"
    local group_name="$4"
    local output_mismatch="$5"
    local exit_code_mismatch="$6"
    local output_number_mismatch="$7"

    printf "\033[1;31m✘ %-45s\t(FAIL)\033[0m\n" "$test_name"

    local error_msg=""
    $output_mismatch         && error_msg+="Output mismatch. "
    $exit_code_mismatch      && error_msg+="Error code mismatch. "
    $output_number_mismatch  && error_msg+="Output number mismatch. "

    echo -e "    \033[1;33mError(s):\033[0m $error_msg"

    if $output_number_mismatch; then
        echo -e "    \033[1;33mBash Lines:\033[0m ${#b_lines[@]}"
        echo -e "    \033[1;33mMinishell Lines:\033[0m ${#m_lines[@]}"
    fi

    # If there's an output mismatch or number mismatch, show the diff
    if $output_mismatch || $exit_code_mismatch || $output_number_mismatch; then
        echo -e "    \033[1;33mDiff:\033[0m"
        local diff_output
        diff_output=$(diff -u <(echo "${m_lines[*]}") <(echo "${b_lines[*]}"))
        while IFS= read -r line; do
            if [[ $line == ---* || $line == +++* ]]; then
                continue
            elif [[ $line == @@* ]]; then
                echo -e "        \033[1;34m$line\033[0m"
            elif [[ $line == -* ]]; then
                echo -e "        \033[1;31m$line\033[0m"
            elif [[ $line == +* ]]; then
                echo -e "        \033[1;32m$line\033[0m"
            else
                echo -e "        $line"
            fi
        done <<< "$diff_output"
    fi

    # If exit code mismatch, print them
    if $exit_code_mismatch; then
        local bash_exit_code="${b_lines[-1]}"
        local mini_exit_code="${m_lines[-1]}"
        echo -e "    \033[1;33mError Code Differences:\033[0m"
        echo -e "      \033[1;36mBash:\033[0m $bash_exit_code"
        echo -e "      \033[1;36mMinishell:\033[0m $mini_exit_code"
    fi
}

