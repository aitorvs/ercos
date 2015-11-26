LDIRS = stdlib

ARCHDIR = $(DEBUGDIR)/lib

ARCHIVES = $(ARCHDIR)/stdlib/libstd.o

ifndef CONFIG_ERCOS_SIMU
#ARCHIVES += $(ARCHDIR)/$(ARCH)/libarch.o
#LDIRS += $(ARCH)
endif

all:
	@[ -d $(LIBDIR) ] || mkdir -p $(LIBDIR)
	@for i in $(LDIRS);do make DIR=lib/$$i -C $$i;done
	@$(LD) -r -o $(ARCHDIR)/libkernel.o $(ARCHIVES)
#	@$(AR) r $(LIBDIR)/libkernel.a $(ARCHDIR)/libkernel.o
	
include $(ROOTDIR)/Rules.make
