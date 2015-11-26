
#
# Especial variables that should not be exported
#
unexport O_TARGET
unexport O_OBJS


.PHONY: dummy 

#
# First rule in the source subdirectories
#
first_rule: dummy
	@$(MAKE) all_targets

#
# Common rules
#
.c.o:
	@[ -d $(DEBUGDIR)/$(DIR) ] || mkdirhier $(DEBUGDIR)/$(DIR)
	@echo "	Compiling\t$(DIR)/$<"
	@$(CC) $(CFLAGS) $(EXTRA_FLAGS) -c -o $(DEBUGDIR)/$(DIR)/$@ $(shell pwd)/$<
	
.cpp.o:
	@[ -d $(DEBUGDIR)/$(DIR) ] || mkdirhier $(DEBUGDIR)/$(DIR)
	@echo "	Compiling\t$(DIR)/$<"
	@$(CPP) $(CFLAGS) $(EXTRA_FLAGS) $(CXX_FLAGS) -c -o $(DEBUGDIR)/$(DIR)/$@ $(shell pwd)/$<
	
.S.o:
	@[ -d $(DEBUGDIR)/$(DIR) ] || mkdirhier $(DEBUGDIR)/$(DIR)
	@echo "	Compiling\t$(DIR)/$<"
	@$(CC) $(CFLAGS) -DST_DIV0=0x2 -I . -DASM -c -o $(DEBUGDIR)/$(DIR)/$@ $(shell pwd)/$<
	
#
# Making all the targets
#
all_targets: $(O_TARGET)

#
# Rule to compile a set of .o files into one .o file
#
$(O_TARGET): $(O_OBJS)
	@echo "	Linking\t\t$(DIR)/$@"
	@echo ""
	@(cd $(DEBUGDIR)/$(DIR);$(LD) -r -o $@ $(O_OBJS))

# vim:syn=make:
