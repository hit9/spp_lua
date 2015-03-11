CC= gcc -std=gnu99
CFLAGS= -O2 -Wall -fPIC -shared 
LIBS= -llua
SRC= src/*.c
OUT= -o spp_lua.so

build:
	$(CC) $(SRC) $(CFLAGS) $(LIBS) $(OUT)

clean:
	rm -rf *.so
