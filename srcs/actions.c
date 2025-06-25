/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/25 19:47:41 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*dine_alone(void *philo)
{
	usleep(gset_dinner(0)->life_duration / 0.001);
	display_state((t_guest *)philo, DIED);
	return (NULL);
}

size_t	eat(t_guest *philo)
{
	t_dinner	*table;

	table = gset_dinner(0);
	if (table->life_duration == 0
		|| time_since_epoch() - philo->last_meal > table->life_duration)
	{
		pthread_mutex_lock(&table->coordinator);
		table->is_done = true;
		pthread_mutex_unlock(&table->coordinator);
		display_state(philo, DIED);
		return (0);
	}
	if (&philo->fork_mutex < &philo->next->fork_mutex)
	{
		pthread_mutex_lock(&philo->fork_mutex);
		pthread_mutex_lock(&philo->next->fork_mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->next->fork_mutex);
		pthread_mutex_lock(&philo->fork_mutex);
	}
	// pthread_mutex_lock(&philo->fork_mutex);
	// pthread_mutex_lock(&philo->next->fork_mutex);
	display_state(philo, PICKING_FORK);
	usleep(table->meal_duration);
	philo->times_eaten++;
	philo->last_meal = time_since_epoch();
	pthread_mutex_unlock(&philo->fork_mutex);
	pthread_mutex_unlock(&philo->next->fork_mutex);
	return (1);
}

void	*launch_routine(void *v_philo)
{
	t_guest			*philo;
	t_dinner		*table;

	table = gset_dinner(0);
	philo = (t_guest *)v_philo;
	while (1)
	{
		if (table->argc == MAX_ARGS
			&& philo->times_eaten >= table->meals_required)
			break ;
		pthread_mutex_lock(&table->coordinator);
		if(table->is_done)
		{
			pthread_mutex_unlock(&table->coordinator);
			break;
		}
		pthread_mutex_unlock(&table->coordinator);
		if (!eat(philo))
			break ;
		display_state(philo, SLEEPING);
		usleep(table->sleep_duration);
		display_state(philo, THINKING);
	}
	return (NULL);
}
