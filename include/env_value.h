/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_value.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeportie <jeportie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 17:42:02 by jeportie          #+#    #+#             */
/*   Updated: 2024/11/06 20:24:15 by jeportie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_VALUE_H
# define ENV_VALUE_H

# include "exec.h"

char	*ms_get_env_value(t_env *env, char *name);
void	ms_set_env_value(t_shell *shell, char *name, char *value);

#endif /* ENV_VALUE_H */
