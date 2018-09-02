CC 		:= x86_64-linux-gnu-gcc
TARGET = userapp
SRC = user.c
CFLAGS = -static

default: all

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -rf $(TARGET)
