/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/19 20:49:00 by epinaud          ###   ########.fr       */
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
	if (time_since_epoch() - guest->last_meal > table->life_duration)
	{
		display_state(guest, DIED);
		table->is_done = true;
		return ;
	}
	pthread_mutex_lock(&guest->fork_mutex);
	pthread_mutex_lock(&guest->next->fork_mutex);
	display_state(guest, PICKING_FORK);
	usleep(table->meal_duration);
	guest->times_eaten++;
	guest->last_meal = time_since_epoch();
	pthread_mutex_unlock(&guest->fork_mutex);
	pthread_mutex_unlock(&guest->next->fork_mutex);
}

void	*launch_routine(void *v_philo)
{
	t_philosopher	*philo;
	t_dinner	*table;

	table = gset_dinner(0);
	philo = (t_philosopher *)v_philo;
	printf("Je suis le philo %lu\n", philo->number);
	while (!table->is_done)
	{
		eat(philo);
		if (table->is_done)
			break ;
		display_state(philo, SLEEPING);
		usleep(table->sleep_duration);
		display_state(philo, THINKING);
	}
	return (exit(0), NULL);
}