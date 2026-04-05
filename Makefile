CFLAGS = -Wall -Wextra

all: bloat.c.o bloatmath.c.o test

test: test.c
	$(CC) $(CFLAGS) test.c -o test

bloat.c.o: bloat.h
	$(CC) $(CFLAGS) -DBLOAT_IMPLEMENTATION -x c -o bloat.c.o -c bloat.h

bloatmath.c.o: bloatmath.h
	$(CC) $(CFLAGS) -DBLOAT_MATH_IMPLEMENTATION -x c -o bloatmath.c.o -c bloatmath.h
