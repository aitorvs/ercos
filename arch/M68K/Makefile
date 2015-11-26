KDIRS = $(ARCH)/common $(ARCH)/$(PLATFORM)

ARCHDIR = $(DEBUGDIR)/arch

KARCHIVES = $(ARCHDIR)/$(ARCH)/$(PLATFORM)/platform.o $(ARCHDIR)/$(ARCH)/common/common.o

all:
	@for i in $(KDIRS);do make DIR=arch/$$i -C $$i;done
	@$(LD) -r -o $(ARCHDIR)/arch.o $(KARCHIVES)
	
include $(ROOTDIR)/Rules.make

