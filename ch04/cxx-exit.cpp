#include <pthread.h>
#include <unistd.h>
#include <cstdio>

class ThreadExitException
{
    public:
        /* Create an exception-signaling thread exit with RETURN_VALUE. */
        ThreadExitException(void* return_value)
            : thread_return_value_ (return_value)
        {
        }

        /* Actually exit the thread, using the return value provided in the
         * constructor. */
        void* DoThreadExit()
        {
            printf("Thread exited.\n");
            pthread_exit(thread_return_value_);
        }

    private:
        /* The return value that will be used when exiting the thread. */
        void* thread_return_value_;
};

static bool exit_thread = false;

bool should_exit_thread_immediately(void)
{
    return exit_thread;
}

void do_some_work()
{
    while (1) {
        /* Do some useful things here... */
        printf("hey...\n");
        usleep(50000);
        if (should_exit_thread_immediately())
            throw ThreadExitException(/* thread's return value = */ NULL);
    }
}

void* thread_function(void*)
{
    try {
        do_some_work();
    }
    catch (ThreadExitException ex) {
        /* Some function indicated that we should exit the thread. */
        printf("Exception thrown.\n");
        ex.DoThreadExit();
    }
    return NULL;
}

int main(void)
{
    pthread_t tid;
    pthread_create(&tid, NULL, thread_function, NULL);
    for (int i = 0; i < 10; i++) {
        printf("hello...\n");
        usleep(50000);
    }
    exit_thread = true;
    pthread_join(tid, NULL);
}
