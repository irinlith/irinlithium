CFLAGS ?= -Ofast
CFLAGS += -fPIC -Wall -g -L/usr/local/lib -I/usr/local/include
SO_CFLAGS = -L/usr/local/lib -I/usr/local/include -lX11 -shared -lpng
PREFIX = /usr/local

build: libirinlithium.so

install: build
	mkdir -p $(PREFIX)/include/irinlithium
	cp -f *.h $(PREFIX)/include/irinlithium
	cp -f libirinlithium.so $(PREFIX)/lib/

clean:
	rm -rf build irinlithium.so

libirinlithium.so: build/irinlithiumImage.o  build/irinlithiumString.o  build/irinlithiumVKB.o
	$(CC) $(CFLAGS) $(SO_CFLAGS) $^ -o $@

build/%.o: %.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $^ -o $@
