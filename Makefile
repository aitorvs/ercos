#
# Makefie principal del proyecto ERCOS-RT on ERC-32
# Copyright (c) Aitor Viana S�nchez <avs@aut.uah.es>
#
# File:    $Source$
# 
# $Id: Makefile 1273 2008-03-13 17:29:27Z cesar $
#  
# $Revision: 1273 $
# $Date: 2008-03-13 18:29:27 +0100 (jue, 13 mar 2008) $
# Last CVS commit by $Author: cesar $
# 
#

.EXPORT_ALL_VARIABLES:

MAKEFLAGS = --no-print-directory

#
# make all
#
all: do-it-all

#
# Archivo que contiene la configuraci�n por defecto
# Si no existe el archivo se mostrar� un mensaje de aviso por pantalla
#

ifeq (.config,$(wildcard .config))
include .config
do-it-all: subdirs install
else
do-it-all: warning
endif


#
# Compilador cruzado para construir el proyecto
#
ifeq ($(CONFIG_ERCOS_SIMU), y)

CROSS_COMPILE =

endif



MKDEP = scripts/mkdep

# 
# Compilador del host junto con sus flags de compilaci�n
# 
HOSTCC = gcc
HOSTCFLAGS      = -O2 -fomit-frame-pointer

## Variables utilizadas para xconfig ###
HOSTARCH := $(shell uname -m | sed -e s/i.86/i386/)
PLATFORM := $(shell config/setconfig)
ARCH := $(shell config/setarch)

TOPDIR  := $(shell if [ "$$PWD" != "" ]; then echo $$PWD; else pwd; fi)
ROOTDIR  = $(shell pwd)
IMAGEDIR = $(ROOTDIR)/image
SCRIPTSDIR = $(ROOTDIR)/config/scripts
DEBUGDIR = $(ROOTDIR)/Debug
FINDPATH = include/lib include/net include/erc32 include
LIBDIR		:= $(DEBUGDIR)/libs

#
# Cabeceras de inclusión
#
HPATH = $(TOPDIR)/include
LIBHPATH = $(TOPDIR)/include/lib

#
# Flags de compilación
#
CFLAGS = -Wall -fomit-frame-pointer -O2 \
-fno-strength-reduce -fno-builtin -pipe -nostdinc -fno-exceptions #-mno-app-regs
CXX_FLAGS = -fno-rtti

ifeq ($(PLATFORM), LEON2)
CFLAGS += -mv8
#CFLAGS += -qleon2 -mv8
endif

ifeq ($(PLATFORM), 68332)
CFLAGS += -mcpu32
endif

ifndef CONFIG_HAS_FPU
ifeq ($(PLATFORM), LEON2)
CFLAGS += -msoft-float
endif
endif

#
# Herramientas de compilación (cruzada o no)
#
#
ifeq ($(ARCH), SPARC)
CROSS_COMPILE = sparc-elf-
#CROSS_COMPILE = sparc-rtems-
endif

ifeq ($(ARCH), M68K)
CROSS_COMPILE = m68k-elf-
endif

AS     = $(CROSS_COMPILE)as
LD     = $(CROSS_COMPILE)ld
CC     = $(CROSS_COMPILE)gcc -I$(HPATH) -I$(LIBHPATH) $(BPLPATH) $(SIMUPATH)
CPP	   = $(CROSS_COMPILE)g++ -I$(HPATH) -I$(LIBHPATH) $(BPLPATH) $(SIMUPATH) #-fno-builtin-function -nostdinc++
AR     = $(CROSS_COMPILE)ar
NM     = $(CROSS_COMPILE)nm
OBJCPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
STRIP  = $(CROSS_COMPILE)strip
MAKE   = make

