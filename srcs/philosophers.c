
#include "philosophers.h"

t_dinner	*gset_dinner(void *g)
{
	static t_dinner	*stored_g = {NULL};

	if (!stored_g)
		stored_g = g;
	return (stored_g);
}

int	set_table(char *args[])
{
	size_t	val;
	size_t	i;

	i = 0;
	while (args[i])
	{
		val = ft_atoi(args[i]);
		if (ft_strlen(args[i]) != ft_nbrlen(val))
			put_err("");
		if (i == 0)
			gset_dinner(0)->guest_count = val;
		else if (i == 1)
			gset_dinner(0)->life_duration = val;
		else if (i == 2)
			gset_dinner(0)->meal_duration = val / 0.001;
		else if (i == 3)
			gset_dinner(0)->sleep_duration = val / 0.001;
		else if	(i == 4)
			gset_dinner(0)->meals_required = val;
		i++;
	}
	return (1);
}

void	init_philo(t_philosopher **head, t_dinner dinner)
{
	size_t			i;
	t_philosopher	*thinker;

	i = 1;
	while (i < dinner.guest_count + 1)
	{
		thinker = malloc(sizeof(t_philosopher));
		if (!thinker)
			put_err("Failled to malloc philosopher");
		*thinker = (t_philosopher){0};
		thinker->number = i;
		if (pthread_mutex_init(&thinker->fork_mutex, NULL) != 0)
			put_err("Failled to initialize philosopher");
		ft_lstadd_back((t_list **)head, (t_list *)thinker);
		i++;
	}
	thinker->next = *head;
	thinker = *head;
	i = 0;
	gset_dinner(0)->start_time = time_since_epoch();
	printf("Dinner start at %ld\n", gset_dinner(0)->start_time);
	while (i++ < dinner.guest_count)
	{
		thinker->last_meal = gset_dinner(0)->start_time;
		pthread_create(&thinker->thread, NULL, launch_routine, thinker);
		thinker = thinker->next;
	}
	i = 0;
	while (i++ < dinner.guest_count)
	{
		pthread_join(thinker->thread, NULL);
		thinker = thinker->next;
	}
	printf("Exited philo init\n");
}

// if (gset_dinner(0)->table.life_duration == 0)
// 	return (death_zero(&dinner), free_dinner(), 0);
// if (gset_dinner(0)->guest_count == 1)
// 	return (philo_one(&dinner), free_dinner(), 0);
// else
// 	coordinate_dinner(&dinner);
int	main(int argc, char *argv[])
{
	t_dinner		dinner;
	// pthread_t		thread1;
	// pthread_t		thread2;

	dinner = (t_dinner){0};
	gset_dinner(&dinner);
	if (argc < 5)
		put_err("Insufficient argument count");
	set_table(&argv[1]);

	t_philosopher	*thinker;

	thinker = NULL;
	printf("There is %ld philosophers around the table\n", dinner.guest_count);	
	init_philo(&thinker, dinner);
	// ft_lstiter((t_list *)thinker, (void (*)(LL_TYP *))lst_put);
	// pthread_create(&thread2, NULL, print_thread, thinker->next);
	printf("<<The following message should never appear before any philo log\
>>\nBOTH THREADS HAVE RETURNED\n Main thread unshackled\ns");
    // // Destroy mutexes
    // for (i = 0; i < dinner.guest_count; i++)

	//number_of_philosophers 
	//time_to_die
	// time_to_eat
	// time_to_sleep
	//[number_of_times_each_philosopher_must_eat]

}
