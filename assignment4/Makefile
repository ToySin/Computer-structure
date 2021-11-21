CC = gcc

TARGET = dinero_copy
TEST_TARGET = test

SRCS = \
convert_read_trace.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = \
test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)


all: $(TARGET)

test: $(TEST_TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c -o $@ $^

clean:
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(TARGET) $(TEST_TARGET)
