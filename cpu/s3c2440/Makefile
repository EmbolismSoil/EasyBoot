
LIB     = lib.a
SOBJS   = start.o
COBJS=

SRCS    := $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS:=$(COBJS) $(SOBJS)

all:    $(LIB)
$(LIB): $(SOBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
#$(SOBJS):%.o:%.S
#	$(CC) $(CFLAGS) -c $<
