DRIVERS_SRCS	= serial.s
DRIVERS_OBJS	= $(addprefix drivers/, $(DRIVERS_SRCS:.s=.o))
