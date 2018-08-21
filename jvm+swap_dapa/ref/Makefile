.PHONY: clean buildtest help

APPS	:= dapa wspgstat pghotness test_hashtable test_units
DAPA	:= dapa
WSPGST	:= wspgstat
PGHT	:= pghotness
HSTS	:= test_hashtable
TSTU	:= test_units

IDIR	:= ./include
CC	:= gcc
CFLAGS	:= -g -I$(IDIR) -O3 -Wall -Werror -std=gnu99
ODIR	:= ./object

_OBJ_DP	:= dapa.o
OBJ_DP	:= $(patsubst %,$(ODIR)/%,$(_OBJ_DP))

_OBJ_WS	:= wspgstat.o pageidle.o pfnofmap.o
OBJ_WS	:= $(patsubst %,$(ODIR)/%,$(_OBJ_WS))

_OBJ_PH	:= pghotness.o pageidle.o pfnofmap.o hashtable_bst.o misc.o
OBJ_PH	:= $(patsubst %,$(ODIR)/%,$(_OBJ_PH))

_OBJ_TS	:= test_hashtable.o hashtable_bst.o
OBJ_TS	:= $(patsubst %,$(ODIR)/%,$(_OBJ_TS))

_OBJ_TU	:= test_units.o pfnofmap.o
OBJ_TU	:= $(patsubst %,$(ODIR)/%,$(_OBJ_TU))

all: $(APPS)

$(ODIR)/%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(DAPA): $(OBJ_DP)
	$(CC) -o $@ $^ $(LIBS)

$(WSPGST): $(OBJ_WS)
	$(CC) -o $@ $^ $(LIBS)

$(PGHT): $(OBJ_PH)
	$(CC) -o $@ $^ $(LIBS)

$(HSTS): $(OBJ_TS)
	$(CC) -o $@ $^ $(LIBS)

$(TSTU): $(OBJ_TU)
	$(CC) -o $@ $^ $(LIBS)

clean:
	rm -f $(ODIR)/*.o $(APPS)

buildtest:
	./test_build.sh

help:
	@echo "Usage: make <target>"
	@echo ""
	@echo "targets: all $(APPS) clean buildtest help"
	@echo ""
