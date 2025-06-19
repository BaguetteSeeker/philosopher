/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putlogs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:37:41 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/18 18:38:24 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	put_err(char *msg)
{
	// if (errno)
	// 	perror(msg);
	if (msg && *msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	// clean_shell();
	exit(EXIT_FAILURE);
}

void	lst_put(t_philosopher *lst)
{
	if (!lst)
		return (ft_putendl_fd("Node does not exist", 1));
	ft_printf(" Philo Number [%d]\n", lst->number);
}

void	*print_thread(void *data)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)data;
	printf("Je suis le philo %lu\n", philo->number);
	eat(philo);
	return (NULL);
}

void	display_state(t_philosopher *philo, size_t action)
{
	if (action == EATING)
		printf("Philosopher number %ld is eating\n", philo->number);
	else if (action == THINKING)
		printf("Philosopher number %ld is thinking\n", philo->number);
	else if (action == SLEEPING)
		printf("Philosopher number %ld is sleeping\n", philo->number);
	else if (action == DIED)
		printf("Philosopher number %ld has DIED\n", philo->number);
	else
		printf("Philosopher number %ld, has taken its forks\n", philo->number);
}