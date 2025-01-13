#!/bin/bash
# report_helpers.sh
#
# Contains helper functions used by results.sh
# that handle smaller, isolated pieces of logic
# (counting commands, handling heredocs, parsing outputs, etc.)

# -------------------------------------------------------
# count_commands():
#   Reads commands in a file, accounting for heredocs.
#   Sets global variable "total_commands".
# -------------------------------------------------------
count_commands() {
    local commands_file="$1"

    total_commands=0

    local in_multiline=0
    local multiline_end_token="EOF"

    while IFS= read -r line || [[ -n "$line" ]]; do
        local trimmed_line
        trimmed_line=$(echo "$line" | sed 's/^[ \t]*//;s/[ \t]*$//')

        [[ -z "$trimmed_line" ]] && continue

        if [[ $in_multiline -eq 1 ]]; then
            if [[ "$trimmed_line" == "$multiline_end_token" ]]; then
                in_multiline=0
            fi
            continue
        fi

        if echo "$trimmed_line" | grep -q '<<[[:space:]]*[A-Za-z0-9_]\+'; then
            multiline_end_token=$(echo "$trimmed_line" \
                | grep -o '<<[[:space:]]*[A-Za-z0-9_]\+' \
                | sed 's/<<[[:space:]]*//')
            in_multiline=1
            ((total_commands++))
            continue
        fi

        ((total_commands++))
    done < "$commands_file"
}

# -------------------------------------------------------
# filter_heredocs():
#   Removes heredoc “body” lines, storing only the
#   “header” (i.e. the line containing '<<') in the output.
# -------------------------------------------------------
filter_heredocs() {
    local -n in_cmds_ref="$1"
    local -n out_filtered_ref="$2"

    local in_heredoc=0
    local heredoc_delimiter=""
    out_filtered_ref=()

    for cmd in "${in_cmds_ref[@]}"; do
        local trimmed_line
        trimmed_line=$(echo "$cmd" | sed 's/^[ \t]*//;s/[ \t]*$//')

        [[ -z "$trimmed_line" ]] && continue

        if [[ $in_heredoc -eq 1 ]]; then
            # Are we at the end of the heredoc?
            if [[ "$trimmed_line" == "$heredoc_delimiter" ]]; then
                in_heredoc=0
            fi
        else
            # Check if this line starts a heredoc
            if [[ "$trimmed_line" =~ '<<[[:space:]]*([A-Za-z0-9_]+)' ]]; then
                heredoc_delimiter="${BASH_REMATCH[1]}"
                in_heredoc=1
                out_filtered_ref+=("$trimmed_line")
            else
                out_filtered_ref+=("$trimmed_line")
            fi
        fi
    done
}

# -------------------------------------------------------
# build_final_test_names():
#   If there's a heredoc, we only want lines that contain '<<'.
#   Otherwise, we use the entire filtered_commands array.
# -------------------------------------------------------
build_final_test_names() {
    local -n filtered_ref="$1"
    local -n final_names_ref="$2"
    local has_heredoc="$3"

    if $has_heredoc; then
        final_names_ref=()
        for c in "${filtered_ref[@]}"; do
            [[ "$c" == *'<<'* ]] && final_names_ref+=("$c")
        done
    else
        final_names_ref=("${filtered_ref[@]}")
    fi
}

# -------------------------------------------------------
# parse_outputs():
#   Reads a single output file into an array, splitting at "@minishell $>".
# -------------------------------------------------------
parse_outputs() {
    local output_file="$1"
    local -n outputs_ref="$2"

    outputs_ref=()
    local current_output=""
    local is_first_prompt_found=false

    while IFS= read -r line || [[ -n "$line" ]]; do
        if [[ "$line" == *"@minishell \$>"* ]]; then
            if $is_first_prompt_found; then
                outputs_ref+=("$current_output")
            else
                is_first_prompt_found=true
            fi
            current_output="$line"$'\n'
        else
            current_output+="$line"$'\n'
        fi
    done < "$output_file"

    if [[ -n "$current_output" && "$current_output" != *"exit"* ]]; then
        outputs_ref+=("$current_output")
    fi
}
