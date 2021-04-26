
NAME = mp3-tagging
PYTHON = python3
PIP = pip3

DESTDIR ?= /
PREFIX ?= /usr/local
PYOPTIMIZE ?= 1

default: test
	@echo 'Run `make options` for a list of all options'

options: help
	@echo
	@echo 'Options:'
	@echo 'PYTHON = $(PYTHON)'

help:
	@echo 'make:                 Test $(NAME).'
	@echo 'make install:         Install $(NAME)'

test:
	$(PYTHON) mp3-tagging.py example

install:
	$(PYTHON) setup.py install

uninstall:
	$(PIP) uninstall $(NAME)

