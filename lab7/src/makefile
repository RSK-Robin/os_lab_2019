CC=gcc
CFLAGS=-I.

all : tcp udp

tcp: tcpclient.c tcpserver.c
	$(CC) -o tcpclient tcpclient.c $(CFLAGS)
	$(CC) -o tcpserver tcpserver.c $(CFLAGS)

udp: udpclient.c udpserver.c
	$(CC) -o udpclient udpclient.c $(CFLAGS)
	$(CC) -o udpserver udpserver.c $(CFLAGS)

clean:
	rm tcpclient tcpserver udpclient udpserver