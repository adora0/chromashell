SRC := chromashell.c
OBJS := $(SRC:.c=.o)

all: chromashell

chromashell: $(OBJS)

%.o: %.c
	@$(CC) -MD -o $@ -c $<

-include $(OBJS:.o=.d)
