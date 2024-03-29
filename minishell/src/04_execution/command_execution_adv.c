/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_execution_adv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: beddinao <beddinao@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 10:01:18 by beddinao          #+#    #+#             */
/*   Updated: 2024/03/10 10:20:35 by beddinao         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command_basic(char **_cmd_, int *_fd, char **env, int *_piped)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		if (_piped[0] && _piped[0] <= _piped[5])
			dup2(_fd[0], 0);
		if (_piped[0] > 1)
			dup2(fd_[1], 1);
		else
			close(_fd[0]);
		close_pipe_ends(fd_[0], fd_[1]);
		execve(_cmd_[0], _cmd_, env);
		(ft_putendl_fd(strerror(errno), 2), exit(127));
	}
	close_pipe_ends(fd_[1], _fd[0]);
	if (_piped[0] > 1)
		_fd[0] = fd_[0];
	else
		close(fd_[0]);
	return (1);
}

int	execute_command_with_redirection(
		char **_cmd_, int *_fd, char **env, int *_piped)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
		child_fds_managment(_piped, _fd, fd_);
		execve(_cmd_[0], _cmd_, env);
		ft_putendl_fd(strerror(errno), 2);
		exit(127);
	}
	parent_fds_managment(_piped, _fd, fd_);
	free_string_array(_cmd_);
	return (1);
}

int	prepare_and_execute_command(
		char **_cmd_, int *_fd, int *_piped, t_env *env)
{
	char				**cmd_args;
	char				**f_args;
	int					status;

	f_args = prepare_cmd_arguments(_cmd_[0], env->original_env, 0);
	cmd_args = merge_command_args(f_args, _cmd_);
	if (check_if_command_is_builtin(cmd_args[0]))
		status = manage_builtin_execution(cmd_args, _fd, env, _piped);
	else
	{
		_piped[10] += 1;
		if (!_piped[8])
		{
			status = execute_command_basic(
					cmd_args, _fd, env->original_env, _piped);
			free_string_array(cmd_args);
		}
		else
			status = execute_command_with_redirection(
					cmd_args, _fd, env->original_env, _piped);
	}
	if (_piped[0] > 1)
		_piped[0] -= 1;
	return (status);
}

int	wait_for_children(int status, int *_piped)
{
	if (status != 2 && status != 127
		&& status != 126 && _piped[10]
		&& _piped[11])
	{
		while (_piped[10])
		{
			wait(&status);
			_piped[10] -= 1;
		}
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!g_var_thing)
			return (WEXITSTATUS(status));
		else
			return (g_var_thing);
	}
	return (status);
}
