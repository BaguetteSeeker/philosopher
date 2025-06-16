/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/16 20:33:10 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	time_since_epoch(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) == -1)
		put_err("Failled to fetch time");
	return (time.tv_sec * 1000L + time.tv_usec / 1000);
}

size_t	time_since_start(void)
{
	return (time_since_epoch() - gset_dinner(0)->start_time);
}

void	safe_usleep(int duration_ms, long start_time, t_dinner *dinner)
{
	long	elapsed;
	long	remaining;

	(void)dinner;
	while (1)
	{
		elapsed = time_since_start() - start_time;
		// if (is_dinner_end(dinner))
		// 	break ;
		if (elapsed >= duration_ms)
			break ;
		remaining = duration_ms - elapsed;
		if (remaining > 2)
			usleep((remaining - 1) * 1000);
		else
			usleep(100);
	}
}


// Improved version of sleep function
int	ft_usleep(size_t milliseconds)
{
	size_t	start;

	start = time_since_start();
	while ((time_since_start() - start) < milliseconds)
		usleep(100);
	exit(0);
}

void	eat(t_philosopher *guest)
{
	t_dinner	*table;

	table = gset_dinner(0);
	if (guest->last_meal + table->life_duration < time_since_start())
		table->is_done = true;
	if (table->is_done)
		exit(0);
	pthread_mutex_lock(&guest->fork_mutex);
	pthread_mutex_lock(&guest->next->fork_mutex);
	display_state(guest, PICKING_FORK);
	usleep(table->meal_duration);
	guest->times_eaten++;
	guest->last_meal = time_since_start();
	pthread_mutex_unlock(&guest->fork_mutex);
	pthread_mutex_unlock(&guest->next->fork_mutex);
}
