/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:48:35 by abello-r          #+#    #+#             */
/*   Updated: 2024/11/04 14:12:29 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

/*int		ft_handle_redirections_and_pipes(t_data *data)
{	
	(void)data;
	//printf("Redirections and pipes handled\n");
	return (0);
}
if (type[0] == '|')
		return ("PIPE");
	else if (type[0] == '>' && type[1] == '>')
		return ("APPEND");
	else if (type[0] == '>')
		return ("OUT");
	else if (type[0] == '<' && type[1] == '<')
		return ("HEREDOC");
	else if (type[0] == '<')
		return ("INPUT");
*/

/*int is_pipe(const char *token) {
    return (ft_strncmp(token, "|", 1) == 0);
}

int ft_create_pipe(int prev_pipe) {
    int pipe_fd[2];

    if (pipe(pipe_fd) == -1) {
        ft_print_exit("PIPE ERROR");
    }

    if (prev_pipe != -1) {
        close(prev_pipe);
    }

    return pipe_fd[0];  // Retorna el descriptor de lectura
}

int is_redirection(t_token *token) {
    return (ft_strncmp(token->content, "<", 1) == 0 ||
            ft_strncmp(token->content, ">", 1) == 0 ||
            ft_strncmp(token->content, ">>", 2) == 0);
}

void ft_setup_redirection(t_token *cmd) {
    int fd = -1;

    if (cmd->next == NULL) {
        fprintf(stderr, "Error: No file specified for redirection.\n");
        return;
    }

    if (ft_strncmp(cmd->content, "<", 1) == 0) {
        fd = open(cmd->next->content, O_RDONLY);
        if (fd == -1) {
            perror("Error opening file for reading");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDIN_FILENO);
    } else if (ft_strncmp(cmd->content, ">", 1) == 0) {
        fd = open(cmd->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error opening file for writing");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
    } else if (ft_strncmp(cmd->content, ">>", 2) == 0) {
        fd = open(cmd->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1) {
            perror("Error opening file for appending");
            exit(EXIT_FAILURE);
        }
        dup2(fd, STDOUT_FILENO);
    }

    if (fd != -1) {
        close(fd);
    }
}

void ft_execute_with_redirection(t_data *data, t_token *cmd, int *prev_pipe) {
    int pipe_fd[2];
    int saved_stdout = dup(STDOUT_FILENO);

    if (is_redirection(cmd)) {
        ft_setup_redirection(cmd);
    }

    if (*prev_pipe != -1) {
        dup2(*prev_pipe, STDIN_FILENO);
        close(*prev_pipe);
    }

    if (cmd->next && is_pipe(cmd->next->content)) {
        pipe(pipe_fd);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }

    if (ft_execute_cmd(data, cmd->content) == -1) {
        fprintf(stderr, "Error executing command: %s\n", cmd->content);
    }

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    *prev_pipe = pipe_fd[0];
}

int ft_handle_redirections_and_pipes(t_data *data) {
    t_token *current_cmd;
    int prev_pipe;

    current_cmd = data->token;
    prev_pipe = -1;
    while (current_cmd) {
        if (is_pipe(current_cmd->content)) {
            prev_pipe = ft_create_pipe(prev_pipe);
        } else {
            ft_execute_with_redirection(data, current_cmd, &prev_pipe);
        }
        current_cmd = current_cmd->next;
    }
    return 0;
}*/
