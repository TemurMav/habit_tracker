CC = gcc
CFLAGS = -Wall
LIBS = -lncurses
INCLUDES = -Iinсlude/ 

SRCDIR = src
BINDIR = bin

SRCS = $(wildcard $(SRCDIR)/*.c)
TARGET = $(BINDIR)/1

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS) $(INCLUDES)

clean:
	rm -f $(TARGET)

