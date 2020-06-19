CC=arm-linux-gnueabihf-gcc

# Location of header files
IDIR=include
# Location of source files
SDIR=src
# Location of output files
ODIR=obj
# Location of thirdparty libraries
LDIR=lib

EXE=rcbot-cam
DESTDIR=
# DESTDIR=/tmp/rcbot-staging
PREFIX=/usr/local


CFLAGS= \
-Wall \
-I $(IDIR) \
-I ~/common-libs/wiringPi/include \
-L $(LDIR) \
-L /home/rc/git/raspberrypi/rootfs/usr/lib/arm-linux-gnueabihf


LIBS=-lwiringPi -lmosquitto -lpthread

_DEPS=config.h common_utils.h cam_controller.h mqtt_custom_api.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = cam_controller.o mqtt_custom_api.o main.o
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