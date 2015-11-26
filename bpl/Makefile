O_TARGET = bpl.o
O_OBJS = edroombp.o edroomsl.o edroomtc.o

all: first_rule
	@[ -d $(LIBDIR) ] || mkdir -p $(LIBDIR)
	@echo "	Making the BPL static library\n"
	@(cd $(DEBUGDIR)/$(DIR);$(AR) r libbpl.a $(O_OBJS);mv libbpl.a $(LIBDIR))
	


include ../Rules.make
