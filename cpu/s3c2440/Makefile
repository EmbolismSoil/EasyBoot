
LIB     = lib.a
SOBJS   = start.o
COBJS= interrupt.o 

SRCS    := $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS:=$(COBJS) $(SOBJS)

all:    $(LIB)
$(LIB): $(COBJS) $(SOBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
$(SOBJS):%.o:%.S
	$(CC) $(CFLAGS) -c $<
