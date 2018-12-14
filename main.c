#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

typedef struct buffer
{
    int tab[10]; //buffer of elements
    int count;
    sem_t mutex;
    sem_t empty;
    sem_t full;
}buffer;

buffer *(array_of_buffers[5]);
sem_t mutex2;

void insert_item(int to_insert, buffer *buff)
{
  int i = 0;
  int j = 0;
  if(buff->count!=0)
  {
    for (i = buff->count; i > 0; --i)
      buff->tab[i] = buff->tab[i - 1];
  }
  buff->tab[0]=to_insert;
  ++buff->count;
};

void erase_item(buffer *buff)
{
  int i=0;
  --buff->count;
}

void print1(int number, buffer *buff)
{
  int i=0;
  printf("\n\nKolejka %d: dodano element %d\n", number, buff->tab[0]);
  printf("Stan kolejki: ");
  for(i=0; i<buff->count; i++)
    printf("%d ", buff->tab[i]);
}

void print2(int number, buffer *buff)
{
  int i=0;
  printf("\n\nKolejka %d: usunieto element %d\n", number, buff->tab[buff->count]);
  printf("Stan kolejki: ");
  for(i=0; i<buff->count; i++)
    printf("%d ", buff->tab[i]);
}

void producer()
{
  int number, new_element;

  number=rand()%5;
  new_element=rand()%100;

  sem_wait(&array_of_buffers[number]->empty);
  sem_wait(&array_of_buffers[number]->mutex);
  insert_item(new_element, array_of_buffers[number]);
  sem_post(&array_of_buffers[number]->mutex);
  sem_wait(&mutex2);
  print1(number+1, array_of_buffers[number]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[number]->full);
}

void consumer1()
{
  sem_wait(&array_of_buffers[0]->full);
  sem_wait(&array_of_buffers[0]->mutex);
  erase_item(array_of_buffers[0]);
  sem_post(&array_of_buffers[0]->mutex);
  sem_wait(&mutex2);
  print2(1, array_of_buffers[0]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[0]->empty);
}

void consumer2()
{
  sem_wait(&array_of_buffers[1]->full);
  sem_wait(&array_of_buffers[1]->mutex);
  erase_item(array_of_buffers[1]);
  sem_post(&array_of_buffers[1]->mutex);
  sem_wait(&mutex2);
  print2(2, array_of_buffers[1]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[1]->empty);
}

void consumer3()
{
  sem_wait(&array_of_buffers[2]->full);
  sem_wait(&array_of_buffers[2]->mutex);
  erase_item(array_of_buffers[2]);
  sem_post(&array_of_buffers[2]->mutex);
  sem_wait(&mutex2);
  print2(3, array_of_buffers[2]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[2]->empty);
}

void consumer4()
{
  sem_wait(&array_of_buffers[3]->full);
  sem_wait(&array_of_buffers[3]->mutex);
  erase_item(array_of_buffers[3]);
  sem_post(&array_of_buffers[3]->mutex);
  sem_wait(&mutex2);
  print2(4, array_of_buffers[3]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[3]->empty);
}

void consumer5()
{
  sem_wait(&array_of_buffers[4]->full);
  sem_wait(&array_of_buffers[4]->mutex);
  erase_item(array_of_buffers[4]);
  sem_post(&array_of_buffers[4]->mutex);
  sem_wait(&mutex2);
  print2(5, array_of_buffers[4]);
  sem_post(&mutex2);
  sem_post(&array_of_buffers[4]->empty);
}

void perform_test1()
{
  while(1)
  {
    pthread_t new_thread;
    int number=rand()%6;
    usleep(20000);

    if(number==0) pthread_create(&new_thread, NULL, (void *) producer, NULL);
    else if (number == 1) pthread_create(&new_thread, NULL, (void *) consumer1, NULL);
    else if (number == 2) pthread_create(&new_thread, NULL, (void *) consumer2, NULL);
    else if (number == 3) pthread_create(&new_thread, NULL, (void *) consumer3, NULL);
    else if (number == 4) pthread_create(&new_thread, NULL, (void *) consumer4, NULL);
    else if (number == 5) pthread_create(&new_thread, NULL, (void *) consumer5, NULL);
    else break;
  }
}

int main()
{
  unsigned seed;
  time_t t;
  int i, j;

  for(i=0; i<5; i++)
    array_of_buffers[i]=(buffer *)malloc(sizeof(buffer));

  seed=time(&t);
  srand(seed);

  for(j=0; j<5; j++)
  {
    sem_init(&array_of_buffers[j]->mutex, 0, 1);
    sem_init(&array_of_buffers[j]->empty, 0, 10);
    sem_init(&array_of_buffers[j]->full, 0, 0);
  }

  sem_init(&mutex2, 0, 1);

  perform_test1();

  for(i=0; i<5; i++)
    free(array_of_buffers[i]);

}