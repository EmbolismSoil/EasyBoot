LIB     = libdrivers.a

SOBJS   = $(filter $(CPU)_%.S,$(shell ls))
COBJS = $(filter $(CPU)_%.c ,$(shell ls))
COBJS += driver.c

SRCS    := $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS:=$(COBJS:%.c=%.o) $(SOBJS:%.S=%.o)

all:    $(LIB)
$(LIB): $(OBJS)
	@echo "CPU DRIVER >>>>>>>>> $(TESTOBJS)"
	$(AR) $(ARFLAGS) $@ $(OBJS)
#$(SOBJS):%.o:%.S
#	$(CC) $(CFLAGS) -c $<