#
# Variables que contiene los nombres de los archivos
#
NAME = image
TARGET = $(IMAGEDIR)/$(NAME)
ELFTARGET = $(IMAGEDIR)/$(NAME).elf
ROMIMAGE = $(IMAGEDIR)/$(NAME).rom
RAMIMAGE = $(IMAGEDIR)/$(NAME).ram
BINIMAGE = $(IMAGEDIR)/$(NAME).bin
SRECIMAGE = $(IMAGEDIR)/$(NAME).srec
LSTFILE = $(IMAGEDIR)/$(NAME).lst
MAPFILE = $(IMAGEDIR)/System.map
LIBERCOS = $(IMAGEDIR)/libercos.a
LIBGCC:=$(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

#
# Subdirectorios que componen el proyecto
#
ifeq ($(CONFIG_ERCOS_SIMU), y)

SIMUPATH = -I$(TOPDIR)/stub

SUBDIRS		:= stub init kernel
ARCHIVES 	:= $(DEBUGDIR)/stub/stub.o $(DEBUGDIR)/init/init.o $(DEBUGDIR)/kernel/kernel.o

else

SUBDIRS		:= arch init kernel lib
#
# Archivos objeto de cada parte del proyecto
#
ARCHIVES 	:= $(DEBUGDIR)/arch/arch.o $(DEBUGDIR)/init/init.o $(DEBUGDIR)/kernel/kernel.o 
ARCHIVES += $(DEBUGDIR)/lib/libkernel.o

APPLICATION =

ifdef CONFIG_PRIM_PP
BPLPATH = -I$(TOPDIR)/bpl
LIBS += -lbpl
endif

ifdef CONFIG_DEBUG
#SUBDIRS += lib
#ARCHIVES += $(DEBUGDIR)/lib/lib.o
#LKERNEL = -lkernel
endif

ifdef CONFIG_ERCOS_AUTOMATIC_TEST
TEST=new
#else
#TEST=old
endif

ifdef CONFIG_ERCOS_AUTOMATIC_TEST
SUBDIRS += test_$(TEST)
ARCHIVES += $(DEBUGDIR)/test_$(TEST)/test.o
endif

ifdef CONFIG_ERCOS_EDROOM_CODE
SUBDIRS += EDROOMCode
APPLICATION += $(DEBUGDIR)/EDROOMCode/EDROOMCode.o
endif

ifdef CONFIG_ERCOS_POSIX
SUBDIRS += posix
ARCHIVES += $(DEBUGDIR)/posix/posix.o
endif

ifdef CONFIG_PRIM_PP
SUBDIRS += bpl
#ARCHIVES += $(DEBUGDIR)/bpl/bpl.o 
endif

ifdef CONFIG_GDB_MONITOR
SUBDIRS += gdb
ARCHIVES += $(DEBUGDIR)/gdb/gdb.o
endif

endif	# ifeq ($(CONFIG_ERCOS_SIMU), y)

ifdef CONFIG_ERCOS_TRACER
pre_sim:
	@-rm log.bin
	@touch log.bin
sis: pre_sim
	@sis $(SRECIMAGE) -uart2 log.bin

tsim: pre_sim
	@tsim-erc32 $(ELFTARGET) -uart2 log.bin

trace:
	@-mkdir tools/logs
	@-mv log.bin tools/logs
	@./tools/lolo tools/logs/log.bin > tools/logstrace_event.txt
	@./tools/lolo2 tools/logstrace_event.txt tools/logskiwifile.ktr
	@./tools/kiwi tools/logskiwifile.ktr&
endif

#
# Regla encargada de compilar todo el proyecto
#
subdirs: .config
#	@-(cd include;ln -s sparc/erc32 arch;ln -s sparc/cpu cpu; ln -s ../bpl/edroombp.h edroombp.h; ln -s ../bpl/edroomdf.h edroomdf.h; ln -s ../bpl/edroomtc.h edroomtc.h)
	@-echo Building subdirs for architectura ${ARCH} platform ${PLATFORM} 
	@-(cd include; [ -h arch ] || ln -s ${ARCH}/${PLATFORM} arch; [ -h cpu ] || ln -s ${ARCH}/cpu cpu)
	@-([ -h link.ld ] || ln -s config/${PLATFORM}/link_${PLATFORM}.ld link.ld)
	
    # Creamos el directorio que contendr� las im�genes del micron�cleo
	@-echo Create directory ${IMAGEDIR} for kernel core location
	@[ -d $(IMAGEDIR) ] || mkdir $(IMAGEDIR)
	
    # Creamos el directorio donde se almacenar�n todos los archivos objeto
	@-echo Create directory ${DEBUGDIR} for object code files
	@[ -d $(DEBUGDIR) ] || mkdir $(DEBUGDIR)
	@[ -d $(DEBUGDIR)/arch/$(ARCH)/$(PLATFORM) ] || mkdir -p $(DEBUGDIR)/arch/$(ARCH)/$(PLATFORM)
	
    # Recorremos todos los subdirectorios que posee el proyecto realizando la compilaci�n
	@for i in $(SUBDIRS); do $(MAKE) DIR=$$i -C $$i; done
	
	@echo "	Building Kernel Library\t$(LIBERCOS)"

	@$(AR) r $(LIBERCOS) $(ARCHIVES)

	@echo "	Building ELF image\t$(TARGET)"

    # Realizamos el proceso de enlazado con el script link.ld
	@$(LD) -nostdlib -M -T link.ld $(ARCHIVES) $(APPLICATION) -o $(TARGET) -L$(LIBDIR) $(LKERNEL) $(LIBS) $(LIBGCC) > $(IMAGEDIR)/image.map

    # Generamos un desensamblado de la imagen
	@echo "	Disassembling image file\t$(LSTFILE)"
	@$(OBJDUMP) -d $(TARGET) > $(LSTFILE)

#
# Configuracion del nucleo para que se cargue desde memoria RAM
#
ifeq ($(CONFIG_RAMKERNEL), y)
	@echo "\n\tBuilding kernel linked for RAM...\n"

	@cp $(TARGET) $(ELFTARGET)

	@echo "	Building RAM Binary image\t$(RAMIMAGE)"
	@$(OBJCPY) -O binary $(ELFTARGET) $(RAMIMAGE)
	
ifeq ($(ARCH), SPARC)
	@mkprom-erc32 $(ELFTARGET) -freq 16 -wdog -ramsize 4096 \
	-romsize 512 -romws 0 -ramws 0 -iows0 0 -o $(ROMIMAGE)
	
	@$(STRIP) $(ROMIMAGE)

	@echo "	Building Binary image\t$(BINIMAGE)"
	@$(OBJCPY) -O binary $(ROMIMAGE) $(BINIMAGE)

	@echo "	Building SRECORD image\t$(SRECIMAGE)"
	@$(OBJCPY) -O srec $(ROMIMAGE) $(SRECIMAGE)
endif

ifeq ($(ARCH), M68K)
	@echo "FIXME -- Building rom image, huh!"
	
	@echo "FIXME -- Stripping rom image, huh!"

	@echo "	Building Binary image\t$(BINIMAGE)"
	@$(OBJCPY) -O binary $(ELFTARGET) $(BINIMAGE)

	@echo "	Building SRECORD image\t$(SRECIMAGE)"
	@$(OBJCPY) -O srec $(ELFTARGET) $(SRECIMAGE)
endif

#
# Configuracion del nucleo para que sea cargado desde memoria ROM
#
else
	@echo "\n\tBuilding kernel linked for ROM...\n"

ifeq ($(ARCH), SPARC)
    # Movemos tanto la seccion de datos como la de codigo a la ROM
    # La de datos la movemos de una forma relativa (restandole 0x02000000)
    # La de codigo la movemos de forma absoluta
    # Con esto conseguimos que se pueda ejecutar desde una memoria ROM
	@$(OBJCPY) --change-section-address .data-0x02380000 \
		   --change-section-address .text-0x02380000 \
		   -O elf32-sparc $(TARGET) $(ELFTARGET)
endif

	@echo "	Building SRECORD image\t$(SRECIMAGE)"
	@$(OBJCPY) -O srec $(ELFTARGET) $(SRECIMAGE)

    # Generamos el archivo que sera grabado en la RAM
	@echo "	Building Binary image\t$(BINIMAGE)"
	@$(OBJCPY) -O binary $(ELFTARGET) $(BINIMAGE)
endif

    # Creamos el archivo que contiene el mapa de simbolos del micronucleo
	@echo "	Creating MAP file\t$(MAPFILE)"
	@$(NM) $(ELFTARGET) |grep -v '\(compiled\)\|\(\.o$$\)\|\( a \)'|sort > $(MAPFILE)

# @make -C tools

.PHONY: install
install:
ifeq ($(ARCH), M68K)
	@echo "\n\tInstallation over samba from saiph to I0060B0F8496E.inta.es"
	mkdir -p /mnt/inta/image/
	rm -f /mnt/inta/image/$(NAME)*
	rm -f /mnt/inta/image/libercos.a
	rm -f /mnt/inta/image/libercos.a
	rm -f /mnt/inta/image/System.map

	cp $(TARGET) /mnt/inta/image
	cp $(ELFTARGET) /mnt/inta/image
	cp $(RAMIMAGE) /mnt/inta/image
	cp $(BINIMAGE) /mnt/inta/image
	cp $(SRECIMAGE) /mnt/inta/image
	cp $(SRECIMAGE) /mnt/inta/image/$(NAME).s19
	cp $(LSTFILE) /mnt/inta/image
	cp $(MAPFILE) /mnt/inta/image
	cp $(LIBERCOS) /mnt/inta/image
endif

ifeq ($(ARCH), SPARC)
	@echo "Nothing defined for installation on SPARC"
endif


#
# Construccion del binario que realiza las dependencias
#
$(MKDEP): $(MKDEP).c
	@echo "\tMaking dep binary"
	@$(HOSTCC) $(HOSTFLAGS) -o $@ $<

.PHONY: clean dummy distclean mrproper depend dep-files warning

#
# Regla que muestra un aviso por pantalla devido a que no se ha encontrado ninguna 
# configuracion para el proceso de compilacion
#
warning:
	@echo "\tNo configuration found"
	@echo "\tPlease type \"make xconfig\""

# 
# Regla que se encarga de crear los archivos de dependencias .depend y .hdepend
#
dep-files: $(MKDEP)
	@echo "\tARCH ${ARCH}"
	@$(MKDEP) main/*.c > .tmpdepend
	@$(MKDEP) `find $(FINDPATH) -follow -name *.h -print` >.hdepend
	@set -e;for i in $(SUBDIRS); do $(MAKE) -C $$i fastdep; done
	@mv .tmpdepend .depend

	
#
# Regla principal para construir dependencias
#
depend dep: dep-files

# Proceso de borrado de objetos e im�genes
clean:
	@echo "	Cleanup\t$(@D)"
	@-$(MAKE) -C $(SCRIPTSDIR)/ clean
	@echo "	Cleaning image directory"
	@echo "	Cleaning Debug directory"
	@- rm -rf *.o *~ $(IMAGEDIR) $(DEBUGDIR) $(MKDEP) 2> /dev/null
	@- rm -f include/arch include/cpu include/edroombp.h include/edroomtc.h include/edroomdf.h 2> /dev/null
	@- rm -rf .hdepend $(MKDEP) 2> /dev/null
	@- rm -rf log.bin .depend `find . -name .depend -print` 2> /dev/null
	@-rm -f link.ld 2> /dev/null
	@-make -C tools clean

# Borramos hasta la configuraci�n del proceso de compilaci�n
distclean mrproper: clean
	@-rm .config 2> /dev/null
	@-rm include/autoconf.h 2> /dev/null
	@-rm .config_arch 2> /dev/null
	@-rm config.tk 2> /dev/null
	@-rm .config.old 2> /dev/null

config.tk: config.in
	$(MAKE) -C $(SCRIPTSDIR) tkparse
	ARCH=dummy $(SCRIPTSDIR)/tkparse < config.in > config.tmp
	@echo ${ARCH}
	@if [ -f /usr/local/bin/wish ]; then \
		echo '#!'"/usr/local/bin/wish -f" > config.tk; \
	else \
		echo '#!'"/usr/bin/wish -f" > config.tk; \
	fi
	cat $(SCRIPTSDIR)/header.tk >> ./config.tk
	cat config.tmp >> config.tk
	rm -f config.tmp
	echo "set defaults \"/dev/null\"" >> config.tk
	echo "set help_file \"config/Configure.help\"" >> config.tk
	cat $(SCRIPTSDIR)/tail.tk >> config.tk
	chmod 755 config.tk

.PHONY: xconfig
xconfig: config.tk
	touch include/autoconf.h
	@wish -f config.tk
	@if [ ! -f .config ]; then \
		echo; \
		echo "You have not saved your config, please re-run make config"; \
		echo; \
		exit 1; \
	fi
	mv .config .config_arch
	@if egrep "^CONFIG_DEFAULTS_VENDOR=y" .config_arch > /dev/null; then \
		$(MAKE) config_xconfig; \
	fi

# 
# Configuraci�n gr�fica gracias a los scripts prestados de linux 
# 
kconfig.tk: dummy
	@if [ -f .config.old ]; then \
		echo; \
	else \
		cat config/${PLATFORM}/defconfig > .config; \
	fi
	$(MAKE) -C $(SCRIPTSDIR) tkparse
	@if [ -f /usr/local/bin/wish ]; then \
		echo '#!'"/usr/local/bin/wish -f" > kconfig.tk; \
	else \
		echo '#!'"/usr/bin/wish -f" > kconfig.tk; \
	fi
	cat $(SCRIPTSDIR)/header.tk >> ./kconfig.tk
	$(SCRIPTSDIR)/tkparse < config/${PLATFORM}/config.in >> kconfig.tk
	echo "set defaults \"config/${PLATFORM}/defconfig\"" >> kconfig.tk
	echo "set PLATFORM \"${PLATFORM}\"" >> kconfig.tk
	cat $(SCRIPTSDIR)/tail.tk >> kconfig.tk
	chmod 755 kconfig.tk

#
# Regla principla que se encarga de la configuracion grafica
#
config_xconfig: clean kconfig.tk config/${PLATFORM}/config.in 
	wish -f kconfig.tk
	rm -f kconfig.tk

#
# Archivo de reglas de compilaci�n
#
include Rules.make
