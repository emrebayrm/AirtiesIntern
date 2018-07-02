all: rx_raw tx_raw

myether:
	gcc myether.c -c 

rx_raw: myether
	gcc rx_raw.c myether.o -o rx_raw

tx_raw: myether
	gcc tx_raw.c myether.o -o tx_raw

clean:
	rm myether.o tx_raw rx_raw
	
