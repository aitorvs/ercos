O_TARGET = platform.o
O_OBJS = crt0.o timer.o halt.o cpu.o

ifdef CONFIG_DEBUG
O_OBJS += spurious.o serial.o
endif

ifdef CONFIG_GDB_MONITOR
O_OBJS += erc32-stub.o serialdbg.o

ifdef CONFIG_FAST_MODE
O_OBJS += gdebug.o
else
O_OBJS += gdebug2.o
endif

endif

include ../../../Rules.make
