/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:47:23 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/15 18:29:51 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#define _DEFAULT_SOURCE
#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

enum e_philo_state
{
	EATING,
	SLEEPING,
	THINKING,
	DIED
};

typedef struct s_cycle_counter
{
	size_t			life;
	size_t			meal;
	size_t			sleep;
}	t_cycle_counter;


typedef struct	s_philosopher
{
	void	*ptr;
	struct s_philosopher	*next;
	size_t			number;
	pthread_t		thread;
	pthread_mutex_t	fork_mutex;
	// t_cycle_counter	counter;/
	size_t			last_meal;
	size_t			meals_required;
	size_t			times_eaten;
}	t_philosopher;

typedef struct	s_dinner
{
	size_t			start_time;
	size_t			guest_count;
	size_t			life_duration;
	size_t			meal_duration;
	size_t			sleep_duration;
	size_t			meals_required;
	// t_fork			*forks;
	t_philosopher	*philos;
}	t_dinner;

// typedef struct	s_dinner
// {
// 	pthread_mutex_t	mutex;
// 	t_table_config	table;
// }	t_dinner;
void		put_err(char *msg);
t_dinner	*gset_dinner(void *g);
void		*print_thread(void *);
size_t		time_since_start(void);
size_t		time_since_epoch(void);
#endif
