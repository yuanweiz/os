all: processor receiver

processor: processor.c common.h
	gcc processor.c -lpthread  -o processor 

receiver: receiver.c common.h
	gcc receiver.c -lpthread -o receiver 
