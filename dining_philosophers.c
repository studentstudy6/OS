#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define N     5      /* number of philosophers / forks */
#define MEALS 3      /* each philosopher eats this many times, then stops */
sem_t forks[N];
sem_t room;          /* allows at most N-1 philosophers to compete for forks */
void *philosopher(void *num)
{
    int id = *(int *)num;
    for (int meal = 1; meal <= MEALS; meal++) {
        printf("Philosopher %d is Thinking\n", id);
        sleep(1);
#ifndef UNSAFE
        sem_wait(&room);                 /* deadlock-free version only */
#endif
        sem_wait(&forks[id]);
        printf("Philosopher %d picked LEFT fork %d\n", id, id);
#ifdef UNSAFE
        usleep(200000);                  /* widen the race window -> deadlock */
#endif
        sem_wait(&forks[(id + 1) % N]);
        printf("Philosopher %d picked RIGHT fork %d\n", id, (id + 1) % N);
        printf("Philosopher %d is Eating (meal %d)\n", id, meal);
        sleep(2);
        sem_post(&forks[id]);
        sem_post(&forks[(id + 1) % N]);
        printf("Philosopher %d released forks\n", id);
#ifndef UNSAFE
Operating Systems Lab - Complete Practical Guide (Programs, Commands, Viva) Page 13
        sem_post(&room);
#endif
        sleep(1);
    }
    printf("Philosopher %d is DONE\n", id);
    pthread_exit(NULL);
}
int main()
{
    pthread_t phil[N];
    int id[N];
    int i;
    sem_init(&room, 0, N - 1);
    for (i = 0; i < N; i++)
        sem_init(&forks[i], 0, 1);
    for (i = 0; i < N; i++) {
        id[i] = i;
        pthread_create(&phil[i], NULL, philosopher, &id[i]);
    }
    for (i = 0; i < N; i++)
        pthread_join(phil[i], NULL);
    for (i = 0; i < N; i++)
        sem_destroy(&forks[i]);
    sem_destroy(&room);
    printf("\nAll philosophers finished - no deadlock.\n");
    return 0;
}
