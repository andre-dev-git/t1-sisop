#!/bin/sh
set -e
cd "$(dirname "$0")"
make clean
make
# 1 thread: termina em exatamente TARGET; várias: costuma passar do alvo (corrida no while)
v1=$(./contador_race 1 | sed -n 's/^contador global: //p')
v4=$(./contador_race 4 | sed -n 's/^contador global: //p')
test "$v1" -eq 1000000000
test "$v4" -ge 1000000000
echo ok
