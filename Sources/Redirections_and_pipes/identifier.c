/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identifier.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/21 19:48:35 by abello-r          #+#    #+#             */
/*   Updated: 2024/11/06 17:40:40 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Includes/minishell.h"

void ft_execute_builtin(t_data *data)
{
	if (ft_strncmp(data->token->content, "pwd", ft_strlen("pwd")) == 0)
		ft_pwd();
	else if (ft_strncmp(data->token->content, "env", ft_strlen("env")) == 0)
		ft_env(data);
	else if (ft_strncmp(data->token->content, "unset", ft_strlen("unset")) == 0)
		ft_unset(data);
	else if (ft_strncmp(data->token->content, "export", ft_strlen("export")) == 0)
		ft_export(data);
	else if (ft_strncmp(data->token->content, "cd", ft_strlen("cd")) == 0)
		ft_cd(data);
	else if (ft_strncmp(data->token->content, "echo", ft_strlen("echo")) == 0)
		ft_echo(data);
	else if (ft_strncmp(data->token->content, "exit", ft_strlen("exit")) == 0)
		ft_exit(data);
	else
		ft_check_cmd_on_path(data);
}

int	ft_handle_redirections_and_pipes(t_data *data)
{
    t_token *current_token = data->token;
    int in_fd = STDIN_FILENO;
    int out_fd = STDOUT_FILENO;
    int pipe_fds[2];
    int has_pipe = 0;
    pid_t pid;
    int status;
    int exit_status = 0;

    // Paso 1: Comprobar si hay pipes o redirecciones en la lista de tokens
    while (current_token)
    {
        if (strcmp(current_token->type, "PIPE") == 0)
            has_pipe = 1;  // Indica que se encontró un pipe
        else if (strcmp(current_token->type, "OUT") == 0 || strcmp(current_token->type, "APPEND") == 0)
            out_fd = (strcmp(current_token->type, "OUT") == 0) ?
                     open(current_token->next->content, O_WRONLY | O_CREAT | O_TRUNC, 0644) :
                     open(current_token->next->content, O_WRONLY | O_CREAT | O_APPEND, 0644);
        else if (strcmp(current_token->type, "INPUT") == 0)
            in_fd = open(current_token->next->content, O_RDONLY);
        
        // Si hubo algún error al abrir archivos, devolver error inmediato
        if (out_fd < 0 || in_fd < 0)
            return -1;
        
        current_token = current_token->next;
    }

	if (is_builtin(data->token->content) && strcmp(data->token->content, "exit") == 0)
        return (ft_exit(data), 0);

    // Paso 2: Configurar pipe si es necesario
    if (has_pipe && pipe(pipe_fds) < 0)
        return -1;  // Error al crear el pipe

    // Paso 3: Ejecutar el comando, manejando pipes y redirecciones
    if ((pid = fork()) == 0)
    {
        // Proceso hijo: Configurar redirección y ejecución de comando
        if (in_fd != STDIN_FILENO)
        {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (has_pipe)
        {
            dup2(pipe_fds[1], STDOUT_FILENO); // Redirige la salida al pipe
            close(pipe_fds[0]);               // Cierra el extremo de lectura
        }
        else if (out_fd != STDOUT_FILENO)
        {
            dup2(out_fd, STDOUT_FILENO); // Redirige salida al archivo
            close(out_fd);
        }
        // Verificar si el comando es builtin y ejecutarlo, si no, llamar execve
        if (is_builtin(data->token->content))
        	ft_execute_builtin(data);
        else
            ft_check_cmd_on_path(data);
        exit(EXIT_FAILURE); // En caso de fallo en execve o comando
    }
    else if (pid > 0)
    {
        // Proceso padre: Esperar el proceso hijo y manejar el extremo del pipe
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            exit_status = WEXITSTATUS(status);
        if (has_pipe)
        {
            close(pipe_fds[1]);       // Cerrar el extremo de escritura
            in_fd = pipe_fds[0];      // El siguiente comando leerá del extremo de lectura del pipe
        }
    }
    else
        return -1; // Error en fork

    // Cerrar descriptores si fueron modificados
    if (in_fd != STDIN_FILENO)
        close(in_fd);
    if (out_fd != STDOUT_FILENO)
        close(out_fd);

    return (exit_status); // Devolver el estado de salida del último comando ejecutado
}
