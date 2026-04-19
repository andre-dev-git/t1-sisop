/* P2: mesmo alvo que p1.c, com semáforo nomeado em cada incremento → contador == 1e9. */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>

#define TARGET 1000000000ULL
#define MAX_PROCS 256
#define SEM_NAME "/sem_contador_p2"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "uso: %s <num_processos>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n < 1 || n > MAX_PROCS) {
        fprintf(stderr, "processos entre 1 e %d\n", MAX_PROCS);
        return 1;
    }

    // Alocando Memória Compartilhada
    int shmid = shmget(IPC_PRIVATE, sizeof(unsigned long long), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        return 1;
    }

    unsigned long long *contador_global = (unsigned long long *) shmat(shmid, NULL, 0);
    if (contador_global == (unsigned long long *) -1) {
        perror("shmat");
        return 1;
    }
    *contador_global = 0;

    // Inicializando Semáforo (nomeado para processos distintos)
    sem_unlink(SEM_NAME); // Garante que não há resíduo de execuções anteriores
    sem_t *sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

    unsigned long long fatia[MAX_PROCS];
    unsigned long long base = TARGET / (unsigned long long)n;
    unsigned long long resto = TARGET % (unsigned long long)n;
    for (int i = 0; i < n; i++)
        fatia[i] = base + (i < (int)resto ? 1ULL : 0ULL);

    pid_t pids[MAX_PROCS];

    for (int i = 0; i < n; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return 1;
        } else if (pids[i] == 0) {
            // Processo Filho
            unsigned long long n_iter = fatia[i];
            for (unsigned long long j = 0; j < n_iter; j++) {
                sem_wait(sem);
                (*contador_global)++;
                sem_post(sem);
            }
            shmdt(contador_global);
            sem_close(sem);
            exit(0);
        }
    }

    // Processo Pai aguarda o término dos filhos
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    printf("incrementos pedidos: %llu\n", (unsigned long long)TARGET);
    printf("contador global: %llu\n", (unsigned long long)*contador_global);

    // Limpeza dos Recursos
    sem_close(sem);
    sem_unlink(SEM_NAME);
    shmdt(contador_global);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}