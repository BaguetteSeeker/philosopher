/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:47:23 by epinaud           #+#    #+#             */
/*   Updated: 2025/06/22 00:50:37 by epinaud          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# define _DEFAULT_SOURCE
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

# define MAX_ARGS 6

enum e_philo_state
{
	EATING,
	SLEEPING,
	THINKING,
	PICKING_FORK,
	DIED
};

typedef struct s_cycle_counter
{
	size_t			life;
	size_t			meal;
	size_t			sleep;
}	t_cycle_counter;

typedef struct s_philosopher
{
	void					*ptr;
	struct s_philosopher	*next;
	size_t					id;
	pthread_t				thread;
	pthread_mutex_t			fork_mutex;
	size_t					last_meal;
	size_t					times_eaten;
}	t_guest;

typedef struct s_dinner
{
	bool			is_done;
	pthread_mutex_t	coordinator;
	size_t			start_time;
	size_t			guest_count;
	size_t			life_duration;
	size_t			meal_duration;
	size_t			sleep_duration;
	size_t			meals_required;
	int				argc;
	t_guest	*philos;
}	t_dinner;

void		lst_put(t_guest *lst);
void		put_err(char *msg);
void		display_state(t_guest *philo, size_t action);
t_dinner	*gset_dinner(void *g);
void		*print_thread(void *data);
size_t		time_since_start(void);
size_t		time_since_epoch(void);
size_t		eat(t_guest *guest);
void		*launch_dinner(void *v_philo);
void		*dine_alone(void *philo);

//Utils
void		ft_putstr_fd(char *s, int fd);
int			ft_isdigit(int c);
int			ft_atoi(const char *nptr);
t_guest		*ft_lstlast(t_guest *lst);
void		ft_lstadd_back(t_guest **lst, t_guest *new);
char		*ft_strchr(const char *s, int c);
#endif
