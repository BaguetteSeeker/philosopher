/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putlogs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:37:41 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/20 00:46:30 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	put_err(char *msg)
{
	if (msg && *msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	exit(EXIT_FAILURE);
}

void	lst_put(t_philosopher *lst)
{
	if (!lst)
		return (ft_putendl_fd("Node does not exist", 1));
	printf(" Philo Number [%ld]\n", lst->id);
}

void	*print_thread(void *data)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)data;
	printf("Je suis le philo %lu\n", philo->id);
	return (NULL);
}

void	display_state(t_philosopher *philo, size_t action)
{
	if (action == EATING)
		printf("%ld Philo %ld is eating\n", time_since_start(), philo->id);
	else if (action == THINKING)
		printf("%ld Philo %ld is thinking\n", time_since_start(), philo->id);
	else if (action == SLEEPING)
		printf("%ld Philo %ld is sleeping\n", time_since_start(), philo->id);
	else if (action == DIED)
		printf("%ld Philo %ld has DIED\n", time_since_start(), philo->id);
	else
		printf("%ld Philo %ld, lifted its forks\n", time_since_start(), philo->id);
}
