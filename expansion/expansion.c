/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:46:49 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/29 14:52:20 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_val(t_env *env, char *str)
{
	char	*env_value;

	env_value = get_env(env, str);
	if (env_value == NULL)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}

char	*get_value(char *token, int *i, t_env *env)
{
	int		start;
	char	*var;
	char	*value;

	(*i)++;
	start = *i;
	if (token[*i] == '?')
		return (++(*i), ft_itoa(g_exit_status));
	while (token[*i] && token[*i] != '"' && token[*i] != '\''
		&& token[*i] != '$')
		(*i)++;
	var = ft_substr(token, start, (*i) - start);
	value = get_env_val(env, var);
	free(var);
	return (value);
}

int	expand(char **token, int i, char *behind_expand, t_env *env)
{
	int		len_skip;
	char	*value;
	char	*join;

	value = get_value(token[0], &i, env);
	len_skip = ft_strlen(value);
	join = ft_strjoin(behind_expand, value);
	free(value);
	free(behind_expand);
	join = ft_strjoin_get(join, token[0] + i);
	free(token[0]);
	token[0] = ft_strdup(join);
	free(join);
	return (len_skip);
}

char	*handle_ds(t_env *env, char *token)
{
	char	*behind_expand;
	int		squotes;
	int		dquotes;
	int		i;

	i = 0;
	squotes = 0;
	dquotes = 0;
	while (token[i])
	{
		if (token[i] == '\'' && !dquotes)
			squotes = !squotes;
		else if (token[i] == '"' && !squotes)
			dquotes = !dquotes;
		else if (token[i] == '$' && !squotes)
		{
			behind_expand = ft_substr(token, 0, i);
			i = i + expand(&token, i, behind_expand, env);
			continue ;
		}
		i++;
	}
	if (token[0] == '\0')
		return (free(token), NULL);
	return (token);
}

char	*expand_token(t_env *env, char *token)
{
	if (token[0] == '~')
		token = expand_tilde(env, token);
	else
		token = handle_ds(env, token);
	return (token);
}
