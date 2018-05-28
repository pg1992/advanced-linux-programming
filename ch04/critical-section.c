#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Use locks to prevent simultaneous access to the arrays. */
int* locks;
/* Start with 10 accounts, which can be changed on the command line. */
long num_accts = 10;
/* The account balances will be allocated later. */
float* account_balances;
/* The transaction structure encodes the origin, destination and
 * amount of each transaction. */
struct transaction {
    int from;
    int to;
    float dollars;
} *trans;

/* The function that executes the transactions and cannot be canceled
 * while doing so. */

int process_transaction(int from_acct, int to_acct, float dollars)
{
    int old_cancel_state;

    /* If there is no funds return with status 1. */
    if (account_balances[from_acct] < dollars)
        return 1;

    /* Loop while other thread is holding locks that the current
     * thread needs. */
    while (locks[from_acct] != 0 && locks[to_acct] != 0)
        printf("waiting locks on %d and %d\n", from_acct, to_acct);
    /* Initialize locks (not atomically, which is bad) */
    locks[from_acct] = locks[to_acct] = 1;
    /* Start a critical section which cannot be canceled by other threads. */
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
    account_balances[from_acct] -= dollars;
    account_balances[to_acct] += dollars;
    pthread_setcancelstate(old_cancel_state, NULL);
    /* Close the critical section and release locks. */
    locks[from_acct] = locks[to_acct] = 0;

    /* Everything went OK so return with status 0. */
    return 0;
}

/* This function is passed to the threads to execute the function
 * that realizes the transactions described above. */

void* transaction_thread(void* arg)
{
    long i = (long) arg;
    long ret_val = 0;

    ret_val = process_transaction(trans[i].from, trans[i].to, trans[i].dollars);

    return (void*) ret_val;
}

/* Initializes all transactions with random origin, destination and amount.
 * Every thread executes this. */

void* init_trans(void* arg)
{
    long i = (long) arg;

    trans[i].from = num_accts * ((double) rand() / RAND_MAX);
    trans[i].to = num_accts * ((double) rand() / RAND_MAX);
    trans[i].dollars = 100.0 * rand() / RAND_MAX;

    return NULL;
}

/* Initialize every field in ACCOUNT_BALANCES and LOCKS.
 * Every field is initialized by a single thread concurrently. */

void* init_acct(void* arg)
{
    long i = (long) arg;

    account_balances[i] = 1000 * ((float) rand() / RAND_MAX);
    locks[i] = 0;

    return NULL;
}

/* Print usage. */

void print_usage()
{
    printf("Usage: critical-section [-n NUM | -h]\n"
           "  -n NUM  Use NUM accounts, transactions and threads.\n"
           "  -h      Print this message.\n");
}

/* Main function. */

int main(int argc, char** argv)
{
    pthread_t* tid;
    long* ret_val;
    int opt;
    long i;

    /* Get a random seed from the clock time plus seconds since the Epoch. */
    long now = (long) clock() + (long) time(NULL);
    srand(now);

    /* Get command line arguments. */
    while ((opt = getopt(argc, argv, "hn:")) != -1) {
        switch(opt) {
            case 'h':
                /* '-h' prints the usage. */
                print_usage();
                exit(0);
                break;
            case 'n':
                /* '-n' sets the number of accounts and transactions. */
                num_accts = strtoul(optarg, NULL, 0);
                break;
            default:
                /* Error: print usage and bail with non-zero return code. */
                print_usage();
                exit(1);
                break;
        }
    }

    /* Dynamic allocation of every array we will use. */
    account_balances = (float*)malloc(num_accts * sizeof(float));
    locks = (int*)malloc(num_accts * sizeof(int));
    tid = (pthread_t*)malloc(num_accts * sizeof(pthread_t));
    ret_val = (long*)malloc(num_accts * sizeof(long));
    trans = (struct transaction*)malloc(num_accts * sizeof(struct transaction));

    /* Start one thread per account to initialize LOCKS and each field in ACCOUNT_BALANCES. */
    for (i = 0; i < num_accts; i++)
        pthread_create(&tid[i], NULL, &init_acct, (void*) i);

    for (i = 0; i < num_accts; i++)
        pthread_join(tid[i], NULL);

    /* Print every account balance. */
    for (i = 0; i < num_accts; i++)
        printf("account[%d] = %8.2f\n", i, account_balances[i]);

    /* Start NUM_ACCTS threads to initialize all transactions. */
    for (i = 0; i < num_accts; i++)
        pthread_create(&tid[i], NULL, &init_trans, (void*) i);

    for (i = 0; i < num_accts; i++)
        pthread_join(tid[i], NULL);

    /* Print every transaction that will be performed. */
    for (i = 0; i < num_accts; i++)
        printf("from: %8d, to: %8d, dollars: %8.2f\n", trans[i].from, trans[i].to, trans[i].dollars);

    /* Perform every transaction. */
    for (i = 0; i < num_accts; i++)
        pthread_create(&tid[i], NULL, &transaction_thread, (void*) i);

    for (i = 0; i < num_accts; i++)
        pthread_join(tid[i], (void*) &ret_val[i]);

    /* Show the final accounts balances. */
    for (i = 0; i < num_accts; i++)
        printf("final: account[%d] = %8.2f\n", i, account_balances[i]);

    /*  */
    for (i = 0; i < num_accts; i++)
        printf("from: %8d, to: %8d, dollars: %8.2f (%s)\n", trans[i].from, trans[i].to, trans[i].dollars, ret_val[i] ? "fail" : "success");

    /* Free every dynamically allocated array and exit. */
    free(locks);
    free(trans);
    free(ret_val);
    free(tid);
    free(account_balances);
    return 0;
}
