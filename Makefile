CFLAGS=-std=c11 -g -static -fno-common
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

zincc: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): zincc.h

test: zincc
	./test.sh

clean:
	rm -f zincc *.o *~ tmp*

.PHONY: test clean
