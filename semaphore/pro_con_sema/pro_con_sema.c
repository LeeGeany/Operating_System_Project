#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>

sem_t produce, consume;
int storagebox;

void *producerRoutine(void*argumentPointer)
{
    int i, item;
    for(i=0;i<10;i++)
    {
        item =i;
        sem_wait(&produce);

        printf("Producer message: Give item to storagebox: %d\n", item);
        storagebox = i;

        sem_post(&consume);
    }
    return NULL;
}

void *consumerRoutine(void*argumentPointer)
{
    int i, item;
    for(i=0;i<10;i++)
    {
        sem_wait(&consume);
        item =storagebox;
        printf("Consumer message: Take item from storagebox: %d\n", item);
        sem_post(&produce);
    }
    return NULL;
}

int main()
{
    pthread_t producerID, consumerID;

    sem_init(&produce, 0, 1);
    sem_init(&consume, 0, 0);

    pthread_create(&producerID, NULL, producerRoutine, NULL);
    pthread_create(&consumerID, NULL, consumerRoutine, NULL);

    pthread_join(producerID, NULL);
    pthread_join(consumerID, NULL);

    printf("Finish Producer and Consumer!!\n");
    return 0;
}
