CC = gcc

TARGET = dinero_copy.exe
TEST_TARGET = test.exe

INCDIR = header

SRCS = \
source/cache_free.c \
source/cache_hit.c \
source/cache_init.c \
source/cache_match_block_index.c \
source/cache_miss.c \
source/cache_process.c \
source/cache_rate_print.c \
source/cache_refresh_block.c \
source/file_read_line.c \
source/file_read.c \
simulator.c
OBJS = $(SRCS:.c=.o)


all: $(TARGET)

onefile:
	$(CC) -o cache_simulator_challenge.exe cache_simulator_challenge.c

%.o: %.c
	$(CC) -c -o $@ $^ -I $(INCDIR)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^


clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(TARGET) cache_simulator_challenge.exe
