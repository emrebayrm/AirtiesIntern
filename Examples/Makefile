CC=gcc

RX_RAW_SRC = myether.c \
			 rx_raw.c
RX_RAW_TARGET = rx_raw

RX_POLL_SRC = myether.c \
			  rx_poll.c
RX_POLL_TARGET = rx_poll

TX_RAW_SRC = myether.c \
			 tx_raw.c
TX_RAW_TARGET = tx_raw

default: all

all: rx_raw tx_raw rx_poll

rx_raw:
	$(CC) $(RX_RAW_SRC) -o $(RX_RAW_TARGET)

rx_poll:
	$(CC) $(RX_POLL_SRC) -o $(RX_POLL_TARGET)

tx_raw:
	$(CC) $(TX_RAW_SRC) -o $(TX_RAW_TARGET)

clean:
	rm -rf newaa newab $(RX_RAW_TARGET) $(RX_POLL_TARGET) $(TX_RAW_TARGET) $(RX_FILE_TARGET) $(TX_FILE_TARGET) rx_bins/*
