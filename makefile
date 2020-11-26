
PREFIX = /usr/local

all: main

main:
	cargo build --release
	strip target/release/mp3-tagging

install: main
	cp target/release/mp3-tagging ${DESTDIR}${PREFIX}/bin/mp3-tagging
	chmod 755 ${DESTDIR}${PREFIX}/bin/mp3-tagging


