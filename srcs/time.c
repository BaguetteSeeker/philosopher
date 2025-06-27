/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:18:47 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/27 09:27:12 by epinaud          ###   ########.fr       */
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
	t_dinner	*dinner;
	size_t		start;

	dinner = gset_dinner(0);
	start = time_since_start();
	while ((time_since_start() - start) < milliseconds)
	{
		pthread_mutex_lock(&dinner->coordinator);
		if (dinner->is_done)
		{
			pthread_mutex_unlock(&dinner->coordinator);
			break;
		}
		pthread_mutex_unlock(&dinner->coordinator);
		usleep(100);
	}
	exit(0);
}
