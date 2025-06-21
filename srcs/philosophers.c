/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 21:25:35 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/22 01:00:35 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

//Deal with atoi 0 return edge case
int	set_table(int argc, char *args[])
{
	size_t				val;
	static size_t		i = 0;
	static size_t		j = 0;
	t_dinner			*dinner;

	dinner = gset_dinner(0);
	while (args[i])
	{
		val = ft_atoi(args[i]);
		j = 0;
		while (args[i][j])
			if (!ft_isdigit(args[i][j++]))
				put_err("Given parameter is not a number");
		if (i == 0)
			dinner->guest_count = val;
		else if (i == 1)
			dinner->life_duration = val;
		else if (i == 2)
			dinner->meal_duration = val / 0.001;
		else if (i == 3)
			dinner->sleep_duration = val / 0.001;
		else if (i == 4)
			dinner->meals_required = val;
		i++;
		dinner->argc = argc;
	}
	pthread_mutex_init(&dinner->coordinator, NULL);
	return (1);
}

t_guest	*create_philos(t_guest **head, t_dinner dinner)
{
	size_t			i;
	t_guest	*thinker;

	i = 1;
	while (i < dinner.guest_count + 1)
	{
		thinker = malloc(sizeof(t_guest));
		if (!thinker)
			put_err("Failled to malloc philosopher");
		*thinker = (t_guest){0};
		thinker->id = i;
		if (pthread_mutex_init(&thinker->fork_mutex, NULL) != 0)
			put_err("Failled to initialize philosopher");
		ft_lstadd_back(head, thinker);
		i++;
	}
	thinker->next = *head;
	return (*head);
}

void	launch_philos(t_guest *thinker, t_dinner dinner)
{
	size_t	i;

	if (dinner.guest_count == 1)
	{
		pthread_create(&thinker->thread, NULL, dine_alone, thinker);
		return ;
	}
	i = 0;
	while (i++ < dinner.guest_count)
	{
		thinker->last_meal = gset_dinner(0)->start_time;
		pthread_create(&thinker->thread, NULL, launch_dinner, thinker);
		thinker = thinker->next;
	}
}

void	init_philo(t_guest **head, t_dinner dinner)
{
	size_t			i;
	t_guest	*thinker;

	thinker = create_philos(head, dinner);
	gset_dinner(0)->start_time = time_since_epoch();
	launch_philos(*head, dinner);
	while (i++ < dinner.guest_count)
	{
		pthread_join(thinker->thread, NULL);
		thinker = thinker->next;
	}
	printf("Exited philo init\n");
}

void	cleanup_table(t_dinner	*dinner, t_guest *philos)
{
	size_t			i;
	t_guest	*next_philo;

	i = 0;
	while (i++ < dinner->guest_count)
	{
		next_philo = philos->next;
		pthread_mutex_destroy(&philos->fork_mutex);
		free(philos);
		philos = next_philo;
	}
}

int	main(int argc, char *argv[])
{
	t_dinner		dinner;

	dinner = (t_dinner){0};
	gset_dinner(&dinner);
	if (argc < 5)
		put_err("Insufficient argument count");
	set_table(argc, &argv[1]);
	printf("There is %ld philosophers around the table\n", dinner.guest_count);
	init_philo(&dinner.philos, dinner);
	printf("<<The following message should never appear before any philo log\
>>\nBOTH THREADS HAVE RETURNED\n Main thread unshackled\n");
	pthread_mutex_destroy(&gset_dinner(0)->coordinator);
	cleanup_table(&dinner, dinner.philos);
}
