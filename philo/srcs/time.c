/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:18:47 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/29 19:13:33 by epinaud          ###   ########.fr       */
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

void	safe_usleep(int duration_ms, long start_time)
{
	long	elapsed;
	long	remaining;

	while (1)
	{
		elapsed = time_since_start() - start_time;
		if (is_dinner_done())
			break ;
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
/* printf("Counter : %ld Time remaining : %ld 
\n", milliseconds, (time_since_start() - start)); */
// void	ft_usleep(size_t milliseconds,t_guest *philo)
// {
// 	size_t	start;
// 	size_t	elapsed;
// 	size_t	elapsed_epoch;

// 	start = time_since_start();
// 	while (1)
// 	{
// 		check_death(philo);
// 		elapsed = time_since_start() - start;
// 		if (is_dinner_done() || elapsed >= milliseconds)
// 			break ;
// 		if (milliseconds - elapsed > 2)
// 		{
// 		elapsed_epoch = time_since_epoch();
// 		if (philo->last_meal + gset_dinner(0)->life_duration
// 			< elapsed_epoch + milliseconds - 1)
// 			if (philo->last_meal + gset_dinner(0)->life_duration - elapsed_epoch > 2)
// 			{
// 				printf("%zu usleeping %zu ms\n", philo->id, philo->last_meal + gset_dinner(0)->life_duration - time_since_epoch());
// 				usleep((philo->last_meal + gset_dinner(0)->life_duration - elapsed_epoch) * 1000);
// 			}
// 			else
// 				usleep(100);
// 		else
// 		{
// 			if (milliseconds - elapsed > 2)
// 				usleep((milliseconds - elapsed - 1) * 1000);
// 			else
// 				usleep(100);
// 		}
// 		}
// 	}
// }

void	ft_usleep(size_t milliseconds, t_guest *philo)
{
	size_t	start;
	size_t	elapsed;
	size_t	elapsed_epoch;
	size_t	sleep_ms;

	start = time_since_start();
	while (1)
	{
		check_death(philo);
		elapsed = time_since_start() - start;
		if (is_dinner_done() || elapsed >= milliseconds)
			break ;
		elapsed_epoch = time_since_epoch();
		if (philo->last_meal + gset_dinner(0)->life_duration
			< elapsed_epoch + milliseconds)
		{
			sleep_ms = philo->last_meal + gset_dinner(0)->life_duration - elapsed_epoch;
			printf("%zu usleeping %zu ms\n", philo->id, sleep_ms);
		}
		else
			sleep_ms = milliseconds - elapsed;
		if (sleep_ms > 2)
			usleep((sleep_ms - 1) / 0.001);
		else
			usleep(100);
	}
}
