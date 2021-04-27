 
PREFIX = /usr/local

all: main

main:
	cargo build --release
	strip target/release/mp3-tagging

install:
	cp target/release/mp3-tagging ${DESTDIR}${PREFIX}/bin/mp3-tagging
	chmod 755 ${DESTDIR}${PREFIX}/bin/mp3-tagging

remove:
	rm -f target/release/mp3-tagging ${DESTDIR}${PREFIX}/bin/mp3-tagging

