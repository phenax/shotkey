# daemonic - dynamic window manager
# See LICENSE file for copyright and license details.

include config.mk

SRC = daemonic.c
OBJ = ${SRC:.c=.o}

all: clean options daemonic

options:
	@echo daemonic build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.mk

daemonic: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f daemonic ${OBJ}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f daemonic ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/daemonic
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < daemonic.1 > ${DESTDIR}${MANPREFIX}/man1/daemonic.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/daemonic.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/daemonic\
		${DESTDIR}${MANPREFIX}/man1/daemonic.1

.PHONY: all options clean dist install uninstall
