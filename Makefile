CC= gcc -std=gnu99
SRC= src/hbuf.c src/spp.c src/spp_lua.c
LUA_PREFIX= lua
LIBS= -lm -l$(LUA_PREFIX)
DLIB= spp_lua.so
OBJS= spp_lua.o spp.o hbuf.o

CFLAGS= -O2 -Wall -fPIC
LINUX_CFLAGS= -shared $(LIBS)
MACOSX_CFLAGS= -bundle -undefined dynamic_lookup

ifeq ($(shell uname), Darwin)
	CFLAGS += $(MACOSX_CFLAGS)
else
	CFLAGS += $(LINUX_CFLAGS)
endif

build:
	$(CC) -c $(SRC) $(EXTRA_FLAGS)
	$(CC) $(CFLAGS) $(DFLAGS) $(SRC) -o $(DLIB) $(EXTRA_FLAGS)

clean:
	rm $(DLIB) $(OBJS)
