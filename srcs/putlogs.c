/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putlogs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:37:41 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/22 01:03:09 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ;
	while (*s)
		write(fd, s++, 1);
}

void	put_err(char *msg)
{
	if (msg && *msg)
	{
		ft_putstr_fd(msg, STDERR_FILENO);
		write(STDERR_FILENO, "\n", 1);
	}
	exit(EXIT_FAILURE);
}

void	lst_put(t_guest *lst)
{
	if (!lst)
		return (ft_putendl_fd("Node does not exist", 1));
	printf(" Philo Number [%ld]\n", lst->id);
}

void	*print_thread(void *data)
{
	t_guest	*philo;

	philo = (t_guest *)data;
	printf("Je suis le philo %lu\n", philo->id);
	return (NULL);
}

void	display_state(t_guest *philo, size_t action)
{
	pthread_mutex_lock(&gset_dinner(0)->coordinator);
	if (action == EATING)
		printf("%ld Philo %ld is eating\n", time_since_start(), philo->id);
	else if (action == THINKING)
		printf("%ld Philo %ld is thinking\n", time_since_start(), philo->id);
	else if (action == SLEEPING)
		printf("%ld Philo %ld is sleeping\n", time_since_start(), philo->id);
	else if (action == DIED)
		printf("%ld Philo %ld has DIED\n", time_since_start(), philo->id);
	else
		printf("%ld Philo %ld, lifted its forks\n",
			time_since_start(), philo->id);
	pthread_mutex_unlock(&gset_dinner(0)->coordinator);
}
