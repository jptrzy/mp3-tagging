
.POSIX:

# ogg-utils version
VERSION = 0.1.0

# Paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

LIBS = -I/usr/include/taglib -ltag -lz
_LDFLAGS = $(LIBS) $(LDFLAGS)\

CPP=g++

all: options ogg-tagging

options:
		@echo ogg-tagging build options:
		@echo "LDFLAGS = $(_LDFLAGS)"
		@echo "CPP      = $(CPP)"

ogg-tagging: clean
		$(CPP) -Ilib -Iinclude src/*.cpp -o $@ $(_LDFLAGS)

clean:
		rm -f ogg-tagging

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ogg-tagging ${DESTDIR}${PREFIX}/bin

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/ogg-tagging"

test: options ogg-tagging
	rm -rf test/Yes-Fragile-1*
	cp ~/Downloads/Yes-Fragile-1* test/

	./ogg-tagging -p test
