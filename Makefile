CFLAGS	  = -Wall -Wextra
BUILD_DIR = build

all: bloat.c.o bloatmath.c.o test

test: test.c bloat.h $(BUILD_DIR)
	$(CC) $(CFLAGS) test.c -o $(BUILD_DIR)/test
	./build/test

bloat.c.o: bloat.h $(BUILD_DIR)
	$(CC) $(CFLAGS) -DBLOAT_IMPLEMENTATION -x c -o $(BUILD_DIR)/bloat.c.o -c bloat.h

bloatmath.c.o: bloatmath.h $(BUILD_DIR)
	$(CC) $(CFLAGS) -DBLOAT_MATH_IMPLEMENTATION -x c -o $(BUILD_DIR)/bloatmath.c.o -c bloatmath.h

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
