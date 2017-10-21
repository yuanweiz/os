all: processor receiver

processor: processor.c common.h
	gcc -o processor -lpthread -g3 processor.c

receiver: receiver.c common.h
	gcc -o receiver -lpthread -g3 receiver.c
