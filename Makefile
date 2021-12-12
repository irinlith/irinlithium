CFLAGS ?= -Ofast
CFLAGS += -fPIC -Wall -g -I/usr/local/include
SO_CFLAGS = -L/usr/local/lib -I/usr/local/include -lX11 -shared -lpng
PREFIX = /usr/local

build: libirinlithium.so

install: build
	mkdir -p $(PREFIX)/include/irinlithium
	cp -f *.h $(PREFIX)/include/irinlithium
	cp -f libirinlithium.so $(PREFIX)/lib/

clean:
	rm -rf build libirinlithium.so

libirinlithium.so: build/image.o  build/string.o  build/VKB.o build/tcp.o
	$(CC) $(CFLAGS) $(SO_CFLAGS) $^ -o $@

build/%.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@
