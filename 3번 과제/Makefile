TARGET	= pa3
CFLAGS  = -g
#CFLAGS += -D_USE_DEFAULT

all: pa3

pa3: pa3.c
	gcc $(CFLAGS) $^ -o $@

.PHONY: test-lw1
test-basic: pa3 testcases/lw-1
	./pa2 < testcases/lw-1 2>&1 >/dev/null

.PHONY: clean
clean:
	rm -rf *.o pa3
