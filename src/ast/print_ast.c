/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:17:41 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/25 14:49:52 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ast.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

t_ast_node *get_left_child(t_ast_node *node)
{
    if (node == NULL)
        return NULL;

    switch (node->type)
    {
        case NODE_AND:
        case NODE_OR:
            return node->data.logic.left;
        case NODE_PIPE:
            return node->data.pipe.left;
        case NODE_REDIRECT_OUT:
        case NODE_REDIRECT_IN:
        case NODE_REDIRECT_APPEND:
        case NODE_REDIRECT_HEREDOC:
            return node->data.redirect.child;
        case NODE_SUBSHELL:
            return node->data.subshell.child;
        default:
            return NULL;
    }
}

t_ast_node *get_right_child(t_ast_node *node)
{
    if (node == NULL)
        return NULL;

    switch (node->type)
    {
        case NODE_AND:
        case NODE_OR:
            return node->data.logic.right;
        case NODE_PIPE:
            return node->data.pipe.right;
        case NODE_REDIRECT_OUT:
        case NODE_REDIRECT_IN:
        case NODE_REDIRECT_APPEND:
            return NULL; // Filename will be displayed with the node
        case NODE_REDIRECT_HEREDOC:
            return NULL;
        default:
            return NULL;
    }
}

char *get_node_label(t_ast_node *node)
{
    if (node == NULL)
        return "";

    switch (node->type)
    {
        case NODE_COMMAND:
            return "\033[1;32m[CMD]\033[0m"; // Bright green
        case NODE_AND:
            return "\033[1;34m{&&}\033[0m"; // Bright blue
        case NODE_OR:
            return "\033[1;34m{||}\033[0m";
        case NODE_PIPE:
            return "\033[1;34m{|}\033[0m";
        case NODE_REDIRECT_OUT:
            return "\033[1;34m{>}\033[0m";
        case NODE_REDIRECT_IN:
            return "\033[1;34m{<}\033[0m";
        case NODE_REDIRECT_APPEND:
            return "\033[1;34m{>>}\033[0m";
        case NODE_REDIRECT_HEREDOC:
            return "\033[1;34m{<<}\033[0m";
        case NODE_SUBSHELL:
            return "\033[1;34m{( )}\033[0m";
        default:
            return "[UNKNOWN]";
    }
}

void print_node_content(t_ast_node *node)
{
    if (node == NULL)
        return;

    if (node->type == NODE_COMMAND)
    {
        t_cmd_node cmd = node->data.command;
        printf(" (");
        for (int i = 0; i < cmd.argc; i++)
        {
            printf("%s", cmd.argv[i]);
            if (i < cmd.argc - 1)
                printf(", ");
        }
        printf(")");
    }
    else if (node->type == NODE_REDIRECT_OUT || node->type == NODE_REDIRECT_IN ||
             node->type == NODE_REDIRECT_APPEND)
    {
        printf(" (File: %s)", node->data.redirect.filename);
    }
    else if (node->type == NODE_REDIRECT_HEREDOC)
    {
        printf(" (Delimiter: %s)", node->data.heredoc.delimiter);
    }
    // Add other node types as needed
}

void print_ast(t_ast_node *node, int depth, char *prefix, int is_left)
{
    if (node == NULL)
        return;

    // Print current node
    printf("%s", prefix);

    if (depth > 0)
    {
        if (is_left)
            printf("├── ");
        else
            printf("└── ");
    }
    else
    {
        // Root node
        printf("└── ");
    }

    // Node label
    printf("%s", get_node_label(node));

    // Node content
    print_node_content(node);

    printf("\n");

    // Prepare prefix for children
    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    // Print left child
    print_ast(get_left_child(node), depth + 1, new_prefix, 1);

    // Print right child
    print_ast(get_right_child(node), depth + 1, new_prefix, 0);
}

// Mock functions to create command nodes for testing
t_ast_node *create_command_node_with_args(int argc, ...)
{
    t_ast_node *node = malloc(sizeof(t_ast_node));
    if (!node)
        return NULL;

    node->type = NODE_COMMAND;
    node->data.command.argc = argc;
    node->data.command.argv = malloc(sizeof(char *) * (argc + 1));

    va_list args;
    va_start(args, argc);
    for (int i = 0; i < argc; i++)
    {
        char *arg = va_arg(args, char *);
        node->data.command.argv[i] = arg;
    }
    va_end(args);

    node->data.command.argv[argc] = NULL;

    return node;
}
