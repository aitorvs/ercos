#/**
# * $Revision: 1252 $ $Date: 2007-11-28 17:20:19 +0100 (mié, 28 nov 2007) $
# * Last CVS commit by $Author: cesar $
# * $Log$
# */
KDIRS = $(ARCH)/common $(ARCH)/$(PLATFORM)
ARCHDIR = $(DEBUGDIR)/arch

KARCHIVES = $(ARCHDIR)/$(ARCH)/$(PLATFORM)/platform.o $(ARCHDIR)/$(ARCH)/common/common.o

all:
	@for i in $(KDIRS);do $(MAKE) DIR=arch/$$i -C $$i;done
	@$(LD) -r -o $(ARCHDIR)/arch.o $(KARCHIVES)
	
include $(ROOTDIR)/Rules.make

