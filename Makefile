eXe = gen_brute
OBJS = gen_brute.o

eXe2 = pwgen_stat
OBJS2 = pwgen_stat.o

CC = gcc
RM = rm
CFLAGS = -W -Wall -O2
LIBS =
LDFLAGS =

all: $(eXe) $(eXe2)

$(eXe): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(eXe2): $(OBJS2)
	$(CC) $(LDFLAGS) -o $@ $(OBJS2) $(LIBS)

.PHONY: clean

clean:
	$(RM) -f $(OBJS) $(OBJS2) $(eXe) $(eXe2) *~

