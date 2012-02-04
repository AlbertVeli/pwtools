eXe = gen_brute
OBJS = gen_brute.o

eXe2 = pwgen_stat
OBJS2 = pwgen_stat.o

eXe3 = move_mouse
OBJS3 = move_mouse.o

CC = gcc
RM = rm
CFLAGS = -W -Wall -O2
LIBS =
LIBS3 = -lX11 -lXmu
LDFLAGS =

all: $(eXe) $(eXe2) $(eXe3)

$(eXe): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(eXe2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LIBS)

$(eXe3): $(OBJS3)
	$(CC) $(LDFLAGS) -o $@ $(OBJS3) $(LIBS3)

.PHONY: clean

clean:
	$(RM) -f $(OBJS) $(OBJS2) $(OBJS3) $(eXe) $(eXe2) $(eXe3) *~

