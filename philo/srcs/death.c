/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:09:28 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/04 19:57:16 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//Will predict death for philo when life_duration < meal_duration * 2
//Will not enter if meal_duration < sleep_duration
//	to continue up until philo dies in its sleep
// 	AND checks that
bool	dies_with_fork(t_guest *philo, pthread_mutex_t *fork)
{
	t_dinner	*table;
	size_t		death_time;

	table = gset_dinner(0);
	death_time = philo->last_meal + table->life_duration;
	if (philo->id % 2 && table->meal_duration >= table->sleep_duration
		&& table->life_duration < table->meal_duration * 2
		&& death_time < time_since_epoch() + (table->life_duration
			- (table->meal_duration + table->sleep_duration)))
	{
		display_state(philo, PICKING_FORK);
		usleep((table->life_duration
				- (table->meal_duration + table->sleep_duration)) / 0.001);
		check_death(philo);
		if (fork)
			pthread_mutex_unlock(fork);
		return (1);
	}
	else
		return (0);
}

void	check_death(t_guest *philo)
{
	t_dinner				*table;

	table = gset_dinner(0);
	if (table->life_duration == 0 || time_since_epoch()
		- philo->last_meal > table->life_duration)
	{
		pthread_mutex_lock(&table->coordinator);
		if (table->is_done)
		{
			pthread_mutex_unlock(&table->coordinator);
			return ;
		}
		table->is_done = true;
		pthread_mutex_unlock(&table->coordinator);
		display_state(philo, DIED);
		return ;
	}
}
