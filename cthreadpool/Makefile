DIR=./
cqueue.o: cqueue.c cqueue.h
	gcc -c	cqueue.c 
test_cqueue.o: test_cqueue.c cqueue.h
	gcc -c	test_cqueue.c 
test_cq: init cqueue.o test_cqueue.o
	gcc -o output/test_cqueue cqueue.o test_cqueue.o
	
cthread_pool.o: cthread_pool.c cthread_pool.h cqueue.h
	gcc -c cthread_pool.c

test_ctp.o: test_ctp.c cthread_pool.h
	gcc -c test_ctp.c

test_ctp: init cthread_pool.o test_ctp.o cqueue.o
	gcc -o output/test_ctp cthread_pool.o test_ctp.o cqueue.o

test: test_cq test_ctp

.PHONY: clean
clean: 
	rm *.o 

.PHONY: init
init: 
	mkdir -p output
