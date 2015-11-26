CC = gcc
CFLAGS =

APPS = lolo lolo2 syslolo

all: $(APPS)

lolo: lolo.c
	@echo -e "	Building the lolo trace application"
	@gcc -o lolo $<

lolo2: lolo2.cc
	@echo -e "	Building the lolo2 kiwi wrapper application"
	@g++ -o lolo2 $<
	
syslolo: syslolo.cc
	@echo -e "	Building the syslolo trace application"
	@g++ -o syslolo $<

clean:
	@-rm -rf *.o $(APPS) *.txt *.ktr logs
