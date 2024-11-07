/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 09:35:56 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/07 18:50:14 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

# include "../lib/libgc/include/libgc.h"
# include "../lib/libft/include/libft.h"
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <dirent.h>

typedef struct s_proc_info
{
	pid_t				pid;
	pid_t				parent_pid;
	int					child_lvl;
	int					fd_in;
	int					fd_out;
	int					fd_error;
	int					fd_original_stdout;
	bool				is_heredoc;
	const char			*title;
	struct s_proc_info	*next;
}				t_proc_info;

typedef struct s_proc_manager
{
	t_proc_info	*head;
	int			process_count;
	pid_t		parent_pid;
	t_gc		*gcl;
}				t_proc_manager;

typedef struct s_proc_params
{
	pid_t		pid;
	pid_t		parent_pid;
	int			child_lvl;
	int			fd_in;
	int			fd_out;
	int			fd_error;
	bool		is_heredoc;
	const char	*title;
}			t_proc_params;

typedef struct s_fork_params
{
	int			child_lvl;
	int			fd_in;
	int			fd_out;
	int			fd_error;
	bool		is_heredoc;
	const char	*title;
}			t_fork_params;

/* Process Manager Init and cleanup */
t_proc_manager	*init_manager(t_gc *gcl);
void			clean_manager(t_proc_manager *manager);

/* Builder and management */
t_proc_info		*add_process(t_proc_manager *manager, t_proc_params *params);
void			cleanup_proc(t_proc_manager *manager, pid_t pid);

/* Safe wrappers for System Calls */
pid_t			safe_fork(t_proc_manager *manager, t_fork_params *params);
int				safe_pipe(int pipefd[2]);
int				safe_open(const char *filepath, int flags, int mode);
int				safe_close(int fd);

/* File descriptor management */
int				redirect_fd(int old_fd, int new_fd);
int				restore_fd(int fd, int saved_fd);

/* Context restoration */
int				back_to_parent_stdout(t_proc_manager *manager, pid_t pid);

/* Debugging and Logging */
void			print_proc_info(const t_proc_manager *manager);

#endif
