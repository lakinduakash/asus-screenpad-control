CC=gcc
PKGCONFIG = $(shell which pkg-config)

CFLAGS=`pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0 --libs x11`

APP_NAME="screenpad-control"

PREFIX=/usr
BINDIR=$(PREFIX)/bin
APP_DIR=$(PREFIX)/share/$(APP_NAME)

ODIR=build

GLIB_COMPILE_RESOURCES = $(shell $(PKGCONFIG) --variable=glib_compile_resources gio-2.0)

BUILT_SRC = resources.c

_OBJ = main.o ui.o $(BUILT_SRC:.c=.o)
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

all: resources.c $(ODIR)/screenpad-control

resources.c: glade/screenpadcontrol.gresource.xml glade/window.ui
	$(GLIB_COMPILE_RESOURCES) glade/screenpadcontrol.gresource.xml --target=$@ --sourcedir=glade --generate-source

$(ODIR)/%.o: %.c build
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR):
	mkdir -p $@

$(ODIR)/screenpad-control: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o
	rm -f $(BUILT_SRC)
	rm -f $(ODIR)/screenpad-control