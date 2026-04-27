CC = gcc
CFLAGS = -Wall -O2 -pthread

SRCS = t1.c t2.c p1.c p2.c
TARGETS = $(SRCS:.c=_exec)

all: $(TARGETS)

%_exec: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *_exec *.o *~

timed:
	@test -n "$(strip $(CMD))" || (echo 'uso: make timed CMD="comando"'; exit 1)
	@time -p $(CMD)

.PHONY: all clean timed