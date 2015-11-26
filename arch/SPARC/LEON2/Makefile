O_TARGET = platform.o
O_OBJS = crt0.o timer.o halt.o cpu.o

ifdef CONFIG_DEBUG
O_OBJS += spurious.o serial.o
endif

include ../../../Rules.make
