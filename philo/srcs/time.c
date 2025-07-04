/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:18:47 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/04 22:45:06 by epinaud          ###   ########.fr       */
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

// Improved version of sleep function
void	ft_usleep(size_t milliseconds, t_guest *philo)
{
	size_t	start;
	size_t	elapsed;
	size_t	epoch_time;
	size_t	sleep_ms;

	start = time_since_start();
	while (1)
	{
		check_death(philo);
		elapsed = time_since_start() - start;
		epoch_time = time_since_epoch();
		if (is_dinner_done() || elapsed >= milliseconds)
			break ;
		if (philo->last_meal + gset_dinner(0)->life_duration
			< epoch_time + milliseconds - elapsed)
			sleep_ms = philo->last_meal
				+ gset_dinner(0)->life_duration - epoch_time;
		else
			sleep_ms = milliseconds - elapsed;
		if (sleep_ms > 2)
			usleep((sleep_ms - 1) / 0.001);
		else
			usleep(100);
	}
}
