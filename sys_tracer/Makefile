CC = gcc
CFLAGS =

APPS = sys_tracer

all: $(APPS)

sys_tracer: sys_tracer.c ../image/libercos.a
	@echo -e "	Building the system tracer application"
	@gcc $< -o sys_tracer -I../include -I../stub -L../image -lercos -ggdb



