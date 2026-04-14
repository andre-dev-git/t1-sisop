/* Exercício: threads somando 1e9 vezes num global; sem mutex → race (valor < 1e9 se threads > 1). */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TARGET 1000000000ULL
#define MAX_THREADS 256

volatile unsigned long long contador_global;

static void *worker(void *arg)
{
    unsigned long long n = *(unsigned long long *)arg;
    for (unsigned long long i = 0; i < n; i++)
        contador_global++;
    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "uso: %s <num_threads>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 1 || n > MAX_THREADS) {
        fprintf(stderr, "threads entre 1 e %d\n", MAX_THREADS);
        return 1;
    }

    contador_global = 0;

    pthread_t threads[MAX_THREADS];
    unsigned long long fatia[MAX_THREADS];

    unsigned long long base = TARGET / (unsigned long long)n;
    unsigned long long resto = TARGET % (unsigned long long)n;
    for (int i = 0; i < n; i++)
        fatia[i] = base + (i < (int)resto ? 1ULL : 0ULL);

    for (int i = 0; i < n; i++)
        pthread_create(&threads[i], NULL, worker, &fatia[i]);

    for (int i = 0; i < n; i++)
        pthread_join(threads[i], NULL);

    printf("incrementos pedidos: %llu\n", (unsigned long long)TARGET);
    printf("contador global: %llu\n", (unsigned long long)contador_global);
    return 0;
}
