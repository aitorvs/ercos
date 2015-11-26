O_TARGET = common.o

ifdef CONFIG_FAST_MODE
O_OBJS = context.o cpu.o window.o isr.o syscall.o
else
O_OBJS = context2.o cpu.o window2.o isr2.o syscall2.o
endif


	
include ../../../Rules.make
