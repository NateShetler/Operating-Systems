// Nathaniel Shetler
// Operating Systems
// 15 October 2020

#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>


#define TRUE 1
#define False 0

sem_t agentSem, tobacco, paper, matches;

Boolean isTobacco = false, isPaper = false, isMatch = false;

sem_t tobaccoSem, paperSem, matchSem, mutex;

void *pusherTobacco();
void *pusherPaper();
void *pusherMatches();

void *agent1();
void *agent2();
void *agent3();

void *smokerTobacco();
void *smokerPaper();
void *smokerMatches();

int main()
{

    // The threads for lab 2
    pthread_t agent1Thread, agent2Thread, agent3Thread;
    pthread_t pusher1, pusher2, pusher3;
    pthread_t smokerTobacco1, smokerTobacco2, smokerPaper1, smokerPaper2, smokerMatches1, smokerMatches2;

    // Initialize the semaphores

    sem_init(&agentSem,0,1);
    sem_init(&tobacco,0,0);
    sem_init(&paper,0,0);
    sem_init(&matches,0,0);

    sem_init(&tobaccoSem,0,0);
    sem_init(&paperSem,0,0);
    sem_init(&matchSem,0,0);
    
    sem_init(&mutex,0,1); // mutex = 1

    // Create the threads

    pthread_create(&agent1Thread, NULL, agent1, NULL);
    pthread_create(&agent2Thread, NULL, agent2, NULL);
    pthread_create(&agent3Thread, NULL, agent3, NULL);

    pthread_create(&pusher1, NULL, pusherTobacco, NULL);
    pthread_create(&pusher2, NULL, pusherPaper, NULL);
    pthread_create(&pusher3, NULL, pusherMatches, NULL);

    pthread_create(&smokerTobacco1, NULL, smokerTobacco, NULL);
    pthread_create(&smokerTobacco2, NULL, smokerTobacco, NULL);
    pthread_create(&smokerPaper1, NULL, smokerPaper, NULL);
    pthread_create(&smokerPaper2, NULL, smokerPaper, NULL);
    pthread_create(&smokerMatches1, NULL, smokerMatches, NULL);
    pthread_create(&smokerMatches2, NULL, smokerMatches, NULL);

    // Join the threads

    pthread_join(agent1Thread, NULL);
    pthread_join(agent2Thread, NULL);
    pthread_join(agent3Thread, NULL);

    pthread_join(pusher1, NULL);
    pthread_join(pusher2, NULL);
    pthread_join(pusher3, NULL);

    pthread_join(smokerTobacco1, NULL);
    pthread_join(smokerTobacco2, NULL);
    pthread_join(smokerPaper1, NULL);
    pthread_join(smokerPaper2, NULL);
    pthread_join(smokerMatches1, NULL);
    pthread_join(smokerMatches2, NULL);

    return 0;
}


void *pusherTobacco()
{
    while (TRUE)
    {
        while (sem_wait(&tobacco) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isPaper == TRUE)
        {
            isPaper = FALSE;
            sem_post(&matchSem);
        }
        else if (isMatch == TRUE)
        {
            isMatch = FALSE;
            sem_post(&paperSem);
        }
        else
        {
            isTobacco = TRUE;
        }

        sem_post(&mutex);
    }
}

void *pusherPaper()
{
    while (TRUE)
    {
        while (sem_wait(&paper) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isTobacco == TRUE)
        {
            isTobacco = FALSE;
            sem_post(&matchSem);
        }
        else if (isMatch == TRUE)
        {
            isMatch = FALSE;
            sem_post(&tobaccoSem);
        }
        else
        {
            isPaper = TRUE;
        }
        
        sem_post(&mutex);
    }
}

void *pusherMatches()
{
    while (TRUE)
    {
        while (sem_wait(&matches) != 0) {}
        while (sem_wait(&mutex) != 0) {}
        if (isPaper == TRUE)
        {
            isPaper = FALSE;
            sem_post(&isTobacco);
        }
        else if (isTobacco == TRUE)
        {
            isTobacco = FALSE;
            sem_post(&isPaper);
        }
        else
        {
            isMatch = TRUE;
        }

        sem_post(&mutex);
        
    }
}

void *agent1()
{
    while (sem_wait(&agentSem) != 0) {}
    sem_post(&tobacco);
    sem_post(&paper);
}

void *agent2()
{
    while (sem_wait(&agentSem) != 0) {}
    sem_post(&tobacco);
    sem_post(&matches);
}

void *agent3()
{
    while (sem_wait(&agentSem) != 0) {}
    sem_post(&matches);
    sem_post(&paper);
}

void *smokerTobacco()
{
    while (sem_wait(&tobaccoSem) != 0) {}

    {
        // make cigarette
    }

    sem_post(&agentSem);

    {
        // smoke the cigarette
    }
}

void *smokerPaper()
{
    while (sem_wait(&paperSem) != 0) {}

    {
        // make cigarette
    }

    sem_post(&agentSem);

    {
        // smoke the cigarette
    }
}

void *smokerMatches()
{
    while (sem_wait(&matchSem) != 0) {}

    {
        // make cigarette
    }

    sem_post(&agentSem);

    {
        // smoke the cigarette
    }
}