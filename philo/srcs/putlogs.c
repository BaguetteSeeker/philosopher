/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   putlogs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 18:37:41 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/27 10:44:12 by epinaud          ###   ########.fr       */
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
		return (ft_putstr_fd("Node does not exist\n", 1));
	printf(" Philo Number [%ld]\n", lst->id);
}

void	display_state(t_guest *philo, size_t action)
{
	pthread_mutex_lock(&gset_dinner(0)->display_lock);
	if (action == EATING && !is_dinner_done())
		printf("%ld %ld is eating\n", time_since_start(), philo->id);
	else if (action == THINKING && !is_dinner_done())
		printf("%ld %ld is thinking\n", time_since_start(), philo->id);
	else if (action == SLEEPING && !is_dinner_done())
		printf("%ld %ld is sleeping\n", time_since_start(), philo->id);
	else if (action == DIED)
		printf("\033[31m%ld %ld died\033[0m\n", time_since_start(), philo->id);
	else if (action == PICKING_FORK && !is_dinner_done())
		printf("%ld %ld, has taken a fork\n", time_since_start(), philo->id);
	pthread_mutex_unlock(&gset_dinner(0)->display_lock);
}
