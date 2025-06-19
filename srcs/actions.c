/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/14 15:19:30 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/20 00:46:41 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	t_dinner		*table;

	table = gset_dinner(0);
	philo = (t_philosopher *)v_philo;
	while (!table->is_done)
	{
		if (table->argc == MAX_ARGS
			&& philo->times_eaten >= table->meals_required)
			break ;
		eat(philo);
		display_state(philo, SLEEPING);
		usleep(table->sleep_duration);
		display_state(philo, THINKING);
	}
	return (NULL);
}
