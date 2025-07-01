/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: epinaud <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 21:47:23 by epinaud           #+#    #+#             */
/*   Updated: 2025/07/02 18:10:07 by epinaud          ###   ########.fr       */
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

typedef struct s_guest
{
	void					*ptr;
	struct s_guest			*next;
	size_t					id;
	pthread_t				thread;
	pthread_mutex_t			fork_mutex;
	size_t					last_meal;
	size_t					times_eaten;
}	t_guest;

typedef struct s_dinner
{
	bool			is_done;
	pthread_mutex_t	display_lock;
	pthread_mutex_t	coordinator;
	size_t			start_time;
	size_t			guest_count;
	size_t			life_duration;
	size_t			meal_duration;
	size_t			sleep_duration;
	size_t			meals_required;
	int				argc;
	t_guest			*philos;
}	t_dinner;

void		launch_dinner(t_guest *thinker, t_dinner dinner);
void		*dine_alone(void *philo);
void		*launch_routine(void *v_philo);
void		set_table(int argc, char *args[]);
void		cleanup_table(t_dinner	*dinner, t_guest *philos);
void		display_state(t_guest *philo, size_t action);
// size_t		eat(t_guest *guest);
t_dinner	*gset_dinner(void *g);
size_t		is_dinner_done(void);
size_t		time_since_start(void);
size_t		time_since_epoch(void);
void		ft_usleep(size_t milliseconds, t_guest *philo, size_t action);
void		check_death(t_guest *philo);

//Utils
void		put_err(char *msg);
void		lst_put(t_guest *lst);
void		ft_putstr_fd(char *s, int fd);
int			ft_isdigit(int c);
int			ft_atoi(const char *nptr);
t_guest		*ft_lstlast(t_guest *lst);
void		ft_lstadd_back(t_guest **lst, t_guest *new);
char		*ft_strchr(const char *s, int c);
#endif
