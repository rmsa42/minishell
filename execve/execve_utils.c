/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/02 14:35:31 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/31 14:40:44 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error(int error)
{
	if (error == 1)
		ft_putstr_fd(": filename argument required\n", STDERR_FILENO);
	else if (error == 2)
		ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	else if (error == 3)
		ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	else if (error == 4)
		ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
	else
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
}

int	check_stat(char *path, struct stat *buffer, int *is_file, int *is_dir)
{
	int	check;

	*is_dir = 0;
	*is_file = 0;
	check = stat(path, buffer);
	if (check == -1)
		return (0);
	*is_dir = S_ISDIR(buffer->st_mode);
	*is_file = S_ISREG(buffer->st_mode);
	return (1);
}

int	execve_syntax(t_env *env, char *path)
{
	struct stat	buffer;
	int			is_file;
	int			is_dir;

	check_stat(path, &buffer, &is_file, &is_dir);
	if (ft_strlen(path) == 1 && path[0] == '.')
		return (g_exit_status = 2, 1);
	else if (is_dir == true && is_file == false)
		return (g_exit_status = 126, 2);
	else if ((is_file == false && (ft_strchr(path, '/') || !get_env(env, "PATH"))))
		return (g_exit_status = 127, 3);
	else if (is_file == true && access(path, X_OK) == -1)
		return (g_exit_status = 126, 4);
	else if (is_file == false && is_dir == false)
		return (g_exit_status = 127, 5);
	return (0);
}

void	change_shlvl(char **env_array, t_env *env)
{
	char	*shlvl_char;
	int		shlvl;
	int		i;
	int		k;

	i = 0;
	while (ft_strncmp(env->var, "SHLVL", 6) != 0)
	{
		env = env->next;
		i++;
	}
	k = 0;
	while (env_array[i][k + 1])
		k++;
	shlvl = ft_atoi(&env_array[i][k]);
	shlvl++;
	shlvl_char = ft_itoa(shlvl);
	free(env_array[i]);
	env_array[i] = ft_strjoin("SHLVL=", shlvl_char);
}
