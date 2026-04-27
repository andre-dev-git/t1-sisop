#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

PROGRAMS=(t1 t2 p1 p2)
THREADS=(2 4 8)

if [[ "${1:-}" == "--build" ]]; then
  make all
fi

for program in "${PROGRAMS[@]}"; do
  bin="./${program}_exec"
  if [[ ! -x "$bin" ]]; then
    echo "binario nao encontrado: $bin"
    echo "dica: rode './run_all.sh --build' ou 'make all'"
    exit 1
  fi

  for n in "${THREADS[@]}"; do
    echo "==== ${program^^} N=$n ===="
    time -p "$bin" "$n"
    echo
  done
done
