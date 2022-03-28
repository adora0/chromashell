SRC := chromashell.c
OBJS := $(SRC:.c=.o)

all: chromashell

chromashell: $(OBJS)

%.o: %.c
	@$(CC) -o $@ -c $<
