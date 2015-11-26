# Copyright (C) Finite State Machine Labs Inc., 2000
# Released under the terms of the GNU  General Public License Version 2

alloverride:
	@basename `echo $(CC) | awk '{print $$1}'`

include Makefile

