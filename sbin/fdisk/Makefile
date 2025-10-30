TARGET	= parted
SRCS	= main.c
OBJS	= $(SRCS:.c=.o)

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	$(RM) $(TARGET) $(OBJS)

.PHONY: install
install: $(TARGET)
