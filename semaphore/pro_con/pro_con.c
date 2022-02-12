#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int storagebox;

void *producerRoutine(void*argumentPointer)
{
    int i, item;
    for(i=0;i<10;i++)
    {
        item =i;
        printf("Producer message: Give item to storagebox: %d\n", item);
        storagebox = i;
    }
    return NULL;
}

void *consumerRoutine(void*argumentPointer)
{
    int i, item;
    for(i=0;i<10;i++)
    {
        item =storagebox;
        printf("Consumer message: Take item from storagebox: %d\n", item);
    }
    return NULL;
}

int main()
{
    pthread_t producerID, consumerID;

    pthread_create(&producerID, NULL, producerRoutine, NULL);
    pthread_create(&consumerID, NULL, consumerRoutine, NULL);

    pthread_join(producerID, NULL);
    pthread_join(consumerID, NULL);

    printf("Finish Producer and Consumer!!\n");
    return 0;
}
