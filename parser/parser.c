/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rumachad <rumachad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 11:32:42 by rumachad          #+#    #+#             */
/*   Updated: 2024/01/23 16:07:16 by rumachad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_generic	*parse_redir(t_lst_tokens **args, t_generic	*struct_pointer)
{
	t_id		redir_type;
	
	redir_type = get_redir_type((*args)->token);
	if (get_token_type((*args)->next) != WORD)
	{
		ft_fprintf(STDERR_FILENO, "Syntax error\n");
		return (struct_pointer);
	}
	(*args) = (*args)->next;
	(*args)->type = IGNORE;
	if (redir_type == REDIR_OUT)
		struct_pointer = redir_constructor(struct_pointer, 1,
			O_WRONLY | O_CREAT | O_TRUNC, (*args)->token);
	else if (redir_type == REDIR_IN)
		struct_pointer = redir_constructor(struct_pointer, 0, O_RDONLY, (*args)->token);
	else if (redir_type == APPEND)
		struct_pointer = redir_constructor(struct_pointer, 1,
			O_WRONLY | O_CREAT | O_APPEND, (*args)->token);
	return (struct_pointer);
}

t_generic	*parser_exec(t_lst_tokens **args)
{
	t_generic	*struct_pointer;
	t_exec		*exec_cast;

	struct_pointer = exec_constructor();
	exec_cast = (t_exec *)struct_pointer;
	while ((*args) && (*args)->type != PIPE)
	{
		if ((*args)->type == REDIR)
			struct_pointer = parse_redir(args, struct_pointer);
		if ((*args)->type != IGNORE)
		{
			if (exec_cast->argv != NULL)
				exec_cast->argv = ft_strjoin_get(exec_cast->argv, " ");
			exec_cast->argv = ft_strjoin_get(exec_cast->argv, (*args)->token);
		}
		(*args) = (*args)->next;
	}
	return (struct_pointer);
}

t_generic	*parser_pipe(t_lst_tokens **args, t_id *shell_state)
{
	t_generic	*struct_pointer;
	
	if (get_token_type(*args) == PIPE)
	{
		ft_fprintf(STDERR_FILENO, "Syntax error\n");
		return (NULL);
	}
	struct_pointer = parser_exec(args);
	if (struct_pointer && get_token_type((*args)) == PIPE)
	{
		(*args) = (*args)->next;
		if (get_token_type((*args)) != WORD)
		{
			ft_fprintf(STDERR_FILENO, "Syntax error\n");
			return (NULL);
		}
		struct_pointer = pipe_constructor(struct_pointer, parser_pipe(args, shell_state));
		*shell_state = PIPED;
	}
	return (struct_pointer);
}

t_generic	*parser_tokens(t_lst_tokens **args, t_id *shell_state)
{
	t_generic		*tree_root;
	t_lst_tokens	*tmp;

	tmp = (*args);
	if (tmp == NULL)
		return (NULL);
	*shell_state = NOT_PIPED;
	tree_root = parser_pipe(&tmp, shell_state);
	free_tokens(args);
	return (tree_root);
}
