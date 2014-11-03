LIB     = libdrivers.a
SOBJS   = 
COBJS   = driver.o leds.o uart.o

SRCS    := $(SOBJS:.o=.S) $(COBJS:.o=.c)
OBJS:=$(COBJS) $(SOBJS)

all:    $(LIB)
$(LIB): $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)
#$(SOBJS):%.o:%.S
#	$(CC) $(CFLAGS) -c $<
