O_TARGET = test.o
O_OBJS = global_test.o timing.o utils.o 

ifdef CONFIG_ERCOS_REC_TEST
O_OBJS += Test1.o
endif

ifdef CONFIG_ERCOS_SEND_RECEIVE_TEST
O_OBJS += Test2.o
endif

ifdef CONFIG_ERCOS_TIME_TEST
O_OBJS += Test3.o
endif

ifdef CONFIG_ERCOS_SIL_REC_TEST
#O_OBJS += Test51-s.o
#O_OBJS += Test5.o
O_OBJS += Test53.o
endif

ifdef CONFIG_ERCOS_CTX_TEST
O_OBJS += lat_ctx2.o
endif

ifdef CONFIG_ERCOS_IRQ_TEST
O_OBJS += Test8.o
endif

ifdef CONFIG_ERCOS_LAT_TEST
O_OBJS += lat_intr.o
endif

ifdef CONFIG_ERCOS_PRIOCEILING_TEST
O_OBJS += Test52.o
endif

ifdef CONFIG_ERCOS_MISC_TEST
#O_OBJS += Test6.o
#O_OBJS += Test8.o
#O_OBJS += Test81.o
#O_OBJS += Test82.o
O_OBJS += Test10.o
endif

include ../Rules.make
