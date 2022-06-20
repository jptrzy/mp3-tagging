GPP=g++

# -Wall -g3
# pmap $(ps -o pid,command ax | grep metro | cut -d ' ' -f 3 | sed "1q")
#pkg-config --libs --cflags taglib

all: options ogg-tagging

options:
		@echo ogg-tagging build options:
		@echo "GPP      = $(GPP)"


ogg-tagging: clean
		$(GPP) -Ilib -Iinclude src/*.cpp -o $@ -I/usr/include/taglib -ltag -lz

clean:
		rm -f ogg-tagging