#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lock1, lock2;

void *resource1(){
    pthread_mutex_lock(&lock1);

    printf("Работа началась в resource1..\n");
    sleep(2);

    printf("Пробовать получить resource2\n");

    pthread_mutex_lock(&lock2); 
    printf("Приобретенный resource2\n");
    pthread_mutex_unlock(&lock2);

    printf("Работа закончена в resource1..\n");

    pthread_mutex_unlock(&lock1);

    pthread_exit(NULL);
}

void *resource2(){
    pthread_mutex_lock(&lock2);

    printf("Работа началась в resource2..\n");
    sleep(2);

    printf("Пробовать получить resource1\n");
    
    pthread_mutex_lock(&lock1); 
    printf("Приобретенный resource1\n");
    pthread_mutex_unlock(&lock1);

    printf("Работа закончена в resource2..\n");

    pthread_mutex_unlock(&lock2);

    pthread_exit(NULL);
}

int main()
{
    pthread_mutex_init(&lock1,NULL);
    pthread_mutex_init(&lock2,NULL);

    pthread_t t1,t2;

    pthread_create(&t1,NULL,resource1,NULL);
    pthread_create(&t2,NULL,resource2,NULL);

    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    return 0;
}