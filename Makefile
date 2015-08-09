eXeS = gen_brute pwgen_stat permute
# Comment out row below if no X
#eXeS += move_mouse

gen_brute_OBJS = gen_brute.o
pwgen_stat_OBJS = pwgen_stat.o
permute_OBJS = permute.o
move_mouse_OBJS = move_mouse.o
move_mouse_LIBS = -lX11 -lXmu

#DEBUG = yes
ifdef DEBUG
DBGFLAGS = -g -O0
else
DBGFLAGS = -O3
endif
CFLAGS = $(DBGFLAGS) -pipe -W -Wall

LIBS =
RM = rm

all: $(eXeS)

gen_brute: $(gen_brute_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(gen_brute_OBJS) $(LIBS)

pwgen_stat: $(pwgen_stat_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(pwgen_stat_OBJS) $(LIBS)

permute: $(permute_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(permute_OBJS) $(LIBS)

move_mouse: $(move_mouse_OBJS)
	$(CC) $(LDFLAGS) -o $@ $(move_mouse_OBJS) $(move_mouse_LIBS)

.PHONY: clean

clean:
	$(RM) -f gen_brute pwgen_stat permute move_mouse *.o *~

