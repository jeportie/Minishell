/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_ast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 15:17:41 by jeportie          #+#    #+#             */
/*   Updated: 2024/10/24 15:21:35 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include "../../include/ast.h"

// Helper function to get the left child of a node
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
        case NODE_REDIRECT_IN:
        case NODE_REDIRECT_OUT:
        case NODE_REDIRECT_APPEND:
        case NODE_REDIRECT_HEREDOC:
            return node->data.redirect.child;
        case NODE_SUBSHELL:
            return node->data.subshell.child;
        default:
            return NULL;
    }
}

// Helper function to get the right child of a node
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
        default:
            return NULL;
    }
}

// Helper function to get a label for a node
char *get_node_label(t_ast_node *node)
{
    if (node == NULL)
        return "";

    switch (node->type)
    {
        case NODE_COMMAND:
            return "[CMD]";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_PIPE:
            return "|";
        case NODE_REDIRECT_OUT:
            return ">";
        case NODE_REDIRECT_IN:
            return "<";
        case NODE_REDIRECT_APPEND:
            return ">>";
        case NODE_REDIRECT_HEREDOC:
            return "<<";
        case NODE_SUBSHELL:
            return "( )";
        default:
            return "[UNKNOWN]";
    }
}

// Helper function to print the content of a node
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

// Main function to print the AST
void print_ast(t_ast_node *node, int depth, char *prefix, int is_left)
{
    if (node == NULL)
        return;

    char new_prefix[256];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    // Print right child first (so it appears on top)
    print_ast(get_right_child(node), depth + 1, new_prefix, 0);

    // Print current node
    printf("%s", prefix);

    if (depth > 0)
    {
        if (is_left)
            printf("├── ");
        else
            printf("└── ");
    }

    // Node label
    printf("%s", get_node_label(node));

    // Node content
    print_node_content(node);

    printf("\n");

    // Print left child
    print_ast(get_left_child(node), depth + 1, new_prefix, 1);
}
