CC = gcc

TARGET = test.out

SRCS = Basic.c

all: $(TARGET)

basic:
	$(CC) -o basic.out Basic.c -g

adv:
	$(CC) -o advanced.out Advanced.c -g

ch:
	$(CC) -o challenge.out Challenge.c -g

non:
	$(CC) -o non.out Non_hazard_Challenge.c -g

$(TARGET): $(SRCS)
	$(CC) -o $@ $^

clean:
	rm -f *.out
