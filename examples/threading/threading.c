#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    struct thread_data *args = (struct thread_data *)thread_param;
    struct timespec ts;
    int rc;

    // Sleep before attempting to obtain the mutex
    ts.tv_sec = args->wait_obtain / 1000;
    ts.tv_nsec = (args->wait_obtain % 1000) * 1000000;
    rc = nanosleep(&ts, NULL);
    if (rc != 0) {
        printf("Failed to sleep before obtaining mutex: %d\n", rc);
        args->thread_complete_success = false;
        return thread_param;
    }
    
    //obtain mutex
    rc = pthread_mutex_lock(args->mutex);
    if (rc != 0) {
        printf("Failed to lock mutex: %d\n", rc);
        args->thread_complete_success = false;
        return thread_param;
    }

    //sleep before attempting to release mutex
    ts.tv_sec = args->wait_release / 1000;
    ts.tv_nsec = (args->wait_release % 1000) * 1000000;
    rc = nanosleep(&ts, NULL);
    if (rc != 0) {
        printf("Failed to sleep before releasing mutex: %d\n", rc);
        args->thread_complete_success = false;
        pthread_mutex_unlock(args->mutex); // Ensure mutex is released before exiting
        return thread_param;
    }

    //release mutex
    rc = pthread_mutex_unlock(args->mutex);
    if (rc != 0) {
        printf("Failed to unlock mutex: %d\n", rc);
        args->thread_complete_success = false;
        return thread_param;
    }

    //set thread complete success flag
    printf("Thread completed successfully\n");
    args->thread_complete_success = true;

    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_param, setup mutex and wait arguments, pass thread_param to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    int rc;
    struct thread_data *thread_param = malloc(sizeof(struct thread_data));
    if (thread_param == NULL) {
        printf("Failed to allocate memory for thread arguments");
        return false;
    }

    /* pass arguments to thread param */
    thread_param->mutex = mutex;
    thread_param->wait_obtain = wait_to_obtain_ms;
    thread_param->wait_release = wait_to_release_ms;

    rc = pthread_create(thread, NULL, threadfunc, (void*)thread_param);
    if (rc != 0) {
        printf("Failed to create thread: %d", rc);
        free(thread_param); // Free allocated memory on failure
        return false;
    }

    printf("thread created succesfully: %d\n", rc);
    return true;
}

