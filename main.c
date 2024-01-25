/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rui <rui@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/06 12:47:06 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/25 01:22:42 by rui              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* char	*ds_expand(char *token, t_env *env)
{
	char	*expanded_token;
	char	*var;
	char	*val;
	int		i;

	i = 0;
	expanded_token = NULL;
	while (token[i])
	{
		token++;
		i++;
		while (token[i] && token[i] != '$')
			i++;
		var = ft_substr(token, 0, i);
		val = get_env_val(env, var);
		expanded_token = ft_strjoin_get(expanded_token, val);
		free(var);
		free(val);
		token = token + i;
		i = 0;
	}
	return (expanded_token);
} */

t_generic	*lexer_parser(t_minishell *shell, t_lst_tokens **args)
{
	t_generic	*cmd;
	
	*args = NULL;
	add_history(shell->rl_str);
	*args = make_tokens(shell, *args);
	free_first(args);
	free(shell->rl_str);
	/* while ((*args))
	{
		printf("%s\n", (*args)->token);
		printf("%d\n", (*args)->type);
		(*args) = (*args)->next;
	} */
	cmd = parser_tokens(shell->env, args);
	return (cmd);
}

int main(int ac, char **av, char **envp)
{
	t_minishell		shell;
	t_lst_tokens	*args;
	t_generic		*cmd;

	ft_memset((void *)&shell, 0, sizeof(t_minishell));
	if (ac != 1 && av)
		return (0);
	shell.env = lst_env(envp);
	while (1)
	{
		// 1.Read Command
		shell.rl_str = readline("minishell$ ");
		if (ft_strlen(shell.rl_str) == 0)
			continue;
		if (unclosed_quotes(shell.rl_str) == YES)
			continue;
		cmd = lexer_parser(&shell, &args);
		if (cmd)
		{
			executer_cmd(&shell, cmd);
			free_tree(cmd);
		}
	}
	return (0);
}
