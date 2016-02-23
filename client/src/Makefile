ELF:=../bin/client
OBJ:=$(wildcard *.c)
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include -lpthread

.PHONY:clean
clean:
	rm -rf ../bin/server
