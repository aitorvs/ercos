# Copyright (C) Finite State Machine Labs Inc., 2000
# Released under the terms of the GNU  General Public License Version 2

alloverride:
	@echo $(CC) $(CFLAGS) $(MODFLAGS) | sed 's/[^ ]* //; s/-I[^ ]* //; s/-fomit-frame-pointer//; s/-O2//'

include Makefile

