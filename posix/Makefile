O_TARGET = posix.o
O_OBJS = time.o pthread.o semaphore.o intr.o  priority.o

ifdef CONFIG_ERCOS_POSIX_TRACE
O_OBJS += trace.o
endif

include ../Rules.make
