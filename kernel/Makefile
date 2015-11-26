O_TARGET = kernel.o
O_OBJS = time.o task.o sched.o traps.o syscall.o sem.o mutex.o irqev.o list.o panic.o cs.o

ifndef CONFIG_ERCOS_SIMU
O_OBJS += cppinit.o
endif

ifdef CONFIG_ERCOS_TRACER
O_OBJS += tracer.o
endif


ifdef CONFIG_MM
O_OBJS +=  mm.o 
else
O_OBJS +=  nomm.o 
endif


include ../Rules.make
