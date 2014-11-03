
LIB     = libcommon.a
SOBJS   = $(filter %.S,$(shell ls))
COBJS   = $(filter %.c,$(shell ls))

SRCS    := $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS:=$(COBJS:%.c=%.o) $(SOBJS:%.S=%.o)

all:    $(LIB)
$(LIB): $(OBJS)
	echo $(COBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
#$(SOBJS):%.o:%.S
#	$(CC) $(CFLAGS) -c $<
