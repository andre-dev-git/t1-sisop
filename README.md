# t1-sisop

## Como usar o Makefile

- Compilar alvo específico:
  - `make t1_exec`
  - `make t2_exec`
- Compilar tudo (`t1`, `t2`, `p1`, `p2`): `make all`
- Limpar binários e temporários: `make clean`
- Medir tempo de comando: `make timed CMD="./t2_exec 4"`

## Resumo de `t1.c` (T1)

- Cria `N` threads e divide `TARGET` (1.000.000.000) em fatias.
- Cada thread faz `contador_global++` sem sincronização.
- Resultado com `N>1` pode sair menor que `TARGET` por condição de corrida.

## Resumo de `t2.c` (T2)

- Mantém mesma divisão de trabalho do `t1.c`.
- Protege cada incremento com `pthread_mutex_lock`/`pthread_mutex_unlock`.
- Elimina condição de corrida e mantém `contador_global == 1000000000`.