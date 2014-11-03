SRCTREE		:= $(shell pwd)
TOPDIR		:= $(SRCTREE)
export  TOPDIR SRCTREE


OBJCFLAGS=--gap-fill=0xff 
CFLAGS=-I $(TOPDIR)/includes -g -O1
export OBJCFLAGS CFLAGS

ARCH=arm
CPU=s3c2440
BOARD=mini2440
export	ARCH CPU BOARD

LDSCRIPT:=$(TOPDIR)/boards/$(BOARD)/easy-boot.lds
LDFLAGS=-Bstatic -T $(LDSCRIPT)
export LDSCRIPT LDFLAGS

ifeq ($(ARCH),ppc)
CROSS_COMPILE = powerpc-linux-
endif
ifeq ($(ARCH),arm)
CROSS_COMPILE = arm-linux-
endif
ifeq ($(ARCH),mips)
CROSS_COMPILE = mips_4KC-
endif
ifeq ($(ARCH),nios)
CROSS_COMPILE = nios-elf-
endif
ifeq ($(ARCH),nios2)
CROSS_COMPILE = nios2-elf-
endif
ifeq ($(ARCH),m68k)
CROSS_COMPILE = m68k-elf-
endif
ifeq ($(ARCH),microblaze)
CROSS_COMPILE = mb-
endif
ifeq ($(ARCH),blackfin)
CROSS_COMPILE = bfin-elf-
endif
ifeq ($(ARCH),avr32)
CROSS_COMPILE = avr32-
endif
#
# Include the make variables (CC, etc...)
#
AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
CPP     = $(CC) -E
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
RANLIB  = $(CROSS_COMPILE)RANLIB
export	CROSS_COMPILE AS LD CC CPP AR NM STRIP OBJCOPY OBJDUMP RANLIB


OBJS  = cpu/$(CPU)/start.o
OBJS += cpu/$(CPU)/interrupt.o
LIBS = lib/libcommon.a
LIBS += drivers/libdrivers.a
LIBS += boards/$(BOARD)/lib$(BOARD).a
LIBS := $(addprefix $(SRCTREE)/,$(LIBS))
.PHONY : $(LIBS)


#########################################################################
#########################################################################

ALL = EasyBoot EasyBoot.bin

all:		$(ALL)

EasyBoot.bin:	EasyBoot
		$(OBJCOPY) ${OBJCFLAGS} -O binary $< $@
		cp $@ /srv/

EasyBoot: $(OBJS) $(LIBS) $(LDSCRIPT)
		#UNDEF_SYM_CMD=`$(OBJDUMP) -x $(LIBS) |sed  -n -e 's/\(__Easy_Boot_cmd\)/-u\1/p'|sort|uniq`;
		UNDEF_SYM_DRV=`$(OBJDUMP) -x $(LIBS) |sed  -n -e 's/.*\(__easy_boot_driver.*\)/-u\1/p'|sort|uniq`;\
		$(LD) $(LDFLAGS) $$UNDEF_SYM_DRV $(OBJS) \
			--start-group $(LIBS) --end-group -o $@

$(OBJS):
		$(MAKE) -C cpu/$(CPU)

$(LIBS):
		$(MAKE) -C $(dir $@)

.PHONY:clean
clean:
#	'find /home/lee/project/EasyBoot/ -type f -name "*.o" -o -name "*.a"';
	rm -f $(shell find /home/lee/project/EasyBoot/ -type f -name \
		"*.o" -o -name "*.a" -o -name "*.bin" -o -name "*.map")
	rm -f EasyBoot
