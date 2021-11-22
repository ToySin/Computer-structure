CC = gcc

TARGET = dinero_copy.exe
TEST_TARGET = test.exe

INCDIR = header

SRCS = \
source/cache_free.c \
source/cache_hit.c \
source/cache_init.c \
source/cache_is_hit.c \
source/cache_is_miss.c \
source/cache_miss.c \
source/cache_process.c \
source/cache_rate_print.c \
source/file_read_line.c \
source/file_read.c \
simulator.c
OBJS = $(SRCS:.c=.o)

TEST_SRCS = \
test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

all: $(TARGET)

test: $(TEST_TARGET)

%.o: %.c
	$(CC) -c -o $@ $^ -I $(INCDIR)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) -o $@ $^



clean:
	rm -f $(OBJS) $(TEST_OBJS)

fclean: clean
	rm -f $(TARGET) $(TEST_TARGET)
