CC = gcc

TARGET = test.out

SRCS = Basic.c

all: $(TARGET)

basic:
	$(CC) -o basic.out Basic.c

adv:
	$(CC) -o advanced.out Advanced.c

$(TARGET): $(SRCS)
	$(CC) -o $@ $^
