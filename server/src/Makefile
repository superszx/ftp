ELF:=../bin/server
OBJ:=$(wildcard *.c)
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include -lpthread

.PHONY:clean
clean:
	rm -rf ../bin/server
