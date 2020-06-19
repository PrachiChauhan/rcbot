# CC=arm-linux-gnueabihf-gcc
CC=gcc

# Location of header files
IDIR=include
# Location of source files
SDIR=src
# Location of output files
ODIR=obj
# Location of thirdparty libraries
LDIR=lib

EXE=rcbot-speech-recognizer
DESTDIR=
# DESTDIR=/tmp/rcbot-staging
PREFIX=/usr/local


CFLAGS= \
-Wall \
-I $(IDIR) \
-I/usr/local/include \
-I/usr/local/include/sphinxbase \
-I/usr/local/include/pocketsphinx \
-I/usr/local/include \
-I/usr/local/include/sphinxbase\
-L $(LDIR) \
-L/usr/local/lib \
-L /home/rc/git/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf


LIBS=-lmosquitto -lpocketsphinx -lsphinxbase -lsphinxad -lasound -lpthread -lm

_DEPS=config.h common_utils.h mqtt_custom_api.h speech_recognizer.h command_processor.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = mqtt_custom_api.o speech_recognizer.o command_processor.o main.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)


.PHONY: install
install: $(EXE)
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp $< $(DESTDIR)$(PREFIX)/bin/

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(EXE)

.PHONY: clean
clean:
	rm -f $(EXE) $(ODIR)/*.o *~ core $(INCDIR)/*~