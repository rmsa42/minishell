/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 16:59:35 by rumachad          #+#    #+#             */
/*   Updated: 2024/03/25 12:37:25 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	swap_node(t_env *sorted_env, t_env *tmp)
{
	char	*buff;

	buff = sorted_env->var;
	sorted_env->var = tmp->var;
	tmp->var = buff;
	buff = sorted_env->var_value;
	sorted_env->var_value = tmp->var_value;
	tmp->var_value = buff;
}

void	declare_env(t_env *env)
{
	t_env	*srt_env;

	srt_env = NULL;
	srt_env = dup_env(env);
	sort_env(srt_env);
	no_args_exp(srt_env);
	free_env(srt_env);
	return ;
}

t_env	*dup_env(t_env *env)
{
	t_env	*new;
	t_env	*head;

	head = NULL;
	new = create_node_env(env->var, env->var_value, env->visible);
	head = new;
	env = env->next;
	while (env != NULL)
	{
		new->next = create_node_env(env->var, env->var_value, env->visible);
		new = new->next;
		env = env->next;
	}
	return (head);
}

char	*get_var_name(char *arg)
{
	char	*var_name;
	int		i;

	i = 0;
	while (arg[i] != '\0' && arg[i] != '=')
		i++;
	var_name = ft_substr(arg, 0, i);
	return (var_name);
}

void	no_args_exp(t_env *env)
{
	while (env)
	{
		printf("declare -x %s", env->var);
		if (env->visible == 1)
			printf("=");
		printf("\"%s\"\n", env->var_value);
		env = env->next;
	}
}
