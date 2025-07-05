/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 16:09:28 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/05 18:20:41 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//Handles the case where a philo would die while trying to pickup a fork
//Typically occurs when :
// meal_duration >= sleep_duration
// AND ([for ODD guest_count] life duration is < meal_duration * 3
// || [for EVEN guest_count] life duration is < meal_duration * 2)
// AND it is detected that philo will actually die before being given the fork
//Ex: ./philo 4 310 200 100
bool	dies_with_fork(t_guest *philo, pthread_mutex_t *fork)
{
	t_dinner	*table;
	size_t		rounds_till_death;
	size_t		starvation;
	size_t		predicted_death;

	table = gset_dinner(0);
	predicted_death = table->life_duration
		- (table->meal_duration + table->sleep_duration);
	if (table->guest_count % 2)
		rounds_till_death = 3;
	else
		rounds_till_death = 2;
	starvation = philo->last_meal + table->life_duration;
	if (table->meal_duration >= table->sleep_duration
		&& (table->life_duration < table->meal_duration * rounds_till_death)
		&& starvation < time_since_epoch() + predicted_death)
	{
		ft_usleep(predicted_death, philo);
		check_death(philo);
		if (fork)
			pthread_mutex_unlock(fork);
		return (true);
	}
	return (false);
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
