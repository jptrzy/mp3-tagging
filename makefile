
.POSIX:

# ogg-utils version
VERSION = 0.1.0

# Paths
PREFIX = /usr/local
MANPREFIX = $(PREFIX)/share/man

LIBS = -I/usr/include/taglib -ltag -lz
_LDFLAGS = $(LIBS) $(LDFLAGS)\

GPP=g++

all: options ogg-tagging

options:
		@echo ogg-tagging build options:
		@echo "LDFLAGS = $(_LDFLAGS)"
		@echo "GPP      = $(GPP)"

ogg-tagging: clean
		$(GPP) -Ilib -Iinclude src/*.cpp -o $@ $(_LDFLAGS)

clean:
		rm -f ogg-tagging

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f ogg-tagging ${DESTDIR}${PREFIX}/bin

uninstall:
	rm "${DESTDIR}${PREFIX}/bin/ogg-tagging"
